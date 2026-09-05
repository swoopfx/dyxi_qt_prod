#include "sessionmanager.h"

// Define constants for QSettings keys to avoid typo errors
namespace SessionKeys {
const QString Group      = "Session/";
const QString Uuid       = "Session/Uuid";
const QString FullName   = "Session/FullName";
const QString Email      = "Session/Email";
const QString Expiration = "Session/Expiration";
}

SessionManager::SessionManager()
    : settings("Orula Deviant Limited", "Orula Dyxi") {
    // QSettings is initialized globally for this class instance
}

// Stores all user data into permanent registry/file settings
void SessionManager::saveSession(const UserSessionx &session) {
    settings.setValue(SessionKeys::Uuid, session.uuid);
    settings.setValue(SessionKeys::FullName, session.fullName);
    settings.setValue(SessionKeys::Email, session.email);
    // Stores QDateTime directly; QSettings serializes it cleanly
    settings.setValue(SessionKeys::Expiration, session.expirationTime);
    settings.sync(); // Force writes to disk immediately
}

// Retrieves user data and populates the UserSession structure
UserSessionx SessionManager::loadSession() const {
    UserSessionx session;

    // Verify if at least the UUID exists before populating data
    if (!settings.contains(SessionKeys::Uuid)) {
        session.isValid = false;
        return session;
    }

    session.uuid = settings.value(SessionKeys::Uuid).toString();
    session.fullName = settings.value(SessionKeys::FullName).toString();
    session.email = settings.value(SessionKeys::Email).toString();
    session.expirationTime = settings.value(SessionKeys::Expiration).toDateTime();
    session.isValid = true;

    return session;
}

// Deletes the entire session block from storage
void SessionManager::clearSession() {
    settings.remove("Session");
}

// Instantly checks if the saved session has run past its expiry time
bool SessionManager::isSessionExpired() const {
    if (!settings.contains(SessionKeys::Expiration)) {
        return true;
    }
    QDateTime expiry = settings.value(SessionKeys::Expiration).toDateTime();
    return QDateTime::currentDateTime() > expiry;
}
