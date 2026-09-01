#include "keychainhelper.h"
#include <qtkeychain/keychain.h>
#include <QDebug>

KeychainHelper::KeychainHelper(QObject *parent)
    : QObject(parent)
{
}

void KeychainHelper::writeKey(const QString &service, const QString &key, const QString &value)
{
    emit logMessage("🔐 [Keychain] Writing token to OS Keychain (Service: " + service + ", Key: " + key + ")...", "info");

    auto *job = new QKeychain::WritePasswordJob(service, this);
    job->setAutoDelete(true);
    job->setKey(key);
    job->setTextData(value);

    connect(job, &QKeychain::Job::finished, this, [this, key, job]() {
        if (job->error() == QKeychain::NoError) {
            emit logMessage("✅ [Keychain] Token successfully persisted to OS Keychain (" + key + ").", "success");
            emit keyWritten(key);
        } else {
            QString errStr = job->errorString();
            emit logMessage("❌ [Keychain] Failed to write key: " + errStr, "error");
            emit keychainError(key, errStr);
        }
    });

    job->start();
}

void KeychainHelper::readKey(const QString &service, const QString &key)
{
    emit logMessage("🔍 [Keychain] Reading token from OS Keychain (" + key + ")...", "info");

    auto *job = new QKeychain::ReadPasswordJob(service, this);
    job->setAutoDelete(true);
    job->setKey(key);

    connect(job, &QKeychain::Job::finished, this, [this, key, job]() {
        if (job->error() == QKeychain::NoError) {
            QString retrievedValue = job->textData();
            emit logMessage("✅ [Keychain] Retrieved existing session token from OS Keychain.", "success");
            emit keyRead(key, retrievedValue);
        } else if (job->error() == QKeychain::EntryNotFound) {
            emit logMessage("ℹ️ [Keychain] No saved session found in OS Keychain.", "info");
            emit keyRead(key, QString());
        } else {
            QString errStr = job->errorString();
            emit logMessage("⚠️ [Keychain] Read error: " + errStr, "warning");
            emit keychainError(key, errStr);
        }
    });

    job->start();
}

void KeychainHelper::deleteKey(const QString &service, const QString &key)
{
    emit logMessage("🗑️ [Keychain] Deleting credential from OS Keychain (" + key + ")...", "info");

    auto *job = new QKeychain::DeletePasswordJob(service, this);
    job->setAutoDelete(true);
    job->setKey(key);

    connect(job, &QKeychain::Job::finished, this, [this, key, job]() {
        if (job->error() == QKeychain::NoError || job->error() == QKeychain::EntryNotFound) {
            emit logMessage("✅ [Keychain] Credential purged from OS Keychain.", "success");
            emit keyDeleted(key);
        } else {
            QString errStr = job->errorString();
            emit logMessage("❌ [Keychain] Failed to delete key: " + errStr, "error");
            emit keychainError(key, errStr);
        }
    });

    job->start();
}
