#include "postactivity.h"

PostActivity::PostActivity(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &PostActivity::onReplyFinished);
}

void PostActivity::handlePostReqest(QByteArray &data, QNetworkRequest &request)
{
    if(!data.isEmpty()){
        QNetworkReply *reply = manager->post(request, data);

        // Connect SSL error handling for secure HTTPS endpoints
        connect(reply, &QNetworkReply::sslErrors, this,
                [this, reply](const QList<QSslError> &errors) { onSslErrors(reply, errors); });
    }else{

    }

}

void PostActivity::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater(); // Ensure cleanup to avoid memory leaks

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

void PostActivity::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    QString errorList;
    for (const QSslError &error : errors) {
        errorList += error.errorString() + "\n";
    }
    emit requestError("SSL Error(s): " + errorList.trimmed());
    reply->deleteLater();
}

// void PostActivity::handlePostReqest(QByteArray data, QNetworkRequest request)
// {

// }
