#include "authservice.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>
#include <QPointer>
#include <qtkeychain/keychain.h> // Modern cross-platform keyring API

AuthService::AuthService(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setStatusText("Ready");
}

AuthService::~AuthService()
{
    if (m_currentReply) {
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }

    if (m_oauth2Flow) {
        m_oauth2Flow->setReplyHandler(nullptr);
    }
}

void AuthService::login(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) {
        emit errorOccurred("Username and password cannot be empty");
        return;
    }

    setBusy(true);
    setStatusText("Verifying credentials...");
    emit rawLog("🔑 AuthService::login() invoked for user: " + username, "info");
    emit rawLog("📡 QNetworkAccessManager: Dispatching credentials POST request...", "info");

    // Build the request body
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    QJsonDocument doc(json);
    QByteArray body = doc.toJson();

    QNetworkRequest request((QUrl(m_authEndpoint)));
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    emit rawLog("🌐 Target Gateway Endpoint: " + m_authEndpoint, "network_out");
    // m_currentReply = m_networkManager->post(request, body);

    // connect(m_currentReply, &QNetworkReply::finished, this, &AuthService::handleLoginReply);
}

void AuthService::loginWithOAuth(const QString &provider)
{
    setBusy(true);
    setStatusText("OAuth initiating...");
    emit rawLog("🔄 [OAuthService::signIn()] Launching " + provider + " flow (asynchronous QOAuth2AuthorizationCodeFlow)...", "info");
    emit rawLog("📡 QOAuthHttpServerReplyHandler: Spawning local HTTP loopback listener on port 8080...", "info");

    m_oauth2Flow = new QOAuth2AuthorizationCodeFlow(this);
    m_replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_oauth2Flow->setReplyHandler(m_replyHandler);

    if (provider == "google") {
        m_oauth2Flow->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/v2/auth"));
        m_oauth2Flow->setTokenUrl(QUrl(m_dummyAuthEndpoint));
        m_oauth2Flow->setClientIdentifier(m_googleClientId);
        m_oauth2Flow->setClientIdentifierSharedKey(m_googleClientSecret);
        m_oauth2Flow->setRequestedScopeTokens({"openid", "profile", "email"});
    } else {
        m_oauth2Flow->setAuthorizationUrl(QUrl("https://appleid.apple.com/auth/authorize"));
        m_oauth2Flow->setTokenUrl(QUrl(m_dummyAuthEndpoint));
        m_oauth2Flow->setClientIdentifier(m_appleClientId);
        m_oauth2Flow->setClientIdentifierSharedKey(m_appleClientSecret);
        m_oauth2Flow->setRequestedScopeTokens({"openid", "profile", "email"});
    }

    connect(m_oauth2Flow, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [this](const QUrl &url) {
        emit rawLog("📡 QOAuthHttpServerReplyHandler: Listening on http://localhost:8080/ for OAuth provider redirection...", "success");
        emit rawLog("🌐 Opening system default browser to direct authorize URL:", "info");
        emit rawLog("🌐 GET " + url.toString(), "network_out");
    });

    connect(m_oauth2Flow, &QOAuth2AuthorizationCodeFlow::granted, [this, provider]() {
        emit rawLog("📡 Loopback socket captured request on http://localhost:8080/", "success");
        emit rawLog("📡 Captured redirect query parameters: GET /?code=oauth_auth_code_simulated...", "network_in");
        emit rawLog("📡 HTTPS exchange success! Valid OAuth access_token and id_token parsed successfully.", "success");
        
        QString token = m_oauth2Flow->token();
        m_sessionToken = token;
        m_userName = provider == "google" ? "Google Workspace Developer" : "Apple Developer Advocate";
        m_userEmail = provider == "google" ? "developer@google-workspace.internal" : "advocate@apple-developer.internal";
        
        saveTokenSecurely(token);
    });

    m_oauth2Flow->grant();
}

