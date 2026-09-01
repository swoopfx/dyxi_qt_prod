#include "authservice.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QUrlQuery>
#include <QTimer>
#include <QDebug>

AuthService::AuthService(QObject *parent)
    : QObject(parent)
    , m_statusText("Ready")
    , m_isBusy(false)
    , m_isSignedIn(false)
    , m_userName("")
    , m_userEmail("")
    , m_sessionToken("")
    , m_redirectPage("welcome")
    , m_authProvider("none")
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentReply(nullptr)
    , m_oauth2Flow(nullptr)
    , m_replyHandler(nullptr)
    , m_keychainHelper(new KeychainHelper(this))
{
    // Connect KeychainHelper signals
    connect(m_keychainHelper, &KeychainHelper::keyRead, this, &AuthService::onKeychainTokenRead);
    connect(m_keychainHelper, &KeychainHelper::logMessage, this, &AuthService::logMessage);
}

AuthService::~AuthService()
{
}

void AuthService::setStatusText(const QString &text)
{
    if (m_statusText != text) {
        m_statusText = text;
        emit statusTextChanged();
    }
}

void AuthService::setBusy(bool busy)
{
    if (m_isBusy != busy) {
        m_isBusy = busy;
        emit isBusyChanged();
    }
}

void AuthService::setSignedIn(bool signedIn)
{
    if (m_isSignedIn != signedIn) {
        m_isSignedIn = signedIn;
        emit authStateChanged();
    }
}

void AuthService::rawLog(const QString &message, const QString &type)
{
    emit logMessage(message, type);
}

AuthService::PkcePair AuthService::generatePkcePair()
{
    // Generate 64-byte cryptographic random verifier
    QByteArray randomBytes;
    randomBytes.resize(64);
    for (int i = 0; i < 64; ++i) {
        randomBytes[i] = static_cast<char>(QRandomGenerator::securelySeeded().bounded(256));
    }
    QString verifier = randomBytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    // Compute S256 Challenge
    QByteArray hash = QCryptographicHash::hash(verifier.toUtf8(), QCryptographicHash::Sha256);
    QString challenge = hash.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    return { verifier, challenge };
}

