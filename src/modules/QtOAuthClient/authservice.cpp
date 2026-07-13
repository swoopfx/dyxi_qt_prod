#include "authservice.h"
#include <QDesktopServices>
#include <QUrlQuery>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSslError>
#include <QMessageAuthenticationCode>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QAbstractOAuth2>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QNetworkReply>
#include <QTimer>

AuthService::AuthService(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_timeoutTimer(new QTimer(this))
    , m_oauth2Flow(new QOAuth2AuthorizationCodeFlow(this))
{
    m_timeoutTimer->setSingleShot(true);

    // 🛡️ Secure Platform-Independent Environment Configuration Mapping
    m_googleClientId = qEnvironmentVariable("VITE_GOOGLE_CLIENT_ID", "YOUR_GOOGLE_CLIENT_ID.apps.googleusercontent.com");
    m_googleClientSecret = qEnvironmentVariable("VITE_GOOGLE_CLIENT_SECRET", "GOCSPX-dummy_secret");
    m_appleClientId = qEnvironmentVariable("VITE_APPLE_CLIENT_ID", "com.yourdomain.apple-login-service");
    m_appleClientSecret = qEnvironmentVariable("VITE_APPLE_CLIENT_SECRET", "apple-dummy_secret");
    m_dummyAuthEndpoint = qEnvironmentVariable("VITE_OAUTH_AUTH_ENDPOINT", "https://oauth.yourdummygateway.com/exchange");

    bool portParsed = false;
    int envPort = qEnvironmentVariable("VITE_OAUTH_LOOPBACK_PORT").toInt(&portParsed);
    m_loopbackPort = (portParsed && envPort > 0) ? envPort : 8080;

    // Initialize QtOAuth Classes
    m_oauth2Flow->setNetworkAccessManager(m_networkManager);

    // Bind authenticated signals for automatic code collection and extraction
    connect(m_oauth2Flow, &QOAuth2AuthorizationCodeFlow::granted, this, &AuthService::onAuthenticated);

    connect(m_oauth2Flow, &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived, this, [this](const QVariantMap &parameters) {
        emit rawLog(QString::fromUtf8(QJsonDocument::fromVariant(parameters).toJson(QJsonDocument::Compact)), "network_in");
    });

    connect(m_timeoutTimer, &QTimer::timeout, this, &AuthService::onTimeout);
}

AuthService::~AuthService()
{
    // Stop timers first
    m_timeoutTimer->stop();

    // Cleanup OAuth flow and handler
    if (m_oauth2Flow) {
        m_oauth2Flow->setReplyHandler(nullptr);
    }

    if (m_replyHandler) {
        m_replyHandler->close();
        // Since it's a child of 'this', it will be deleted by QObject destructor.
        // We just ensure it's closed and disconnected.
        m_replyHandler = nullptr;
    }
}

bool AuthService::isBusy() const
{
    return m_state != AuthState::Idle && m_state != AuthState::SuccessSignedIn && m_state != AuthState::ErrorOccurred;
}

void AuthService::setStatus(AuthState state, const QString &text)
{
    if (m_state != state || m_statusText != text) {
        m_state = state;
        m_statusText = text;
        emit stateChanged(m_state);
        emit statusTextChanged();
        emit isBusyChanged();
        emit rawLog(QString("State updated: %1 - %2").arg(static_cast<int>(state)).arg(text), "info");
    }
}

void AuthService::raiseError(AuthError error, const QString &details)
{
    m_timeoutTimer->stop();
    if (m_replyHandler) {
        m_replyHandler->close();
    }
    setStatus(AuthState::ErrorOccurred, QString("Error: %1").arg(details));
    emit authErrorOccurred(error, details);
    emit rawLog(QString("CRITICAL ERROR [%1]: %2").arg(static_cast<int>(error)).arg(details), "error");
}

void AuthService::generateCsrfState()
{
    QByteArray randomData;
    randomData.resize(16);
    for (int i = 0; i < 16; ++i) {
        randomData[i] = static_cast<char>(QRandomGenerator::global()->bounded(33, 126));
    }
    m_csrfState = QString::fromLatin1(randomData.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

void AuthService::startGoogleLogin()
{
    emit rawLog("Initiating QOAuth2AuthorizationCodeFlow for Google...", "info");
    resetState();
    m_currentProvider = "google";

    setStatus(AuthState::GeneratingUrl, "Generating secure Google authorization URL...");
    generateCsrfState();

    // Initialize reply handler
    m_replyHandler = new QOAuthHttpServerReplyHandler(m_loopbackPort, this);
    m_oauth2Flow->setReplyHandler(m_replyHandler);

    emit rawLog(QString("📡 [QOAuthHttpServerReplyHandler] Spawning automatic listener on localhost:%1").arg(m_loopbackPort), "success");

    m_oauth2Flow->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/v2/auth"));
    m_oauth2Flow->setTokenUrl(QUrl(m_dummyAuthEndpoint));
    m_oauth2Flow->setClientIdentifier(m_googleClientId);
    m_oauth2Flow->setClientIdentifierSharedKey(m_googleClientSecret);
    m_oauth2Flow->setRequestedScopeTokens({"openid", "profile", "email"});

    m_oauth2Flow->setModifyParametersFunction([this](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
            parameters->insert("state", m_csrfState);
            parameters->insert("prompt", "select_account");
        }
    });

    m_timeoutTimer->start(120000);
    setStatus(AuthState::ListeningLoopback, "Awaiting Google verification callback...");
    m_oauth2Flow->grant();
}

void AuthService::startAppleLogin()
{
    emit rawLog("Initiating QOAuth2AuthorizationCodeFlow for Apple...", "info");
    resetState();
    m_currentProvider = "apple";

    setStatus(AuthState::GeneratingUrl, "Generating secure Apple authorization URL...");
    generateCsrfState();

    m_replyHandler = new QOAuthHttpServerReplyHandler(m_loopbackPort, this);
    m_oauth2Flow->setReplyHandler(m_replyHandler);

    emit rawLog(QString("📡 [QOAuthHttpServerReplyHandler] Spawning automatic listener on localhost:%1").arg(m_loopbackPort), "success");

    m_oauth2Flow->setAuthorizationUrl(QUrl("https://appleid.apple.com/auth/authorize"));
    m_oauth2Flow->setTokenUrl(QUrl(m_dummyAuthEndpoint));
    m_oauth2Flow->setClientIdentifier(m_appleClientId);
    m_oauth2Flow->setClientIdentifierSharedKey(m_appleClientSecret);
    m_oauth2Flow->setRequestedScopeTokens({"name", "email"});

    m_oauth2Flow->setModifyParametersFunction([this](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
            parameters->insert("state", m_csrfState);
            parameters->insert("response_mode", "form_post");
        }
    });

    m_timeoutTimer->start(120000);
    setStatus(AuthState::ListeningLoopback, "Awaiting Apple registration callback...");
    m_oauth2Flow->grant();
}

void AuthService::onAuthenticated()
{
    m_timeoutTimer->stop();
    emit rawLog("📡 [QOAuthHttpServerReplyHandler] Handshake completed successfully!", "success");

    QString token = m_oauth2Flow->token();
    setStatus(AuthState::CodeReceived, "Secure handshake established. Fetching profile...");
    fetchUserProfile(token);
}

void AuthService::onAuthFailed(const QString &error, const QString &errorDescription)
{
    raiseError(AuthError::TokenExchangeFailed, QString("%1: %2").arg(error).arg(errorDescription));
}

void AuthService::fetchUserProfile(const QString &token)
{
    setStatus(AuthState::ExchangingToken, "Querying user profile...");

    QUrl url(m_dummyAuthEndpoint + "?token=" + token);
    QNetworkRequest request(url);

    QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
    sslConf.setProtocol(QSsl::SecureProtocols);
    request.setSslConfiguration(sslConf);

    QNetworkReply *reply = m_networkManager->get(request);

    // Use the 4-argument connect to ensure safety if 'this' is destroyed
    connect(reply, &QNetworkReply::finished, this, [this, reply, token]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            raiseError(AuthError::TokenExchangeFailed, "Profile retrieval failed: " + reply->errorString());
            return;
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        m_userName = jsonObj.value("name").toString();
        m_userEmail = jsonObj.value("email").toString();
        m_userPicture = jsonObj.value("picture").toString();

        if (m_userName.isEmpty()) {
            m_userName = (m_currentProvider == "google" ? "Google Member" : "Apple Member");
        }
        if (m_userEmail.isEmpty()) {
            m_userEmail = "fellow.explorer@securedomain.internal";
        }

        setStatus(AuthState::SuccessSignedIn, "Dynamic connection successfully initialized!");
        emit profileUpdated();
        emit rawLog(QString("Signed in successfully as [%1] (%2)").arg(m_userName).arg(m_userEmail), "success");

        // 🔑 Secure Storage using QtKeychain
        // FIX: Do not set 'this' as parent if setAutoDelete(true) is used to avoid double deletion
        QKeychain::WritePasswordJob *job = new QKeychain::WritePasswordJob("EnterpriseSecurity");
        job->setAutoDelete(true);
        job->setKey("oauth_access_token");
        job->setTextData(token);

        connect(job, &QKeychain::WritePasswordJob::finished, this, [this](QKeychain::Job *j) {
            if (j->error()) {
                emit rawLog("🔑 [QtKeychain] Secure keyring write failed: " + j->errorString(), "error");
            } else {
                emit rawLog("🔑 [QtKeychain] Access token encrypted and stored successfully!", "success");
            }
        });

        job->start();
    });
}