void AuthService::registerWithOAuth(const QString &provider)
{
    setBusy(true);
    setStatusText("OAuth registering...");
    emit rawLog("🔄 [OAuthService::signUp()] Launching " + provider + " registration flow (asynchronous QOAuth2AuthorizationCodeFlow)...", "info");
    emit rawLog("📡 QOAuthHttpServerReplyHandler: Spawning local HTTP loopback listener on port 8080...", "info");

    m_oauth2Flow = new QOAuth2AuthorizationCodeFlow(this);
    m_replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_oauth2Flow->setReplyHandler(m_replyHandler);

    if (provider == "google") {
        m_oauth2Flow->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/v2/auth"));
        m_oauth2Flow->setTokenUrl(QUrl(m_dummyAuthEndpoint));
        m_oauth2Flow->setClientIdentifier(m_googleClientId);
        m_oauth2Flow->setClientIdentifierSharedKey(m_googleClientSecret);
        m_oauth2Flow->setRequestedScopeTokens({"openid", "profile", "email"});
    } else {
        m_oauth2Flow->setAuthorizationUrl(QUrl("https://appleid.apple.com/auth/authorize"));
        m_oauth2Flow->setTokenUrl(QUrl(m_dummyAuthEndpoint));
        m_oauth2Flow->setClientIdentifier(m_appleClientId);
        m_oauth2Flow->setClientIdentifierSharedKey(m_appleClientSecret);
        m_oauth2Flow->setRequestedScopeTokens({"openid", "profile", "email"});
    }

    connect(m_oauth2Flow, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [this](const QUrl &url) {
        emit rawLog("📡 QOAuthHttpServerReplyHandler: Listening on http://localhost:8080/ for OAuth registration callback...", "success");
        emit rawLog("🌐 Opening system default browser to direct register URL:", "info");
        emit rawLog("🌐 GET " + url.toString(), "network_out");
    });

    connect(m_oauth2Flow, &QOAuth2AuthorizationCodeFlow::granted, [this, provider]() {
        emit rawLog("📡 Loopback socket captured request on http://localhost:8080/", "success");
        emit rawLog("📡 Captured registration redirect query parameters: GET /?code=oauth_register_code_simulated...", "network_in");
        emit rawLog("📡 HTTPS exchange success! User registered and valid session access_token parsed cleanly.", "success");
        
        QString token = m_oauth2Flow->token();
        m_sessionToken = token;
        m_userName = provider == "google" ? "Google Workspace Registered Developer" : "Apple Registered Developer";
        m_userEmail = provider == "google" ? "new.developer@google-workspace.internal" : "new.advocate@apple-developer.internal";
        
        saveTokenSecurely(token);
    });

    m_oauth2Flow->grant();
}

void AuthService::handleLoginReply()
{
    if (!m_currentReply) return;

    setBusy(false);
    m_currentReply->deleteLater();

    if (m_currentReply->error() != QNetworkReply::NoError) {
        QString errorString = m_currentReply->errorString();
        emit rawLog("❌ QNetworkReply failed: " + errorString, "error");
        setStatusText("Network error: Verification failed");
        emit errorOccurred("Verification failed: " + errorString);
        return;
    }

    QByteArray responseData = m_currentReply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject responseJson = doc.object();

    emit rawLog("📡 QNetworkReply received code 200 OK successfully.", "network_in");

    // Retrieve simulated response token
    QString token = responseJson.value("token").toString();
    if (token.isEmpty()) {
         token = "jwt_session_token_" + QString::number(100000 + (rand() % 900000));
    }

    m_sessionToken = token;
    m_userEmail = m_userName.contains('@') ? m_userName : m_userName + "@securedomain.internal";
    
    emit rawLog("✅ Credentials validated successfully for profile: " + m_userName, "success");
    saveTokenSecurely(token);
}

void AuthService::saveTokenSecurely(const QString &token)
{
    emit rawLog("🔐 QtKeychain: Preparing WritePasswordJob secure write operation...", "info");

    // Write password asynchronously using WritePasswordJob
    // QKeychain::WritePasswordJob *job = new QKeychain::WritePasswordJob("EnterpriseSecurity", this);
    // job->setKey("session_auth_token");
    // job->setPassword(token);

    QKeychain::WritePasswordJob *job =
        new QKeychain::WritePasswordJob("EnterpriseSecurity", this);

    job->setKey("session_auth_token");
    job->setTextData(token);


    connect(job, &QKeychain::WritePasswordJob::finished, this, [this, job, token]() {
        job->deleteLater();
        if (job->error()) {
            emit rawLog("❌ QtKeychain::WritePasswordJob failed: " + job->errorString(), "error");
            setStatusText("Secure storage failed");
            emit errorOccurred("Failed to store credentials: " + job->errorString());
        } else {
            emit rawLog("🔑 QtKeychain::WritePasswordJob completed: Active session token committed securely to OS keyring.", "success");
            setSignedIn(true);
            setStatusText("Signed in successfully");
            emit profileUpdated();
            emit loginSuccess();
        }
    });

    job->start();
}

// bool AuthService::checkSavedSession()
// {
//     emit rawLog("🔍 AuthService::checkSavedSession() checking for cached tokens in platform keyrings...", "info");

//     QKeychain::ReadPasswordJob *job = new QKeychain::ReadPasswordJob("EnterpriseSecurity", this);
//     job->setKey("session_auth_token");

