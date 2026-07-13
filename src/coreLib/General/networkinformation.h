#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#include <QObject>
#include <QNetworkInformation>
#include <QQmlEngine>

class NetworkInformation : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NetworkInformation(QObject *parent = nullptr);

    void checkNetworkStatus();

signals:
    void networkChanged(bool connected);

private slots:
    void onReachabilityChanged(
        QNetworkInformation::Reachability reachability);
};

#endif // NETWORKINFORMATION_H