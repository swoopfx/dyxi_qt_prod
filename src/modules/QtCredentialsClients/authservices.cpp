#include "authservices.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QStringLiteral>
#include <QDebug>

#include <qtkeychain/keychain.h>

namespace OAuth {


AuthServices::AuthServices(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setStatusText("Ready");
}

// AuthServices::~AuthServices()
// {
//     if (m_currentReply) {
//         m_currentReply->abort();
//         m_currentReply->deleteLater();
//         m_currentReply = nullptr;
//     }

//     if (m_oauth2Flow) {
//         m_oauth2Flow->deleteLater();
//         m_oauth2Flow = nullptr;
//     }

//     if (m_replyHandler) {
//         m_replyHandler->deleteLater();
//         m_replyHandler = nullptr;
//     }
// }

AuthServices::~AuthServices()
{
    // Abort any active backend/network request.
    if (m_currentReply) {
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }

    // Stop and destroy the Android OAuth callback handler.
    if (m_replyHandler) {
        m_replyHandler->close();
        m_replyHandler->deleteLater();
        m_replyHandler = nullptr;
    }

    // Clear OAuth transaction state.
    m_googleState.clear();
    m_googleCodeVerifier.clear();
    m_googleNonce.clear();

    // Clear the application session token.
    m_sessionToken.clear();
}


QString AuthServices::statusText() const
{
    return m_statusText;
}

bool AuthServices::isBusy() const
{
    return m_isBusy;
}

bool AuthServices::isSignedIn() const
{
    return m_isSignedIn;
}

QString AuthServices::userName() const
{
    return m_userName;
}

QString AuthServices::userEmail() const
{
    return m_userEmail;
}

QString AuthServices::userId() const
{
    return m_userId;
}

void AuthServices::login(
    const QString &username,
    const QString &password)
{
    if (m_isBusy)
        return;

    if (username.trimmed().isEmpty()) {
        emit errorOccurred(
            "Username or email is required.");
        return;
    }

    if (password.isEmpty()) {
        emit errorOccurred(
            "Password is required.");
        return;
    }

    if (password.length() < 6) {
        emit errorOccurred(
            "Password must contain at least 6 characters.");
        return;
    }

    loginWithPasswordBackend(
        username.trimmed(),
        password);
}

void AuthServices::loginWithPasswordBackend(
    const QString &username,
    const QString &password)
{
    setBusy(true);
    setStatusText("Signing in...");

    QUrl url(
        QString(kAuthBaseUrl) +
        QString(kLoginEndpoint));

    QNetworkRequest request(url);

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    request.setRawHeader(
        "Accept",
        "application/json");

    QJsonObject payload;

    payload["username"] = username;
    payload["password"] = password;

    const QByteArray body =
        QJsonDocument(payload).toJson(
            QJsonDocument::Compact);

    QNetworkReply *reply =
        m_networkManager->post(request, body);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]() {
            handleLoginReply(reply);
        });
}

void AuthServices::handleLoginReply(
    QNetworkReply *reply)
{
    if (!reply)
        return;

    const QByteArray data =
        reply->readAll();

    const bool networkError =
        reply->error() != QNetworkReply::NoError;

    const QString errorString =
        reply->errorString();

    reply->deleteLater();

    setBusy(false);

    if (networkError) {
        emit rawLog(
            "Login network error: " +
                errorString,
            "error");

        setStatusText("Login failed");

        emit errorOccurred(
            "Unable to contact authentication server: " +
            errorString);

        return;
    }

    TokenResponse response;

    if (!parseTokenResponse(
            data,
            response)) {

        setStatusText(
            "Invalid authentication response");

        emit errorOccurred(
            "Authentication server returned an invalid response.");

        return;
    }

    applyTokenResponse(response);
}