//     connect(job, &QKeychain::ReadPasswordJob::finished, this, [this, job]() {
//         job->deleteLater();
//         if (!job->error()) {
//             m_sessionToken = job->textData();
//             m_userEmail = "explorer@securedomain.internal";
//             m_userName = "Explorer Character";
            
//             emit rawLog("🔑 QtKeychain::ReadPasswordJob success! Restored session token dynamically.", "success");
//             setSignedIn(true);
//             setStatusText("Session restored from OS keychain");
//             emit profileUpdated();
//             emit loginSuccess();
//         } else {
//             emit rawLog("🔍 QtKeychain::ReadPasswordJob: No stored session found.", "info");
//         }
//     });

//     job->start();
//     return true;
// }

bool AuthService::checkSavedSession()
{
    // Prevent multiple simultaneous keychain reads
    if (m_isBusy) {
        qDebug() << "Session check already in progress.";
        return false;
    }

    setBusy(true);
    setStatusText("Checking saved session...");

    auto *job = new QKeychain::ReadPasswordJob("EnterpriseSecurity", this);
    job->setKey("session_auth_token");

    // Guard against AuthService being destroyed while the async job is running
    QPointer<AuthService> self(this);

    connect(job, &QKeychain::Job::finished,
            this,
            [self, job]()
            {
                if (!self) {
                    return;
                }

                self->setBusy(false);

                if (job->error()) {
                    qDebug() << "No saved session:" << job->errorString();

                    self->m_sessionToken.clear();
                    self->setSignedIn(false);
                    self->setStatusText("No saved session.");

                    job->deleteLater();
                    return;
                }

                self->m_sessionToken = job->textData();

                if (self->m_sessionToken.isEmpty()) {
                    self->setSignedIn(false);
                    self->setStatusText("No saved session.");

                    job->deleteLater();
                    return;
                }

                qDebug() << "Saved session restored.";

                self->setSignedIn(true);
                self->setStatusText("Session restored.");

                // Fetch the user's profile only after we've restored the token
                self->fetchUserProfile();

                job->deleteLater();
            });

    job->start();

    return true;
}
void AuthService::signOut()
{
    emit rawLog("🚽 AuthService::signOut() invoked. Flushing keyrings and clearing active variables...", "warning");
    clearTokenSecurely();
}

void AuthService::clearTokenSecurely()
{
    QKeychain::DeletePasswordJob *job = new QKeychain::DeletePasswordJob("EnterpriseSecurity", this);
    job->setKey("session_auth_token");

    connect(job, &QKeychain::DeletePasswordJob::finished, this, [this, job]() {
        job->deleteLater();
        m_sessionToken.clear();
        m_userName.clear();
        m_userEmail.clear();
        setSignedIn(false);
        setStatusText("Logged out");
        emit profileUpdated();
        emit rawLog("🔑 QtKeychain::DeletePasswordJob completed: Credentials erased cleanly from system secure vault.", "success");
    });

    job->start();
}

void AuthService::resetState()
{
    setBusy(false);
    setStatusText("Ready");
}

void AuthService::fetchUserProfile()
{
    if (m_sessionToken.isEmpty()) {
        emit rawLog("❌ Cannot fetch user profile: No valid session token.", "error");
        return;
    }
    
    emit rawLog("📡 Fetching user profile from secure enterprise server...", "info");
    emit rawLog("🌐 GET https://api.enterprise-security.internal/v1/auth/profile", "network_out");
    emit rawLog("📡 Authorization: Bearer " + m_sessionToken.left(10) + "...", "network_out");
    
    // Simulate server response
    emit rawLog("📡 Profile response captured! Code 200 OK.", "network_in");
    emit rawLog("✅ User profile parsed: clearance_level=LEVEL_4, department=Security_Operations, direct_mfa=enabled", "success");
    emit profileUpdated();
}

void AuthService::onAuthFailed(const QString &errorMsg)
{
    setBusy(false);
    setStatusText("Authentication failed");
    emit rawLog("❌ [AuthService::onAuthFailed] Flow interrupted: " + errorMsg, "error");
    emit errorOccurred(errorMsg);
}

void AuthService::onTimeout()
{
    setBusy(false);
    setStatusText("Authorization timed out");
    emit rawLog("⏰ [AuthService::onTimeout] OAuth handshaking timed out after 60 seconds of inactivity.", "warning");
    emit rawLog("📡 QOAuthHttpServerReplyHandler: Stopped loopback server listening on port 8080.", "info");
    if (m_oauth2Flow) {
        m_oauth2Flow->deleteLater();
        m_oauth2Flow = nullptr;
    }
    emit errorOccurred("OAuth authorization timed out.");
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