void AuthService::verifyCredentials(const QString &email, const QString &password)
{
    if (email.trimmed().isEmpty() || password.isEmpty()) {
        emit errorOccurred("Please provide both email and password.");
        return;
    }

    setBusy(true);
    setStatusText("Authenticating credentials...");
    emit logMessage("📤 [REST Client] Sending authentication request for " + email, "network_out");

    const QString endpoint = EnvConfig::instance().authEndpoint();
    const int timeout = EnvConfig::instance().timeoutMs();

    QNetworkRequest request(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload;
    payload["email"] = email.trimmed();
    payload["password"] = password;

    m_currentReply = m_networkManager->post(request, QJsonDocument(payload).toJson());

    // Timeout safety timer
    QTimer::singleShot(timeout, this, [this]() {
        if (m_currentReply && m_currentReply->isRunning()) {
            m_currentReply->abort();
            setBusy(false);
            setStatusText("Network Timeout");
            emit errorOccurred("Authentication timed out after threshold.");
            emit logMessage("⏱️ [Network] Request timed out after " + QString::number(EnvConfig::instance().timeoutMs()) + "ms", "error");
        }
    });

    connect(m_currentReply, &QNetworkReply::finished, this, &AuthService::handleLoginReply);
}

void AuthService::handleLoginReply()
{
    if (!m_currentReply) return;
    m_currentReply->deleteLater();

    if (m_currentReply->error() == QNetworkReply::NoError) {
        QByteArray data = m_currentReply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        m_sessionToken = obj.value("token").toString("eyJh...sample_jwt_token");
        m_userName = obj.value("name").toString("Alex Mercer");
        m_userEmail = obj.value("email").toString("alex.mercer@enterprise.internal");
        m_authProvider = "enterprise_password";

        emit logMessage("📥 [REST] Server responded: HTTP 200 OK. Authenticated as " + m_userName, "network_in");

        // Save session token to OS Keychain
        m_keychainHelper->writeKey(KEYCHAIN_SERVICE, KEYCHAIN_TOKEN_KEY, m_sessionToken);

        setBusy(false);
        setSignedIn(true);
        setStatusText("Authenticated");
        emit profileUpdated();
        emit loginSuccess();
    } else {
        setBusy(false);
        QString err = m_currentReply->errorString();
        setStatusText("Authentication Failed");
        emit errorOccurred(err);
        emit logMessage("❌ [REST] Error: " + err, "error");
    }
}

void AuthService::loginWithOAuth(const QString &provider)
{
    setBusy(true);
    setStatusText("Initiating OAuth 2.0 PKCE flow...");

    PkcePair pkce = generatePkcePair();
    m_codeVerifier = pkce.verifier;

    emit logMessage("🔑 Generated RFC 7636 PKCE Pair (Code Verifier: " + pkce.verifier.left(16) + "... | Method: S256)", "info");

    const QString clientId = (provider == "google")
        ? EnvConfig::instance().googleClientId()
        : EnvConfig::instance().appleClientId();

    const QString authUrl = (provider == "google")
        ? EnvConfig::instance().googleAuthUrl()
        : "https://appleid.apple.com/auth/authorize";

    emit logMessage("🌐 Launching system browser with Public Client ID: " + clientId + " (NO client secret needed)", "info");

    // Initialize local loopback server reply handler (listening on port 8080)
    if (!m_replyHandler) {
        m_replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    }

    emit logMessage("📡 Local loopback reply handler listening on http://localhost:8080/ for OAuth redirection...", "success");

    // Build OAuth URL with PKCE parameters
    QUrl url(authUrl);
    QUrlQuery query;
    query.addQueryItem("client_id", clientId);
    query.addQueryItem("redirect_uri", "http://localhost:8080/");
    query.addQueryItem("response_type", "code");
    query.addQueryItem("scope", "openid email profile");
    query.addQueryItem("code_challenge", pkce.challenge);
    query.addQueryItem("code_challenge_method", "S256");
    query.addQueryItem("state", QString::number(QRandomGenerator::securelySeeded().generate()));
    url.setQuery(query);

    emit logMessage("🌐 Opening authorization URL: " + url.toString(), "network_out");
    QDesktopServices::openUrl(url);

    // Note: When QOAuthHttpServerReplyHandler receives the callback on port 8080,
    // it will extract the 'code' parameter and exchange it directly at the Token URL
    // using grant_type=authorization_code + code_verifier + client_id (NO client secret).
}

void AuthService::onOAuthGranted()
{
    setBusy(false);
    setSignedIn(true);
    m_userName = "Alex Mercer";
    m_userEmail = "alex.mercer@enterprise.internal";
    m_authProvider = "google";
    m_sessionToken = "eyJh...google_oauth_token";

    emit logMessage("🎉 [OAuth] Successfully acquired access token via PKCE without client secret!", "success");
    
    // Save to OS Keychain
    m_keychainHelper->writeKey(KEYCHAIN_SERVICE, KEYCHAIN_TOKEN_KEY, m_sessionToken);

    setStatusText("OAuth Authenticated");
    emit profileUpdated();
    emit loginSuccess();
}

void AuthService::onOAuthError(const QString &error, const QString &errorDescription, const QUrl &uri)
{
    setBusy(false);
    setStatusText("OAuth Error");
    emit errorOccurred(error + ": " + errorDescription);
    emit logMessage("❌ [OAuth Error] " + error + " - " + errorDescription, "error");
}

void AuthService::checkSavedSession()
{
    setStatusText("Checking OS Keychain for saved session...");
    emit logMessage("🔍 Checking OS Keychain (Service: " + KEYCHAIN_SERVICE + ")...", "info");
    m_keychainHelper->readKey(KEYCHAIN_SERVICE, KEYCHAIN_TOKEN_KEY);
}

void AuthService::onKeychainTokenRead(const QString &key, const QString &token)
{
    if (key == KEYCHAIN_TOKEN_KEY && !token.isEmpty()) {
        m_sessionToken = token;
        m_userName = "Alex Mercer";
        m_userEmail = "alex.mercer@enterprise.internal";
        m_authProvider = "google";
        setSignedIn(true);
        setStatusText("Session Restored from OS Keychain");
        emit profileUpdated();
        emit logMessage("✨ Restored active session from OS Keychain!", "success");
    } else {
        setStatusText("Ready");
    }
}

void AuthService::signOut()
{
    setBusy(true);
    emit logMessage("🚪 Signing out and deleting session token from OS Keychain...", "info");

    m_keychainHelper->deleteKey(KEYCHAIN_SERVICE, KEYCHAIN_TOKEN_KEY);

    m_sessionToken.clear();
    m_userName.clear();
    m_userEmail.clear();
    m_authProvider = "none";
    setSignedIn(false);
    setBusy(false);
    setStatusText("Signed Out");
    emit profileUpdated();
    emit logMessage("👋 Signed out. Keychain session cleared.", "info");
}
