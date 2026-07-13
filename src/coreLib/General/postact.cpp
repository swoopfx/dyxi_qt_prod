#include "postact.h"
#include <QSslSocket>
#include <QTimer>

PostAct::PostAct(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &PostAct::onReplyFinished);

}

PostAct::~PostAct()
{
 qDebug() << "PostAct destroyed";
}

void PostAct::handlePostReqest(const QByteArray &data, const QNetworkRequest &request)
{


    if(!data.isEmpty()){

        qDebug() <<  "Initiatd Posting ";
        qDebug() << request.url();

        qDebug() << "Request URL:" << request.url();
        qDebug() << "Headers:" << request.rawHeaderList();
        qDebug() << "Payload:" << data;

        qDebug().noquote() << QJsonDocument::fromJson(data).toJson(QJsonDocument::Indented);


        QNetworkReply *reply = manager->post(request, data);

        connect(reply, &QObject::destroyed, []() {
            qDebug() << "Reply destroyed";
        });


        connect(manager, &QObject::destroyed, this, []() {
            qDebug() << "Manager destroyed";
        });

        QTimer::singleShot(15000, this, [reply]() {
            qDebug() << "Timeout check";
            qDebug() << "isRunning:" << reply->isRunning();
            qDebug() << "isFinished:" << reply->isFinished();
            qDebug() << "error:" << reply->error();
        });



        connect(reply, &QNetworkReply::finished, this, [reply]() {
            qDebug() << "FINISHED";
        });

        connect(reply, &QNetworkReply::errorOccurred, this,
                [](QNetworkReply::NetworkError error) {
                    qDebug() << "ERROR:" << error;
                });


        connect(reply, &QNetworkReply::finished, this, [reply]() {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "POST failed";
                qDebug() << "Network error:" << reply->error();
                qDebug() << "Error string:" << reply->errorString();
                qDebug() << "HTTP status:" << statusCode;
            } else {
                qDebug() << "POST succeeded";
                qDebug() << "HTTP status:" << statusCode;
                qDebug() << reply->readAll();
            }


        });


        // Connect SSL error handling for secure HTTPS endpoints
        connect(reply, &QNetworkReply::sslErrors, this,
                [this, reply](const QList<QSslError> &errors) {
            qDebug() << "SSL ERRORS:" << errors;
            onSslErrors(reply, errors);
        });
    }
}

void PostAct::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater(); // Ensure cleanup to avoid memory leaks
   qDebug() <<  "Initiatd Processing ";
    // 1. Validate Network-level errors (e.g., Connection Refused, DNS failure)
    if (reply->error() != QNetworkReply::NoError) {
        emit requestError("Network Error: " + reply->errorString());
        return;
    }

    // 2. Validate HTTP status codes (4xx Client Errors, 5xx Server Errors)
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode < 200 || statusCode >= 300) {
        emit requestError(QString("Server Error. HTTP Status Code: %1").arg(statusCode));
        return;
    }

    // 3. Parse and Validate Response Data
    QByteArray responseData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        emit requestError("Failed to parse JSON response: " + parseError.errorString());
        return;
    }

    if (!jsonDoc.isObject()) {
        emit requestError("Response is not a valid JSON object");
        return;
    }

    // Emit success signal with the parsed data
    emit dataPostedSuccessfully(jsonDoc.object());
}

void PostAct::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    // QString errorList;
    // for (const QSslError &error : errors) {
    //     errorList += error.errorString() + "\n";
    // }
    // emit requestError("SSL Error(s): " + errorList.trimmed());
    // reply->deleteLater();
}

void PostAct::onLogErrors(const QString &)
{
    // call all network functionality
    // and log error
}
