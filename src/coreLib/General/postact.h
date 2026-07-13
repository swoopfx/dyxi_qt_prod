#ifndef POSTACT_H
#define POSTACT_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>

class PostAct : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:

    static PostAct* instance(QObject *parent = nullptr)
    {
        static PostAct* s_instance = new PostAct(parent);
        return s_instance;
    }
    // explicit PostAct(QObject *parent = nullptr);
    ~PostAct();

signals:
    void dataPostedSuccessfully(const QJsonObject &responseData);
    void requestError(const QString &errorString);
    void err(const QString &err);


public slots:
    void handlePostReqest(const QByteArray &data, const QNetworkRequest &request); // handles all post request by the app

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void onLogErrors(const QString &);

private:
    explicit PostAct(QObject *parent = nullptr) ;
    QNetworkAccessManager *manager;
};

#endif // POSTACT_H