bool AuthServices::parseTokenResponse(
    const QByteArray &data,
    TokenResponse &result)
{
    QJsonParseError parseError;

    const QJsonDocument document =
        QJsonDocument::fromJson(
            data,
            &parseError);

    if (parseError.error !=
        QJsonParseError::NoError) {

        emit rawLog(
            "Invalid JSON from auth server: " +
                parseError.errorString(),
            "error");

        return false;
    }

    if (!document.isObject())
        return false;

    const QJsonObject root =
        document.object();

    result.accessToken =
        root.value("access_token")
            .toString();

    result.refreshToken =
        root.value("refresh_token")
            .toString();

    result.expiresIn =
        root.value("expires_in")
            .toInt();

    const QJsonObject user =
        root.value("user")
            .toObject();

    result.userId =
        user.value("id").toString();

    result.username =
        user.value("username").toString();

    result.email =
        user.value("email").toString();

    result.displayName =
        user.value("display_name").toString();

    if (result.accessToken.isEmpty())
        return false;

    return true;
}

void AuthServices::applyTokenResponse(
    const TokenResponse &response)
{
    if (!response.isValid()) {
        emit errorOccurred(
            "Authentication server did not return an access token.");

        return;
    }

    m_sessionToken =
        response.accessToken;

    if (!response.refreshToken.isEmpty()) {
        m_refreshToken =
            response.refreshToken;
    }

    m_tokenExpiresIn =
        response.expiresIn;

    m_userId =
        response.userId;

    m_userName =
        response.username;

    m_userEmail =
        response.email;

    m_displayName =
        response.displayName;

    saveTokenSecurely(
        m_sessionToken);

    if (!m_refreshToken.isEmpty()) {
        saveRefreshTokenSecurely(
            m_refreshToken);
    }

    setSignedIn(true);
    setBusy(false);

    setStatusText(
        "Signed in successfully");

    emit profileUpdated();
    emit loginSuccess();
}


void AuthServices::loginWithGoogle()
{
    if (m_isBusy)
        return;

    setBusy(true);
    setStatusText(QStringLiteral("Connecting to Google..."));

    cleanupGoogleOAuth();

    m_googleState = generateOAuthState();
    m_googleCodeVerifier = generatePkceVerifier();
    m_googleNonce = generateOAuthState();

    const QString codeChallenge =
        generatePkceChallenge(m_googleCodeVerifier);

    // GOOGLE_REDIRECT_URI must be the Android URI/App Link registered in
    // Google Cloud Console and in the Android manifest.
    const QUrl redirectUri(QString::fromUtf8(GOOGLE_REDIRECT_URI));
    const QUrl authUrl(QString::fromUtf8(GOOGLE_AUTH_URL));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("client_id"),
                       QString::fromUtf8(GOOGLE_WEB_CLIENT_ID));
    query.addQueryItem(QStringLiteral("redirect_uri"),
                       redirectUri.toString(QUrl::FullyEncoded));
    query.addQueryItem(QStringLiteral("response_type"),
                       QStringLiteral("code"));
    query.addQueryItem(QStringLiteral("scope"),
                       QStringLiteral("openid profile email"));
    query.addQueryItem(QStringLiteral("state"), m_googleState);
    query.addQueryItem(QStringLiteral("code_challenge"), codeChallenge);
    query.addQueryItem(QStringLiteral("code_challenge_method"),
                       QStringLiteral("S256"));
    query.addQueryItem(QStringLiteral("nonce"), m_googleNonce);

    QUrl authorizationUrl = authUrl;
    authorizationUrl.setQuery(query);

    // The URI handler receives only Google's redirect. Qt does not exchange
    // the authorization code; the backend does that in Design A.
    if (!m_replyHandler)
        m_replyHandler = new QOAuthUriSchemeReplyHandler(redirectUri, this);

    disconnect(m_replyHandler, nullptr, this, nullptr);
    connect(m_replyHandler,
            &QAbstractOAuthReplyHandler::callbackReceived,
            this,
            [this](const QVariantMap &data) {
                handleGoogleOAuthCallback(data);
            });

    if (!m_replyHandler->listen()) {
        setBusy(false);
        setStatusText(QStringLiteral("Unable to start Google authentication."));
        emit errorOccurred(QStringLiteral("Could not register the Google callback handler."));
        cleanupGoogleOAuth();
        return;
    }

    emit rawLog(QStringLiteral("Opening Google authentication..."),
                QStringLiteral("info"));

    // Do not log the authorization code, verifier, state, nonce, or complete
    // URL in production.
    if (!QDesktopServices::openUrl(authorizationUrl)) {
        setBusy(false);
        setStatusText(QStringLiteral("Unable to open Google authentication."));
        emit errorOccurred(QStringLiteral("Could not open the system browser."));
        cleanupGoogleOAuth();
    }
}

