#ifndef ENVCONFIG_H
#define ENVCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QProcessEnvironment>
#include <QQmlEngine>

class EnvConfig : public QObject
{
    Q_OBJECT
    QML_ELEMENT // Qt 6.10 Declarative QML Element

    Q_PROPERTY(QString authEndpoint READ authEndpoint WRITE setAuthEndpoint NOTIFY authEndpointChanged)
    Q_PROPERTY(int timeoutMs READ timeoutMs WRITE setTimeoutMs NOTIFY timeoutMsChanged)
    Q_PROPERTY(QString targetEnv READ targetEnv WRITE setTargetEnv NOTIFY targetEnvChanged)
    Q_PROPERTY(bool isProduction READ isProduction NOTIFY envLoaded)

public:
    explicit EnvConfig(QObject *parent = nullptr);

    Q_INVOKABLE void loadFromSystem();
    Q_INVOKABLE void mapVariable(const QString &key, const QString &envName, const QString &defaultValue = "");
    Q_INVOKABLE QString getMappedValue(const QString &key) const;
    Q_INVOKABLE QString getSystemPath(const QString &folderName) const;

    QString authEndpoint() const { return m_authEndpoint; }
    void setAuthEndpoint(const QString &endpoint);

    int timeoutMs() const { return m_timeoutMs; }
    void setTimeoutMs(int timeout);

    QString targetEnv() const { return m_targetEnv; }
    void setTargetEnv(const QString &env);

    bool isProduction() const;

signals:
    void authEndpointChanged();
    void timeoutMsChanged();
    void targetEnvChanged();
    void envLoaded();
    void logMessage(const QString &message, const QString &type);

private:
    QString m_authEndpoint;
    int m_timeoutMs;
    QString m_targetEnv;
    QMap<QString, QString> m_configMap;
};

#endif // ENVCONFIG_H
