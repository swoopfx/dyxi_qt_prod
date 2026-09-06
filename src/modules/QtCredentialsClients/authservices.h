#pragma once

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>
// #include <QOAuthHttpServerReplyHandler>
#include <QOAuthUriSchemeReplyHandler>
// #include <QOAuthUriSchemeReplyHandler>
#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>
#include <QSet>
#include <QJsonDocument>
#include <QJsonObject>

namespace  OAuth {
// Compile-time string constants using string_view
// constexpr std::string_view GOOGLE_AUTH_URL = "https://accounts.google.com/o/oauth2/v2/auth";
// constexpr std::string_view GOOGLE_CLIENT_ID = "952310262713-4b1f3os6uqmhorsjmp9tmritbph1m6sg.apps.googleusercontent.com";
// constexpr std::string_view GOOGLE_WEB_CLIENT_ID = "952310262713-3t4aiv26nl4pobvhs6mqiqmdl8vt4so9.apps.googleusercontent.com";
// // 952310262713-3t4aiv26nl4pobvhs6mqiqmdl8vt4so9.apps.googleusercontent.com

static constexpr auto GOOGLE_AUTH_URL =
    "https://accounts.google.com/o/oauth2/v2/auth";

static constexpr auto GOOGLE_WEB_CLIENT_ID = "952310262713-3t4aiv26nl4pobvhs6mqiqmdl8vt4so9.apps.googleusercontent.com";

static constexpr auto GOOGLE_REDIRECT_URI = "https://api.dyxi.site/oauth/android/callback";

static constexpr auto GOOGLE_LOGIN_API_URL ="https://api.dyxi.site/auth/ipa/google-oauth";


/*
 * This must be a real HTTPS App Link that:
 *
 * 1. Is registered with Google.
 * 2. Is associated with your Android application.
 * 3. Has an Android assetlinks.json file.
 */
// constexpr auto GOOGLE_REDIRECT_URI ="https://api.dyxi.site/auth/ipa/google-oauth";
// constexpr auto GOOGLE_REDIRECT_URI ="org.qtproject.oruladyxiprod:/ipa/google-callback";

class AuthServices final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString statusText
                   READ statusText
                       NOTIFY statusTextChanged)

    Q_PROPERTY(bool isBusy
                   READ isBusy
                       NOTIFY isBusyChanged)

    Q_PROPERTY(bool isSignedIn
                   READ isSignedIn
                       NOTIFY authStateChanged)

    Q_PROPERTY(QString userName
                   READ userName
                       NOTIFY profileUpdated)

    Q_PROPERTY(QString userEmail
                   READ userEmail
                       NOTIFY profileUpdated)

    Q_PROPERTY(QString userId
                   READ userId
                       NOTIFY profileUpdated)


    Q_PROPERTY(QString token READ keyToken WRITE setKeyToken NOTIFY tokenChanged FINAL)
    Q_PROPERTY(QString refreshToken READ keyRefreshToken WRITE setKeyRefreshToken NOTIFY refreshTokenChanged FINAL)


public:

    explicit AuthServices(QObject *parent = nullptr);
    ~AuthServices() override;


    QString statusText() const;
    bool isBusy() const;
    bool isSignedIn() const;

    QString userName() const;
    QString userEmail() const;
    QString userId() const;

    Q_INVOKABLE void login(
        const QString &username,
        const QString &password);

    Q_INVOKABLE void loginWithGoogle();

    Q_INVOKABLE bool checkSavedSession();

    Q_INVOKABLE void refreshToken();

    Q_INVOKABLE void fetchUserProfile();

    Q_INVOKABLE void signOut();

    // Token processing
    void retrieveKeyToken();
    void retriveKeyRefreshToken();

    QString keyToken() const;
    void setKeyToken(const QString &newKeyToken);

    QString keyRefreshToken() const;
    void setKeyRefreshToken(const QString &newKeyRefreshToken);

    QString token() const;
    void setToken(const QString &newToken);

signals:

    void statusTextChanged();
    void isBusyChanged();
    void authStateChanged();
    void profileUpdated();

    void loginSuccess();
    void logoutCompleted();

    void errorOccurred(
        const QString &message);

    void rawLog(
        const QString &message,
        const QString &level);

    void tokenChanged();

    void refreshTokenChanged();