void AuthServices::sendGoogleAuthorizationCode(const QString &code,
                                               const QString &redirectUri,
                                               const QString &codeVerifier,
                                               const QString &nonce)
{
    const QUrl url(QString(kAuthBaseUrl) + QString(kGoogleEndpoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/json"));
    request.setRawHeader("Accept", "application/json");

    QJsonObject payload;

     setStatusText("RedirectUrl"+ redirectUri);
    qDebug() << "RedirectUrl "+ redirectUri;
     qDebug() << "code "+ code;
    qDebug() << "Code Verifier" + codeVerifier;
         qDebug() << "nonce " + nonce;

    payload.insert(QStringLiteral("code"), code);
    payload.insert(QStringLiteral("redirect_uri"), redirectUri);
    payload.insert(QStringLiteral("code_verifier"), codeVerifier);
    payload.insert(QStringLiteral("nonce"), nonce);
    payload.insert(QStringLiteral("client"), "mobile");

    QNetworkReply *reply = m_networkManager->post(
        request, QJsonDocument(payload).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray body = reply->readAll();
        const bool failed = reply->error() != QNetworkReply::NoError;
        const QString errorText = reply->errorString();
        reply->deleteLater();

        cleanupGoogleOAuth();
        setBusy(false);

        if (failed) {
            setStatusText(QStringLiteral("Google login failed"));
            emit errorOccurred(QStringLiteral("Authentication server request failed: %1")
                                   .arg(errorText));
            return;
        }

        TokenResponse response;
        if (!parseTokenResponse(body, response)) {
            setStatusText(QStringLiteral("Invalid authentication response"));
            emit errorOccurred(QStringLiteral("Authentication server returned an invalid Google login response."));
            return;
        }

        applyTokenResponse(response);
    });
}



// void AuthServices::handleGoogleOAuthCallback(
//     const QVariantMap &data)
// {
//     emit rawLog(
//         QStringLiteral(
//             "Google OAuth callback received."),
//         QStringLiteral("network_in"));

//     /*
//      * -------------------------------------------------------
//      * Check Google OAuth errors
//      * -------------------------------------------------------
//      */

//     const QString error =
//         data.value(
//                 QStringLiteral("error"))
//             .toString();

//     if (!error.isEmpty()) {

//         const QString description =
//             data.value(
//                     QStringLiteral("error_description"))
//                 .toString();

//         setBusy(false);

//         setStatusText(
//             QStringLiteral(
//                 "Google authentication failed"));

//         QString message =
//             QStringLiteral(
//                 "Google OAuth error: ") +
//             error;

//         if (!description.isEmpty()) {
//             message +=
//                 QStringLiteral(
//                     " - ") +
//                 description;
//         }

//         emit errorOccurred(message);

//         cleanupGoogleOAuth();

//         return;
//     }

//     /*
//      * -------------------------------------------------------
//      * Get authorization code
//      * -------------------------------------------------------
//      */

//     const QString code =
//         data.value(
//                 QStringLiteral("code"))
//             .toString();

//     if (code.isEmpty()) {

//         setBusy(false);

//         setStatusText(
//             QStringLiteral(
//                 "Google authentication failed"));

//         emit errorOccurred(
//             QStringLiteral(
//                 "Google did not return an authorization code."));

//         cleanupGoogleOAuth();

//         return;
//     }

//     /*
//      * -------------------------------------------------------
//      * Validate state
//      * -------------------------------------------------------
//      */

//     const QString receivedState =
//         data.value(
//                 QStringLiteral("state"))
//             .toString();

//     if (receivedState.isEmpty()) {

//         setBusy(false);

//         setStatusText(
//             QStringLiteral(
//                 "Google authentication failed"));

//         emit errorOccurred(
//             QStringLiteral(
//                 "Google OAuth response did not contain state."));

//         cleanupGoogleOAuth();

//         return;
//     }

//     if (receivedState != m_googleState) {

//         setBusy(false);

//         setStatusText(
//             QStringLiteral(
//                 "Google authentication failed"));

//         emit errorOccurred(
//             QStringLiteral(
//                 "OAuth state validation failed."));

//         cleanupGoogleOAuth();

//         return;
//     }

//     /*
//      * -------------------------------------------------------
//      * PKCE verifier must still exist.
//      * -------------------------------------------------------
//      */

//     if (m_googleCodeVerifier.isEmpty()) {

//         setBusy(false);

//         setStatusText(
//             QStringLiteral(
//                 "Google authentication failed"));

//         emit errorOccurred(
//             QStringLiteral(
//                 "PKCE verifier is missing."));

//         cleanupGoogleOAuth();

//         return;
//     }

//     /*
//      * -------------------------------------------------------
//      * Send authorization code to backend.
//      * -------------------------------------------------------
//      *
//      * IMPORTANT:
//      *
//      * We send the PKCE verifier as well.
//      *
//      * Your backend needs it when it exchanges the
//      * authorization code with Google.
//      */

//     setStatusText(
//         QStringLiteral(
//             "Completing Google authentication..."));

//     sendGoogleAuthorizationCode(
//         code,
//         QString::fromUtf8(
//             GOOGLE_REDIRECT_URI),
//         m_googleCodeVerifier);

//     /*
//      * Do NOT log:
//      *
//      *     code
//      *     m_googleCodeVerifier
//      *
//      * Both are credentials during this stage.
//      */
// }


void AuthServices::handleGoogleOAuthCallback(const QVariantMap &data)
{
    const QString error = data.value(QStringLiteral("error")).toString();
    if (!error.isEmpty()) {
        const QString description =
            data.value(QStringLiteral("error_description")).toString();
        setBusy(false);
        setStatusText(QStringLiteral("Google authentication failed"));
        emit errorOccurred(QStringLiteral("Google OAuth error: %1%2")
                               .arg(error)
                               .arg(description.isEmpty()
                                        ? QString()
                                        : QStringLiteral(" - ") + description));
        cleanupGoogleOAuth();
        return;
    }

    const QString code = data.value(QStringLiteral("code")).toString();
    const QString receivedState = data.value(QStringLiteral("state")).toString();

    if (code.isEmpty()) {
        setBusy(false);
        setStatusText(QStringLiteral("Google authentication failed"));
        emit errorOccurred(QStringLiteral("Google did not return an authorization code."));
        cleanupGoogleOAuth();
        return;
    }

    if (receivedState.isEmpty() || receivedState != m_googleState) {
        setBusy(false);
        setStatusText(QStringLiteral("Google authentication failed"));
        emit errorOccurred(QStringLiteral("OAuth state validation failed."));
        cleanupGoogleOAuth();
        return;
    }

    if (m_googleCodeVerifier.isEmpty() || m_googleNonce.isEmpty()) {
        setBusy(false);
        setStatusText(QStringLiteral("Google authentication failed"));
        emit errorOccurred(QStringLiteral("OAuth transaction data is missing."));
        cleanupGoogleOAuth();
        return;
    }

    const QString verifier = m_googleCodeVerifier;
    const QString nonce = m_googleNonce;
    const QUrl redirectUri(QString::fromUtf8(GOOGLE_REDIRECT_URI));

    setStatusText(QStringLiteral("Verifying Google account..."));
    sendGoogleAuthorizationCode(code, redirectUri.toString(), verifier, nonce);
}


void AuthServices::handleGoogleReply(
    QNetworkReply *reply)
{
    if (!reply)
        return;

    const QByteArray data =
        reply->readAll();

    const bool networkError =
        reply->error() !=
        QNetworkReply::NoError;

    const QString errorString =
        reply->errorString();

    reply->deleteLater();

    setBusy(false);

    if (networkError) {

        emit rawLog(
            "Google authentication failed: " +
                errorString,
            "error");

        setStatusText(
            "Google login failed");

        emit errorOccurred(
            "Google authentication failed: " +
            errorString);

        return;
    }

    TokenResponse response;

    if (!parseTokenResponse(
            data,
            response)) {

        setStatusText(
            "Invalid authentication response");

        emit errorOccurred(
            "Authentication server returned an invalid Google login response.");

        return;
    }

    applyTokenResponse(response);
}


void AuthServices::cleanupGoogleOAuth()
{
    if (m_replyHandler) {

        m_replyHandler->close();

        m_replyHandler->deleteLater();

        m_replyHandler = nullptr;
    }

    /*
     * Never keep these values around after the
     * authorization transaction is finished.
     */
    m_googleState.clear();
    m_googleCodeVerifier.clear();
    m_googleNonce.clear();
}




void AuthServices::saveTokenSecurely(
    const QString &token)
{
    if (token.isEmpty())
        return;

    auto *job =
        new QKeychain::WritePasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            kAccessTokenKey));

    job->setTextData(token);

    connect(
        job,
        &QKeychain::WritePasswordJob::finished,
        this,
        [this, job]() {

            if (job->error()) {

                emit rawLog(
                    "Failed to store access token: " +
                        job->errorString(),
                    "error");

                emit errorOccurred(
                    "Secure token storage failed.");

            } else {

                emit rawLog(
                    "Application access token stored securely.",
                    "success");
            }

            job->deleteLater();
        });

    job->start();
}

