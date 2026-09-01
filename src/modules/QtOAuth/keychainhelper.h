#ifndef KEYCHAINHELPER_H
#define KEYCHAINHELPER_H

#include <QObject>
#include <QString>
#include <QQmlEngine>

/**
 * @class KeychainHelper
 * @brief Cross-platform interface to OS secure credential store via QKeychain.
 * 
 * Supports:
 * - Windows: Windows Credential Manager
 * - macOS / iOS: Apple Keychain Services
 * - Linux: Secret Service API / libsecret / KWallet
 * - Android: Android KeyStore via QtKeychain
 */
class KeychainHelper : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit KeychainHelper(QObject *parent = nullptr);

    /**
     * @brief Writes a secret credential into the OS Keychain.
     */
    Q_INVOKABLE void writeKey(const QString &service, const QString &key, const QString &value);

    /**
     * @brief Reads a secret credential from the OS Keychain.
     */
    Q_INVOKABLE void readKey(const QString &service, const QString &key);

    /**
     * @brief Deletes a credential from the OS Keychain.
     */
    Q_INVOKABLE void deleteKey(const QString &service, const QString &key);

signals:
    void keyWritten(const QString &key);
    void keyRead(const QString &key, const QString &value);
    void keyDeleted(const QString &key);
    void keychainError(const QString &key, const QString &errorMessage);
    void logMessage(const QString &message, const QString &type);
};

#endif // KEYCHAINHELPER_H
