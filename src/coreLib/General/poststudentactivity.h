#ifndef POSTSTUDENTACTIVITY_H
#define POSTSTUDENTACTIVITY_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>

class PostStudentActivity : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PostStudentActivity(QObject *parent = nullptr);

signals:

private slots:
    void onPostRequest(QByteArray, QNetworkRequest); // listens for on post request

private:
    QNetworkAccessManager *manager;

};

#endif // POSTSTUDENTACTIVITY_H