void AuthServices::saveRefreshTokenSecurely(
    const QString &token)
{
    if (token.isEmpty())
        return;

    auto *job =
        new QKeychain::WritePasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            kRefreshTokenKey));

    job->setTextData(token);

    connect(
        job,
        &QKeychain::WritePasswordJob::finished,
        this,
        [this, job]() {

            if (job->error()) {

                emit rawLog(
                    "Failed to store refresh token: " +
                        job->errorString(),
                    "error");

            } else {

                emit rawLog(
                    "Refresh token stored securely.",
                    "success");
            }

            job->deleteLater();
        });

    job->start();
}

bool AuthServices::checkSavedSession()
{
    if (m_restoringSession)
        return false;

    m_restoringSession = true;

    emit rawLog(
        "Checking stored authentication session...",
        "info");

    auto *job =
        new QKeychain::ReadPasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            kAccessTokenKey));

    connect(
        job,
        &QKeychain::ReadPasswordJob::finished,
        this,
        [this, job]() {

            const bool failed =
                job->error();

            const QString token =
                job->textData();

            if (!failed &&
                !token.isEmpty()) {

                m_sessionToken = token;

                emit rawLog(
                    "Stored access token recovered.",
                    "success");

                fetchUserProfile();

            } else {

                emit rawLog(
                    "No stored access token found.",
                    "info");

                readRefreshTokenSecurely();
            }

            m_restoringSession = false;

            job->deleteLater();
        });

    job->start();

    return true;
}

