#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QJsonObject>
#include <QTimer>
#include <QMultiMap>
// #include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <qtkeychain/keychain.h>

class AuthService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Expose status and user details to QML
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY profileUpdated)
    Q_PROPERTY(QString userEmail READ userEmail NOTIFY profileUpdated)
    Q_PROPERTY(QString userPicture READ userPicture NOTIFY profileUpdated)

public:
    explicit AuthService(QObject *parent = nullptr);
    ~AuthService();

    enum class AuthState {
        Idle,
        GeneratingUrl,
        ListeningLoopback,
        CodeReceived,
        ExchangingToken,
        SuccessSignedIn,
        ErrorOccurred
    };
    Q_ENUM(AuthState)

    enum class AuthError {
        NoError,
        NetworkOffline,
        LocalServerPortInUse,
        StateCsrfMismatch,
        UserCancelled,
        TokenExchangeFailed,
        SslHandshakeFailed,
        InvalidConfiguration,
        KeychainAccessFailed
    };
    Q_ENUM(AuthError)

    // Direct QML-triggerable methods
    Q_INVOKABLE void startGoogleLogin();
    Q_INVOKABLE void startAppleLogin();
    Q_INVOKABLE void cancelLogin();
    Q_INVOKABLE void resetState();
    
    // 🔄 Credentials validation and session preservation checks via QtKeychain
    Q_INVOKABLE bool checkSavedSession();
    Q_INVOKABLE void signOut();

    // Property getters
    QString statusText() const { return m_statusText; }
    bool isBusy() const;
    QString userName() const { return m_userName; }
    QString userEmail() const { return m_userEmail; }
    QString userPicture() const { return m_userPicture; }

signals:
    void statusTextChanged();
    void isBusyChanged();
    void profileUpdated();
    
    // Detailed signals to log and feed back to design UI
    void stateChanged(AuthState state);
    void authErrorOccurred(AuthError error, const QString &errorMessage);
    void rawLog(const QString &msg, const QString &type = "info");

private slots:
    void onAuthenticated();
    void onAuthFailed(const QString &error, const QString &errorDescription);
    void onTimeout();

private:
    void setStatus(AuthState state, const QString &statusText);
    void raiseError(AuthError error, const QString &details);
    void generateCsrfState();
    
    // Helper to request active profiles from the user endpoint using acquired Access Token
    void fetchUserProfile(const QString &token);

    // Client Config (Dynamically resolved from system environments in a platform-independent manner)
    QString m_googleClientId;
    QString m_appleClientId;
    QString m_googleClientSecret;
    QString m_appleClientSecret;
    QString m_dummyAuthEndpoint;
    int m_loopbackPort { 8080 };

    // Authentication States Memory
    AuthState m_state { AuthState::Idle };
    QString m_statusText { "Ready" };
    QString m_csrfState;
    QString m_currentProvider; // "google" or "apple"
    
    // User info
    QString m_userName;
    QString m_userEmail;
    QString m_userPicture;

    // Networking Components (Native QtNetworkAuth elements)
    QOAuth2AuthorizationCodeFlow *m_oauth2Flow { nullptr };
    QOAuthHttpServerReplyHandler *m_replyHandler { nullptr };
    QNetworkAccessManager *m_networkManager { nullptr };
    QTimer *m_timeoutTimer { nullptr };
};
