#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQmlEngine>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

class AuthService : public QObject
{
    Q_OBJECT
    QML_ELEMENT // Modern declarative registration in Qt 6.10

    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY profileUpdated)
    Q_PROPERTY(QString userEmail READ userEmail NOTIFY profileUpdated)
    Q_PROPERTY(bool isSignedIn READ isSignedIn NOTIFY authStateChanged)

public:
    explicit AuthService(QObject *parent = nullptr);
    ~AuthService();

    QString statusText() const { return m_statusText; }
    bool isBusy() const { return m_isBusy; }
    QString userName() const { return m_userName; }
    QString userEmail() const { return m_userEmail; }
    bool isSignedIn() const { return m_isSignedIn; }

    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE void loginWithOAuth(const QString &provider);
    Q_INVOKABLE void registerWithOAuth(const QString &provider);
    Q_INVOKABLE void signOut();
    Q_INVOKABLE bool checkSavedSession();
    Q_INVOKABLE void resetState();
    Q_INVOKABLE void fetchUserProfile();
    Q_INVOKABLE void onAuthFailed(const QString &errorMsg);
    Q_INVOKABLE void onTimeout();

signals:
    void statusTextChanged();
    void isBusyChanged();
    void profileUpdated();
    void authStateChanged();
    void errorOccurred(const QString &errorMsg);
    void loginSuccess();
    void rawLog(const QString &message, const QString &type);

private slots:
    void handleLoginReply();

private:
    void setStatusText(const QString &text);
    void setBusy(bool busy);
    void setSignedIn(bool signedIn);
    void saveTokenSecurely(const QString &token);
    void clearTokenSecurely();

    QString m_statusText;
    bool m_isBusy = false;
    bool m_isSignedIn = false;
    QString m_userName;
    QString m_userEmail;
    QString m_sessionToken;

    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply = nullptr;
    QOAuth2AuthorizationCodeFlow *m_oauth2Flow = nullptr;
    QOAuthHttpServerReplyHandler *m_replyHandler = nullptr;
    const QString m_authEndpoint = "https://api.enterprise-security.internal/v1/auth/login";
    const QString m_dummyAuthEndpoint = "https://api.enterprise-security.internal/v1/auth/token";
    const QString m_googleClientId = "google_enterprise_client_id_6739";
    const QString m_googleClientSecret = "google_enterprise_client_secret_xyz123";
    const QString m_appleClientId = "apple_enterprise_client_id_4821";
    const QString m_appleClientSecret = "apple_enterprise_client_secret_abc789";
};

#endif // AUTHSERVICE_H