QString AuthServices::generatePkceVerifier()
{
    QByteArray randomBytes;

    /*
     * Generate 64 cryptographically secure random bytes.
     *
     * Base64url encoding gives us a verifier well within
     * RFC 7636's 43-128 character requirement.
     */
    for (int i = 0; i < 8; ++i) {
        const quint64 value =
            QRandomGenerator::system()->generate64();

        for (int j = 0; j < 8; ++j) {
            randomBytes.append(
                static_cast<char>(
                    (value >> (j * 8)) & 0xff));
        }
    }

    return QString::fromLatin1(
        randomBytes
            .toBase64(QByteArray::Base64UrlEncoding |
                      QByteArray::OmitTrailingEquals));
}

void AuthServices::readRefreshTokenSecurely()
{
    auto *job =
        new QKeychain::ReadPasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            kRefreshTokenKey));

    connect(
        job,
        &QKeychain::ReadPasswordJob::finished,
        this,
        [this, job]() {

            if (!job->error() &&
                !job->textData().isEmpty()) {

                m_refreshToken =
                    job->textData();

                emit rawLog(
                    "Refresh token recovered. Attempting session refresh.",
                    "info");

                refreshToken();

            } else {

                setSignedIn(false);
                setStatusText("Ready");

                emit rawLog(
                    "No stored authentication session.",
                    "info");
            }

            job->deleteLater();
        });

    job->start();
}

