#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQmlEngine>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

#include "keychainhelper.h"
#include "envconfig.h"

/**
 * @class AuthService
 * @brief Primary Authentication Controller for Qt 6.11.
 * 
 * Features:
 * - Conventional Google OAuth 2.0 with PKCE (S256) - NO client secret required.
 * - Enterprise REST authentication (Email / Password).
 * - Automatic OS Keychain token persistence via KeychainHelper.
 * - Auto-login on application startup.
 */
class AuthService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)
    Q_PROPERTY(bool isSignedIn READ isSignedIn NOTIFY authStateChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY profileUpdated)
    Q_PROPERTY(QString userEmail READ userEmail NOTIFY profileUpdated)
    Q_PROPERTY(QString redirectPage READ redirectPage WRITE setRedirectPage NOTIFY redirectPageChanged)
    Q_PROPERTY(QString authProvider READ authProvider NOTIFY profileUpdated)

public:
    explicit AuthService(QObject *parent = nullptr);
    ~AuthService();

    QString statusText() const { return m_statusText; }
    bool isBusy() const { return m_isBusy; }
    bool isSignedIn() const { return m_isSignedIn; }
    QString userName() const { return m_userName; }
    QString userEmail() const { return m_userEmail; }
    QString redirectPage() const { return m_redirectPage; }
    QString authProvider() const { return m_authProvider; }

    void setRedirectPage(const QString &page) {
        if (m_redirectPage != page) {
            m_redirectPage = page;
            emit redirectPageChanged();
        }
    }

    /**
     * @brief Authenticates via conventional REST email & password.
     */
    Q_INVOKABLE void verifyCredentials(const QString &email, const QString &password);

    /**
     * @brief Initiates conventional Google OAuth 2.0 PKCE flow (No client secret).
     */
    Q_INVOKABLE void loginWithOAuth(const QString &provider);

    /**
     * @brief Reads saved token from the OS Keychain to auto-login.
     */
    Q_INVOKABLE void checkSavedSession();

    /**
     * @brief Clears the active session and deletes token from OS Keychain.
     */
    Q_INVOKABLE void signOut();

    /**
     * @brief Relays raw debug log messages to QML console.
     */
    Q_INVOKABLE void rawLog(const QString &message, const QString &type = "info");

signals:
    void statusTextChanged();
    void isBusyChanged();
    void authStateChanged();
    void profileUpdated();
    void redirectPageChanged();
    void loginSuccess();
    void errorOccurred(const QString &errorMessage);
    void logMessage(const QString &message, const QString &type);

private slots:
    void handleLoginReply();
    void onKeychainTokenRead(const QString &key, const QString &token);
    void onOAuthGranted();
    void onOAuthError(const QString &error, const QString &errorDescription, const QUrl &uri);

private:
    void setStatusText(const QString &text);
    void setBusy(bool busy);
    void setSignedIn(bool signedIn);

    // PKCE generation
    struct PkcePair { QString verifier; QString challenge; };
    PkcePair generatePkcePair();

    QString m_statusText;
    bool m_isBusy;
    bool m_isSignedIn;
    QString m_userName;
    QString m_userEmail;
    QString m_sessionToken;
    QString m_redirectPage;
    QString m_authProvider;

    QString m_codeVerifier;

    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply;
    QOAuth2AuthorizationCodeFlow *m_oauth2Flow;
    QOAuthHttpServerReplyHandler *m_replyHandler;
    KeychainHelper *m_keychainHelper;

    const QString KEYCHAIN_SERVICE = "HeroGateEnterprise";
    const QString KEYCHAIN_TOKEN_KEY = "auth_session_token";
};

#endif // AUTHSERVICE_H