private:

    struct TokenResponse
    {
        bool success = false;

        QString schema;          // "Bearer"

        QString accessToken;    // token
        QString refreshToken;   // refresh_token
        int expiresIn = 0;      // expires_in

        QString luhnToken;      // luhn_token

        // User information
        QString userId;         // user.uuid
        QString username;       // user.username
        QString email;          // user.email
        QString displayName;    // user.fullname

        bool isValid() const
        {
            return success &&
                   !accessToken.isEmpty() &&
                   !userId.isEmpty();
        }
    };

    // -----------------------------
    // Backend requests
    // -----------------------------

    void loginWithPasswordBackend(
        const QString &username,
        const QString &password);

    void sendGoogleAuthorizationCode(
        const QString &code,
        const QString &redirectUri,
        const QString &codeVerifier,
        const QString &nonce);

    void handleLoginReply(
        QNetworkReply *reply);

    void handleGoogleReply(
        QNetworkReply *reply);

    void handleRefreshReply(
        QNetworkReply *reply);

    void handleProfileReply(
        QNetworkReply *reply);

    void handleGoogleOAuthCallback(
        const QVariantMap &data);
    void cleanupGoogleOAuth();

    // -----------------------------
    // Token processing
    // -----------------------------

    bool parseTokenResponse(
        const QByteArray &data,
        TokenResponse &result);

    void applyTokenResponse(
        const TokenResponse &response);

    void saveTokenSecurely(
        const QString &token);

    void saveRefreshTokenSecurely(
        const QString &token);

    void readAccessTokenSecurely();

    void readRefreshTokenSecurely();

    void clearTokenSecurely();

    void clearRefreshTokenSecurely();

    // -----------------------------
    // HTTP
    // -----------------------------

    QNetworkRequest createRequest(
        const QUrl &url) const;

    QNetworkRequest createAuthenticatedRequest(
        const QUrl &url) const;

    void handleNetworkError(
        QNetworkReply *reply,
        const QString &operation);

    // -----------------------------
    // State
    // -----------------------------

    void setBusy(bool value);
    void setStatusText(const QString &value);
    void setSignedIn(bool value);

    void clearSessionState();

    // -----------------------------
    // Configuration
    // -----------------------------

    static constexpr const char *kAuthBaseUrl ="https://api.dyxi.site";

    static constexpr const char *kLoginEndpoint = "/auth/ipa/login";

    static constexpr const char *kGoogleEndpoint = "/auth/ipa/google-oauth";

    static constexpr const char *kRefreshEndpoint =
        "/auth/refresh";

    static constexpr const char *kLogoutEndpoint =
        "/auth/logout";

    static constexpr const char *kMeEndpoint =
        "/auth/me";

    static constexpr const char *kAccessTokenKey =
        "session_access_token";

    static constexpr const char *kRefreshTokenKey =
        "session_refresh_token";

    const QString m_dummyAuthEndpoint = "https://api.enterprise-security.internal/v1/auth/token";
    const QString m_googleClientId = "google_enterprise_client_id_6739";
    const QString m_appleClientId = "apple_enterprise_client_id_4821";

    // -----------------------------
    // Qt networking
    // -----------------------------

    QNetworkAccessManager *m_networkManager = nullptr;

    QNetworkReply *m_currentReply = nullptr;

    // QOAuth2AuthorizationCodeFlow *m_oauth2Flow = nullptr;

    // QOAuthHttpServerReplyHandler *m_replyHandler = nullptr;

    QOAuthUriSchemeReplyHandler *m_replyHandler = nullptr;

    QString m_googleState;
    QString m_googleCodeVerifier;
    QString m_googleNonce;

    // -----------------------------
    // Session
    // -----------------------------

    QString m_sessionToken;
    QString m_refreshToken;

    QString m_userId;
    QString m_userName;
    QString m_userEmail;
    QString m_displayName;

    int m_tokenExpiresIn = 0;

    // -----------------------------
    // State
    // -----------------------------

    QString m_statusText = "Ready";

    bool m_isBusy = false;
    bool m_isSignedIn = false;

    bool m_restoringSession = false;

    QString generatePkceVerifier();
    QString generatePkceChallenge(
        const QString &verifier);
    QString generateOAuthState();





 // Token
    QString m_keyToken;
    QString m_keyRefreshToken;




    // void sendGoogleAuthorizationCode(
    //     const QString &code,
    //     const QString &redirectUri,
    //     const QString &codeVerifier);

    QString m_token;
};

}