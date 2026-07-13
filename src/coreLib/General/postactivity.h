#ifndef POSTACTIVITY_H
#define POSTACTIVITY_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>

class PostActivity : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PostActivity(QObject *parent = nullptr);

signals:
    void dataPostedSuccessfully(const QJsonObject &responseData);
    void requestError(const QString &errorString);


public slots:
    void handlePostReqest(QByteArray &data, QNetworkRequest &request); // handles all post request by the app

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    QNetworkAccessManager *manager;

};

#endif // POSTACTIVITY_H