QString AuthServices::generatePkceChallenge(
    const QString &verifier)
{
    const QByteArray hash =
        QCryptographicHash::hash(
            verifier.toUtf8(),
            QCryptographicHash::Sha256);

    return QString::fromLatin1(
        hash.toBase64(
            QByteArray::Base64UrlEncoding |
            QByteArray::OmitTrailingEquals));
}

QNetworkRequest AuthServices::createRequest(
    const QUrl &url) const
{
    QNetworkRequest request(url);

    request.setRawHeader(
        "Accept",
        "application/json");

    return request;
}

QNetworkRequest
AuthServices::createAuthenticatedRequest(
    const QUrl &url) const
{
    QNetworkRequest request =
        createRequest(url);

    if (!m_sessionToken.isEmpty()) {

        request.setRawHeader(
            "Authorization",
            QByteArray("Bearer ") +
                m_sessionToken.toUtf8());
    }

    return request;
}

void AuthServices::fetchUserProfile()
{
    if (m_sessionToken.isEmpty()) {
        readRefreshTokenSecurely();
        return;
    }

    QUrl url(
        QString(kAuthBaseUrl) +
        QString(kMeEndpoint));

    QNetworkRequest request =
        createAuthenticatedRequest(url);

    QNetworkReply *reply =
        m_networkManager->get(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]() {
            handleProfileReply(reply);
        });
}

void AuthServices::handleProfileReply(
    QNetworkReply *reply)
{
    if (!reply)
        return;

    const int statusCode =
        reply->attribute(
                 QNetworkRequest::HttpStatusCodeAttribute)
            .toInt();

    const QByteArray data =
        reply->readAll();

    reply->deleteLater();

    if (statusCode == 401) {

        emit rawLog(
            "Access token expired or invalid. Refreshing...",
            "warning");

        refreshToken();
        return;
    }

    if (statusCode < 200 ||
        statusCode >= 300) {

        setSignedIn(false);

        emit errorOccurred(
            "Unable to validate saved session.");

        return;
    }

    QJsonParseError parseError;

    QJsonDocument doc =
        QJsonDocument::fromJson(
            data,
            &parseError);

    if (parseError.error !=
        QJsonParseError::NoError) {

        setSignedIn(false);

        emit errorOccurred(
            "Invalid profile response.");

        return;
    }

    const QJsonObject user =
        doc.object()
            .value("user")
            .toObject();

    m_userId =
        user.value("id").toString();

    m_userName =
        user.value("username").toString();

    m_userEmail =
        user.value("email").toString();

    m_displayName =
        user.value("display_name").toString();

    setSignedIn(true);

    setStatusText(
        "Session restored");

    emit profileUpdated();
    emit loginSuccess();
}

