#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QDateTime>
#include <QSettings>

    // Structure to group user data tightly
    struct UserSessionx {
    QString uuid;
    QString fullName;
    QString email;
    QDateTime expirationTime;
    bool isValid = false;
};
class SessionManager
{
public:
    // Constructor defines the registry/ini organization and app names
    SessionManager();

    // Core functionality
    void saveSession(const UserSessionx &session);
    UserSessionx loadSession() const;
    void clearSession();
    bool isSessionExpired() const;

private:
    QSettings settings;
};

#endif // SESSIONMANAGER_H