void AuthService::onTimeout()
{
    if (m_state == AuthState::ListeningLoopback) {
        emit rawLog("Login sequence timed out.", "warning");
        raiseError(AuthError::UserCancelled, "Login sequence timed out.");
    }
}

void AuthService::cancelLogin()
{
    emit rawLog("Login cancellation triggered.", "warning");
    resetState();
}

void AuthService::resetState()
{
    m_timeoutTimer->stop();
    if (m_oauth2Flow) {
        m_oauth2Flow->setReplyHandler(nullptr);
    }
    if (m_replyHandler) {
        m_replyHandler->close();
        m_replyHandler->deleteLater();
        m_replyHandler = nullptr;
    }
    m_csrfState.clear();
    m_userName.clear();
    m_userEmail.clear();
    m_userPicture.clear();
    setStatus(AuthState::Idle, "Ready");
}

bool AuthService::checkSavedSession()
{
    emit rawLog("🔄 Checking for saved session...", "info");

    // FIX: Do not set 'this' as parent if setAutoDelete(true) is used
    QKeychain::ReadPasswordJob *job = new QKeychain::ReadPasswordJob("EnterpriseSecurity");
    job->setAutoDelete(true);
    job->setKey("oauth_access_token");

    connect(job, &QKeychain::ReadPasswordJob::finished, this, [this, job](QKeychain::Job *j) {
        if (j->error()) {
            emit rawLog("🔑 No active session located.", "info");
        } else {
            QString token = job->textData();
            emit rawLog("🔑 Secure token restored!", "success");

            m_userName = "Restored Explorer";
            m_userEmail = "keyring-restored@enterprise.system";
            m_userPicture = "default_avatar";

            setStatus(AuthState::SuccessSignedIn, "Authenticated via native keyring!");
            emit profileUpdated();
        }
    });

    job->start();
    return true;
}

void AuthService::signOut()
{
    emit rawLog("🔄 Signing out...", "info");

    // FIX: Do not set 'this' as parent if setAutoDelete(true) is used
    QKeychain::DeletePasswordJob *job = new QKeychain::DeletePasswordJob("EnterpriseSecurity");
    job->setAutoDelete(true);
    job->setKey("oauth_access_token");

    connect(job, &QKeychain::DeletePasswordJob::finished, this, [this](QKeychain::Job *j) {
        if (j->error()) {
            emit rawLog("🔑 Failed to delete tokens: " + j->errorString(), "warning");
        } else {
            emit rawLog("🔑 Tokens deleted successfully.", "success");
        }
        resetState();
    });

    job->start();
}
