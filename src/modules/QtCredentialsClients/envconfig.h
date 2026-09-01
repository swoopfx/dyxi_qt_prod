#ifndef ENVCONFIG_H
#define ENVCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QProcessEnvironment>
#include <QQmlEngine>

/**
 * @class EnvConfig
 * @brief Handles cross-platform security parameter configurations and localized .env overrides.
 * @details Establishes a standard environment variables mapping engine built with Qt 6.10.
 *          Integrates system-level process checks via QProcessEnvironment and implements
 *          user-sandboxed file-level override loops via QStandardPaths to safely resolve paths.
 *          This class is exposed declaratively to QML screens using the QML_ELEMENT macro.
 */
class EnvConfig : public QObject
{
    Q_OBJECT
    QML_ELEMENT // Qt 6.10 Declarative QML Element

    /**
     * @property authEndpoint
     * @brief The primary central gateway API endpoint resolved from the environment.
     */
    Q_PROPERTY(QString authEndpoint READ authEndpoint WRITE setAuthEndpoint NOTIFY authEndpointChanged)

    /**
     * @property timeoutMs
     * @brief The maximum network handshake connection limit in milliseconds before trigger failures.
     */
    Q_PROPERTY(int timeoutMs READ timeoutMs WRITE setTimeoutMs NOTIFY timeoutMsChanged)

    /**
     * @property targetEnv
     * @brief The targeted application stage environment (e.g. "development", "staging", "production").
     */
    Q_PROPERTY(QString targetEnv READ targetEnv WRITE setTargetEnv NOTIFY targetEnvChanged)

    /**
     * @property isProduction
     * @brief Boolean evaluation indicating whether the active workspace matches production standards.
     */
    Q_PROPERTY(bool isProduction READ isProduction NOTIFY envLoaded)

public:
    /**
     * @brief Explicit constructor for EnvConfig.
     * @param parent Optional parent QObject to coordinate automatic Qt memory resource collection.
     */
    explicit EnvConfig(QObject *parent = nullptr);

    /**
     * @brief Initiates system environment inspection and parses secure localized overrides (.env.security).
     * @details Commences standard platform checks, querying QProcessEnvironment for global variables, and
     *          then resolves AppConfig locations to check for and load a secure .env.security override map.
     */
    Q_INVOKABLE void loadFromSystem();

    /**
     * @brief Maps a generic parameter key to a specific environment variable or secondary default fallback.
     * @param key Internal dictionary query identifier used to reference the variable.
     * @param envName System process variable key string to query.
     * @param defaultValue Fallback string value assigned if no matching system or file override is resolved.
     */
    Q_INVOKABLE void mapVariable(const QString &key, const QString &envName, const QString &defaultValue = "");

    /**
     * @brief Retrieves a mapped config value.
     * @param key The key string identifier.
     * @return Mapped value string, or an empty string if unregistered.
     */
    Q_INVOKABLE QString getMappedValue(const QString &key) const;

    /**
     * @brief Utility helper to resolve secure sandboxed directories managed by the operating system kernel.
     * @details Translates standard folder requests ("config", "cache", "data") into absolute paths
     *          using QStandardPaths::writableLocation to comply with local privilege limitations.
     * @param folderName Target configuration folder category requested (e.g. "config").
     * @return Absolute directory path string resolved by standard OS parameters.
     */
    Q_INVOKABLE QString getSystemPath(const QString &folderName) const;

    QString authEndpoint() const { return m_authEndpoint; }
    void setAuthEndpoint(const QString &endpoint);

    int timeoutMs() const { return m_timeoutMs; }
    void setTimeoutMs(int timeout);

    QString targetEnv() const { return m_targetEnv; }
    void setTargetEnv(const QString &env);

    bool isProduction() const;

signals:
    /**
     * @brief Emitted when m_authEndpoint is successfully altered.
     */
    void authEndpointChanged();

    /**
     * @brief Emitted when connection timeouts parameters are updated.
     */
    void timeoutMsChanged();

    /**
     * @brief Emitted when targeted deployment environment labels change.
     */
    void targetEnvChanged();

    /**
     * @brief Emitted upon full parsing and resolution of system parameters.
     */
    void envLoaded();

    /**
     * @brief Relays formatted status alerts and credential overrides logs directly to the UI Console component.
     * @param message Text of the notification to display.
     * @param type Level label indicating message context ("info", "success", "warning", "error").
     */
    void logMessage(const QString &message, const QString &type);

private:
    QString m_authEndpoint;
    int m_timeoutMs;
    QString m_targetEnv;
    QMap<QString, QString> m_configMap; ///< Thread-safe configuration cache map of keys and decrypted variables.
};

#endif // ENVCONFIG_H
