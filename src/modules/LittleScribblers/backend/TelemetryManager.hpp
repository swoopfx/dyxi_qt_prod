#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtQml/qqmlregistration.h>
#include "AppDifficulty.hpp"
#include "TracingEngine.hpp"

class TelemetryManager : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString endpointUrl READ endpointUrl WRITE setEndpointUrl NOTIFY endpointUrlChanged)
    Q_PROPERTY(bool isSubmitting READ isSubmitting NOTIFY isSubmittingChanged)
    Q_PROPERTY(QString lastReceiptJson READ lastReceiptJson NOTIFY lastReceiptJsonChanged)
    Q_PROPERTY(int lastStatusCode READ lastStatusCode NOTIFY lastStatusCodeChanged)

public:
    explicit TelemetryManager(QObject *parent = nullptr);

    QString endpointUrl() const { return m_endpointUrl; }
    void setEndpointUrl(const QString &url);

    bool isSubmitting() const { return m_isSubmitting; }
    QString lastReceiptJson() const { return m_lastReceiptJson; }
    int lastStatusCode() const { return m_lastStatusCode; }

    // Invokable from QML when the user clicks the Submit button
    Q_INVOKABLE void submitSessionTelemetry(TracingEngine *engine, const QString &appTypePage);

signals:
    void endpointUrlChanged();
    void isSubmittingChanged();
    void lastReceiptJsonChanged();
    void lastStatusCodeChanged();
    void telemetrySuccess(const QString &message, int statusCode, const QString &payloadJson);
    void telemetryError(const QString &errorMessage);

private slots:
    void onNetworkReplyFinished(QNetworkReply *reply);

private:
    QString m_endpointUrl = "https://httpbin.org/post";
    bool m_isSubmitting = false;
    QString m_lastReceiptJson = "{}";
    int m_lastStatusCode = 0;

    QNetworkAccessManager *m_networkManager = nullptr;
};
