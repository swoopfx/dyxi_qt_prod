#include "networkinformation.h"
#include <QNetworkInformation>
#include <QDebug>

NetworkInformation::NetworkInformation(QObject *parent)
    : QObject(parent)
{
}

void NetworkInformation::checkNetworkStatus()
{
    if (QNetworkInformation::loadDefaultBackend()) {
        auto networkInfo = QNetworkInformation::instance();

        if (networkInfo) {
            connect(networkInfo,
                    &QNetworkInformation::reachabilityChanged,
                    this,
                    &NetworkInformation::onReachabilityChanged);
        }
    }
}

void NetworkInformation::onReachabilityChanged(
    QNetworkInformation::Reachability reachability)
{
    if (reachability == QNetworkInformation::Reachability::Online) {
        qDebug() << "Connected to the Internet!";
        emit networkChanged(true);
    } else {
        qDebug() << "Network access is limited or disconnected.";
        emit networkChanged(false);
    }
}
