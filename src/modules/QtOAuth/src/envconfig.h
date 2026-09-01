#ifndef ENVCONFIG_H
#define ENVCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QQmlEngine>

/**
 * @class EnvConfig
 * @brief Manages environment variables and .env.security settings.
 */
class EnvConfig : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString authEndpoint READ authEndpoint NOTIFY configChanged)
    Q_PROPERTY(int timeoutMs READ timeoutMs NOTIFY configChanged)
    Q_PROPERTY(QString targetEnv READ targetEnv NOTIFY configChanged)
    Q_PROPERTY(QString googleClientId READ googleClientId NOTIFY configChanged)
    Q_PROPERTY(QString googleAuthUrl READ googleAuthUrl NOTIFY configChanged)
    Q_PROPERTY(QString googleTokenUrl READ googleTokenUrl NOTIFY configChanged)
    Q_PROPERTY(QString appleClientId READ appleClientId NOTIFY configChanged)

public:
    static EnvConfig& instance();
    explicit EnvConfig(QObject *parent = nullptr);

    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE QString get(const QString &key, const QString &defaultValue = QString()) const;

    QString authEndpoint() const { return m_authEndpoint; }
    int timeoutMs() const { return m_timeoutMs; }
    QString targetEnv() const { return m_targetEnv; }
    QString googleClientId() const { return m_googleClientId; }
    QString googleAuthUrl() const { return m_googleAuthUrl; }
    QString googleTokenUrl() const { return m_googleTokenUrl; }
    QString appleClientId() const { return m_appleClientId; }

signals:
    void configChanged();
    void logMessage(const QString &message, const QString &type);

private:
    void parseEnvFile(const QString &filePath);

    QString m_authEndpoint;
    int m_timeoutMs;
    QString m_targetEnv;
    QString m_googleClientId;
    QString m_googleAuthUrl;
    QString m_googleTokenUrl;
    QString m_appleClientId;
    QMap<QString, QString> m_values;
};

#endif // ENVCONFIG_H
