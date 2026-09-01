#include "envconfig.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QDebug>

EnvConfig& EnvConfig::instance()
{
    static EnvConfig s_instance;
    return s_instance;
}

EnvConfig::EnvConfig(QObject *parent)
    : QObject(parent)
    , m_authEndpoint("https://api.enterprise-security.internal/v1/auth/login")
    , m_timeoutMs(1200)
    , m_targetEnv("development")
    , m_googleClientId("952310262713-4b1f3os6uqmhorsjmp9tmritbph1m6sg.apps.googleusercontent.com")
    , m_googleAuthUrl("https://accounts.google.com/o/oauth2/v2/auth")
    , m_googleTokenUrl("https://oauth2.googleapis.com/token")
    , m_appleClientId("com.enterprise.herogate.client")
{
}

void EnvConfig::loadConfig()
{
    // Search for .env.security in application directory, working directory, and app data
    QStringList candidates = {
        QDir::current().filePath(".env.security"),
        QCoreApplication::applicationDirPath() + "/.env.security",
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/.env.security"
    };

    bool fileLoaded = false;
    for (const QString &path : candidates) {
        if (QFile::exists(path)) {
            parseEnvFile(path);
            fileLoaded = true;
            emit logMessage("📄 Loaded configuration file: " + path, "success");
            break;
        }
    }

    // Overlay system environment variables (highest priority)
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("ENTERPRISE_AUTH_ENDPOINT")) m_authEndpoint = env.value("ENTERPRISE_AUTH_ENDPOINT");
    if (env.contains("ENTERPRISE_AUTH_TIMEOUT_MS")) m_timeoutMs = env.value("ENTERPRISE_AUTH_TIMEOUT_MS").toInt();
    if (env.contains("ENTERPRISE_TARGET_ENV")) m_targetEnv = env.value("ENTERPRISE_TARGET_ENV");
    if (env.contains("GOOGLE_CLIENT_ID")) m_googleClientId = env.value("GOOGLE_CLIENT_ID");
    if (env.contains("GOOGLE_AUTH_URL")) m_googleAuthUrl = env.value("GOOGLE_AUTH_URL");
    if (env.contains("GOOGLE_TOKEN_URL")) m_googleTokenUrl = env.value("GOOGLE_TOKEN_URL");
    if (env.contains("APPLE_CLIENT_ID")) m_appleClientId = env.value("APPLE_CLIENT_ID");

    emit configChanged();
    emit logMessage("⚙️ Environment initialized. Target Stage: " + m_targetEnv + " | Google Client ID: " + m_googleClientId, "info");
}

void EnvConfig::parseEnvFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;

        int eqIdx = line.indexOf('=');
        if (eqIdx > 0) {
            QString key = line.left(eqIdx).trimmed();
            QString val = line.mid(eqIdx + 1).trimmed();
            
            // Strip outer quotes if present
            if ((val.startsWith('(') && val.endsWith(')')) || (val.startsWith('"') && val.endsWith('"'))) {
                val = val.mid(1, val.length() - 2);
            }


            m_values[key] = val;

            if (key == "ENTERPRISE_AUTH_ENDPOINT") m_authEndpoint = val;
            else if (key == "ENTERPRISE_AUTH_TIMEOUT_MS") m_timeoutMs = val.toInt();
            else if (key == "ENTERPRISE_TARGET_ENV") m_targetEnv = val;
            else if (key == "GOOGLE_CLIENT_ID") m_googleClientId = val;
            else if (key == "GOOGLE_AUTH_URL") m_googleAuthUrl = val;
            else if (key == "GOOGLE_TOKEN_URL") m_googleTokenUrl = val;
            else if (key == "APPLE_CLIENT_ID") m_appleClientId = val;
        }
    }
    file.close();
}

QString EnvConfig::get(const QString &key, const QString &defaultValue) const
{
    return m_values.value(key, defaultValue);
}