void AuthServices::refreshToken()
{
    if (m_refreshToken.isEmpty()) {
        readRefreshTokenSecurely();
        return;
    }

    setBusy(true);
    setStatusText(
        "Refreshing session...");

    QUrl url(
        QString(kAuthBaseUrl) +
        QString(kRefreshEndpoint));

    QNetworkRequest request(url);

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    request.setRawHeader(
        "Accept",
        "application/json");

    QJsonObject payload;

    payload["refresh_token"] =
        m_refreshToken;

    const QByteArray body =
        QJsonDocument(payload)
            .toJson(QJsonDocument::Compact);

    QNetworkReply *reply =
        m_networkManager->post(
            request,
            body);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]() {
            handleRefreshReply(reply);
        });
}

void AuthServices::handleRefreshReply(
    QNetworkReply *reply)
{
    if (!reply)
        return;

    const QByteArray data =
        reply->readAll();

    const bool failed =
        reply->error() !=
        QNetworkReply::NoError;

    const QString errorString =
        reply->errorString();

    reply->deleteLater();

    setBusy(false);

    if (failed) {

        emit rawLog(
            "Refresh failed: " +
                errorString,
            "error");

        clearSessionState();

        emit errorOccurred(
            "Your session has expired. Please sign in again.");

        return;
    }

    TokenResponse response;

    if (!parseTokenResponse(
            data,
            response)) {

        clearSessionState();

        emit errorOccurred(
            "Invalid refresh response.");

        return;
    }

    applyTokenResponse(response);

    fetchUserProfile();
}

void AuthServices::signOut()
{
    const QString refreshToken =
        m_refreshToken;

    clearSessionState();

    if (refreshToken.isEmpty()) {
        emit logoutCompleted();
        return;
    }

    QUrl url(
        QString(kAuthBaseUrl) +
        QString(kLogoutEndpoint));

    QNetworkRequest request(url);

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QJsonObject payload;

    payload["refresh_token"] =
        refreshToken;

    QNetworkReply *reply =
        m_networkManager->post(
            request,
            QJsonDocument(payload)
                .toJson(QJsonDocument::Compact));

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply]() {

            reply->deleteLater();

            emit logoutCompleted();
        });
}

void AuthServices::clearSessionState()
{
    m_sessionToken.clear();
    m_refreshToken.clear();

    m_userId.clear();
    m_userName.clear();
    m_userEmail.clear();
    m_displayName.clear();

    setSignedIn(false);
    setStatusText("Logged out");

    clearTokenSecurely();
    clearRefreshTokenSecurely();

    emit profileUpdated();
}

void AuthServices::clearTokenSecurely()
{
    auto *job =
        new QKeychain::DeletePasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            kAccessTokenKey));

    connect(
        job,
        &QKeychain::DeletePasswordJob::finished,
        this,
        [job]() {
            job->deleteLater();
        });

    job->start();
}

void AuthServices::clearRefreshTokenSecurely()
{
    auto *job =
        new QKeychain::DeletePasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            kRefreshTokenKey));

    connect(
        job,
        &QKeychain::DeletePasswordJob::finished,
        this,
        [job]() {
            job->deleteLater();
        });

    job->start();
}


void AuthServices::setBusy(
    bool value)
{
    if (m_isBusy == value)
        return;

    m_isBusy = value;

    emit isBusyChanged();
}

void AuthServices::setStatusText(
    const QString &value)
{
    if (m_statusText == value)
        return;

    m_statusText = value;

    emit statusTextChanged();
}

void AuthServices::setSignedIn(
    bool value)
{
    if (m_isSignedIn == value)
        return;

    m_isSignedIn = value;

    emit authStateChanged();
}

QString AuthServices::generateOAuthState()
{
    QByteArray randomBytes;

    for (int i = 0; i < 4; ++i) {
        const quint64 value =
            QRandomGenerator::system()->generate64();

        for (int j = 0; j < 8; ++j) {
            randomBytes.append(
                static_cast<char>(
                    (value >> (j * 8)) & 0xff));
        }
    }

    return QString::fromLatin1(
        randomBytes
            .toBase64(QByteArray::Base64UrlEncoding |
                      QByteArray::OmitTrailingEquals));
}
}