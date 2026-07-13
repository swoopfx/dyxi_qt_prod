#include "envconfig.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>

EnvConfig::EnvConfig(QObject *parent)
    : QObject(parent)
    , m_authEndpoint("https://api.enterprise-security.internal/v1/auth/login")
    , m_timeoutMs(1200)
    , m_targetEnv("development")
{
}

void EnvConfig::loadFromSystem()
{
    emit logMessage("⚡ [EnvConfig::loadFromSystem] Commencing secure cross-platform environment configuration mapping...", "info");
    QProcessEnvironment systemEnv = QProcessEnvironment::systemEnvironment();

    // Map central auth endpoints
    if (systemEnv.contains("ENTERPRISE_AUTH_ENDPOINT")) {
        setAuthEndpoint(systemEnv.value("ENTERPRISE_AUTH_ENDPOINT"));
        emit logMessage("📡 Environment Mapped: ENTERPRISE_AUTH_ENDPOINT=" + m_authEndpoint, "success");
    }
    
    // Map custom server-timeout limits
    if (systemEnv.contains("ENTERPRISE_AUTH_TIMEOUT_MS")) {
        bool ok = false;
        int customTimeout = systemEnv.value("ENTERPRISE_AUTH_TIMEOUT_MS").toInt(&ok);
        if (ok) {
            setTimeoutMs(customTimeout);
            emit logMessage("📡 Environment Mapped: ENTERPRISE_AUTH_TIMEOUT_MS=" + QString::number(m_timeoutMs) + "ms", "success");
        }
    }
    
    // Map targeted execution stage environments (staging / development / production)
    if (systemEnv.contains("ENTERPRISE_TARGET_ENV")) {
        setTargetEnv(systemEnv.value("ENTERPRISE_TARGET_ENV"));
        emit logMessage("📡 Environment Mapped: ENTERPRISE_TARGET_ENV=" + m_targetEnv, "success");
    }

    // Resolve secure platform-independent writable paths for secondary overrides
    // Windows: C:/Users/<User>/AppData/Local/<OrgName>/<AppName>
    // macOS:   /Users/<User>/Library/Preferences/<OrgName>/<AppName>
    // Linux:   /home/<User>/.config/<OrgName>/<AppName>
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(configDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QFile file(dir.filePath(".env.security"));
    emit logMessage("📂 Checking secure platform-independent configuration directory: " + configDir, "info");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit logMessage("📂 Local secure override file found! Extracting variable maps...", "info");
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith("#")) continue;
            
            QStringList parts = line.split("=");
            if (parts.size() >= 2) {
                QString key = parts.at(0).trimmed();
                QString val = parts.mid(1).join("=").trimmed();
                m_configMap[key] = val;
                
                // Keep credentials secure by not revealing secrets in logs
                if (key.contains("SECRET") || key.contains("TOKEN") || key.contains("PASSWORD")) {
                    emit logMessage("🤫 Mapped secure variable override: " + key + "=[ENCRYPTED_AT_REST]", "success");
                } else {
                    emit logMessage("✅ Mapped secure variable override: " + key + "=" + val, "success");
                }
            }
        }
        file.close();
    } else {
        emit logMessage("ℹ️ No override .env.security file detected in path location. Continuing with defaults.", "info");
    }

    emit envLoaded();
    emit logMessage("🚀 Secure environment configuration loaded successfully!", "success");
}

void EnvConfig::mapVariable(const QString &key, const QString &envName, const QString &defaultValue)
{
    QProcessEnvironment systemEnv = QProcessEnvironment::systemEnvironment();
    QString finalValue = defaultValue;
    if (systemEnv.contains(envName)) {
        finalValue = systemEnv.value(envName);
    } else if (m_configMap.contains(key)) {
        finalValue = m_configMap.value(key);
    }
    m_configMap[key] = finalValue;
}

QString EnvConfig::getMappedValue(const QString &key) const
{
    return m_configMap.value(key, "");
}

QString EnvConfig::getSystemPath(const QString &folderName) const
{
    if (folderName == "config") {
        return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    } else if (folderName == "cache") {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    } else if (folderName == "data") {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

void EnvConfig::setAuthEndpoint(const QString &endpoint)
{
    if (m_authEndpoint != endpoint) {
        m_authEndpoint = endpoint;
        emit authEndpointChanged();
    }
}

void EnvConfig::setTimeoutMs(int timeout)
{
    if (m_timeoutMs != timeout) {
        m_timeoutMs = timeout;
        emit timeoutMsChanged();
    }
}

void EnvConfig::setTargetEnv(const QString &env)
{
    if (m_targetEnv != env) {
        m_targetEnv = env;
        emit targetEnvChanged();
    }
}

bool EnvConfig::isProduction() const
{
    return m_targetEnv.toLower() == "production" || m_targetEnv.toLower() == "prod";
}
