#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQmlEngine>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <qtkeychain/keychain.h>

class AuthService : public QObject
{
    Q_OBJECT
    QML_ELEMENT // Modern declarative registration in Qt 6.10

    /**
     * @property statusText
     * @brief Holds the current visual user status of the auth system (e.g. "Ready", "Verifying...").
     */
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

    /**
     * @property isBusy
     * @brief Indicator that is true whenever a REST verification or OAuth token handshake is active.
     */
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

    /**
     * @property userName
     * @brief User's profile name parsed from tokens.
     */
    Q_PROPERTY(QString userName READ userName NOTIFY profileUpdated)

    /**
     * @property userEmail
     * @brief User's profile email address retrieved from the authentication backend.
     */
    Q_PROPERTY(QString userEmail READ userEmail NOTIFY profileUpdated)

    /**
     * @property isSignedIn
     * @brief Tracks whether a valid token session is currently established and active.
     */
    Q_PROPERTY(bool isSignedIn READ isSignedIn NOTIFY authStateChanged)

    /**
     * @property redirectPage
     * @brief Holds the target page string where the user is redirected upon a successful session check.
     */
    Q_PROPERTY(QString redirectPage READ redirectPage WRITE setRedirectPage NOTIFY redirectPageChanged)

public:
    /**
     * @brief Explicit constructor for AuthService.
     * @param parent Optional parent QObject for Qt's hierarchical memory management.
     */
    explicit AuthService(QObject *parent = nullptr);

    /**
     * @brief Destructor for AuthService.
     */
    ~AuthService();

    QString statusText() const { return m_statusText; }
    bool isBusy() const { return m_isBusy; }
    QString userName() const { return m_userName; }
    QString userEmail() const { return m_userEmail; }
    bool isSignedIn() const { return m_isSignedIn; }
    QString redirectPage() const { return m_redirectPage; }
    void setRedirectPage(const QString &page) {
        if (m_redirectPage != page) {
            m_redirectPage = page;
            emit redirectPageChanged();
        }
    }

    /**
     * @brief Verifies a user's standard credentials with username and password.
     * @param username The username/email provided by the user.
     * @param password The user password.
     */
    Q_INVOKABLE void login(const QString &username, const QString &password);

    /**
     * @brief Launches the secure 5-step OAuth 2.0 Single Sign-In protocol.
     * @param provider The OAuth provider to query, i.e., "google" or "apple".
     */
    Q_INVOKABLE void loginWithOAuth(const QString &provider);

    /**
     * @brief Launches the OAuth 2.0 flow configured specifically for registration.
     * @param provider The OAuth provider to query, i.e., "google" or "apple".
     */
    Q_INVOKABLE void registerWithOAuth(const QString &provider);

    /**
     * @brief Destroys the local token session, triggers secure keyring flushes, and logs the user out.
     */
    Q_INVOKABLE void signOut();

    /**
     * @brief Looks for pre-existing persistent credential tokens on the native OS keychain.
     * @return true if a background read job is dispatched, false otherwise.
     */
    Q_INVOKABLE bool checkSavedSession();

    /**
     * @brief Automatically refreshes the expired session access token using a stored refresh token.
     * @details Searches for "session_refresh_token" in the system keyring. If found, dispatches
     *          an asynchronous network POST request to exchange it for a fresh access token.
     */
    Q_INVOKABLE void refreshToken();

    /**
     * @brief Resets transient states like busy and sets status text back to "Ready".
     */
    Q_INVOKABLE void resetState();

    /**
     * @brief Fetches detailed profile clearance and departmental details using the active token.
     */
    Q_INVOKABLE void fetchUserProfile();

    /**
     * @brief Interrupts the auth loop and triggers error notifications.
     * @param errorMsg The error message text.
     */
    Q_INVOKABLE void onAuthFailed(const QString &errorMsg);

    /**
     * @brief Handles inactivity timeouts during OAuth loops, freeing up reply handler sockets.
     */
    Q_INVOKABLE void onTimeout();

signals:
    /**
     * @brief Emitted when the statusText property updates.
     */
    void statusTextChanged();

    /**
     * @brief Emitted when the isBusy property state changes.
     */
    void isBusyChanged();

    /**
     * @brief Emitted when profile values such as name, email, or clearance are updated.
     */
    void profileUpdated();

    /**
     * @brief Emitted when the user changes from logged-in to logged-out or vice-versa.
     */
    void authStateChanged();

    /**
     * @brief Emitted when any verification or loopback server error occurs.
     */
    void errorOccurred(const QString &errorMsg);

    /**
     * @brief Emitted upon successful verification and session storage.
     */
    void loginSuccess();

    /**
     * @brief Relays formatted logging output directly to the UI Console component.
     */
    void rawLog(const QString &message, const QString &type);

    /**
     * @brief Emitted when the redirectPage property state changes.
     */
    void redirectPageChanged();

private slots:
    /**
     * @brief Handles raw REST response payloads returned by QNetworkAccessManager login requests.
     */
    void handleLoginReply();

    /**
     * @brief Handles raw REST response payloads returned by QNetworkAccessManager refresh token requests.
     */
    void handleRefreshReply();

private:
    void setStatusText(const QString &text);
    void setBusy(bool busy);
    void setSignedIn(bool signedIn);
    void saveTokenSecurely(const QString &token);
    void saveRefreshTokenSecurely(const QString &refreshToken);
    void clearTokenSecurely();
    void clearRefreshTokenSecurely();

    QString m_statusText;
    bool m_isBusy = false;
    bool m_isSignedIn = false;
    QString m_userName;
    QString m_userEmail;
    QString m_sessionToken;
    QString m_redirectPage = "design";

    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply = nullptr;
    QOAuth2AuthorizationCodeFlow *m_oauth2Flow = nullptr;
    QOAuthHttpServerReplyHandler *m_replyHandler = nullptr;
    const QString m_authEndpoint = "https://api.enterprise-security.internal/v1/auth/login";
    const QString m_dummyAuthEndpoint = "https://api.enterprise-security.internal/v1/auth/token";
    const QString m_googleClientId = "952310262713-4b1f3os6uqmhorsjmp9tmritbph1m6sg.apps.googleusercontent.com";
    const QString m_googleClientSecret = "google_enterprise_client_secret_xyz123";
    const QString m_appleClientId = "apple_enterprise_client_id_4821";
    const QString m_appleClientSecret = "apple_enterprise_client_secret_abc789";
};

#endif // AUTHSERVICE_H
