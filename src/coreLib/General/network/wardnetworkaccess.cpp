#include "wardnetworkaccess.h"

WardNetworkAccess::WardNetworkAccess(QObject *parent):
   AbstractErrorProcessing{parent}
{
    setIsLoadingData(false);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &WardNetworkAccess::onGetWardList);

}

void WardNetworkAccess::loadChildren(const QString &url)
{
    setIsLoadingData(true);
    QNetworkRequest request(url.trimmed());
    manager->get(request);

    // Format the JWT Bearer token string
    QString jwtToken = "YOUR_JWT_BEARER_TOKEN_HERE";
    QByteArray bearerHeaderValue = QString("Bearer %1").arg(jwtToken).toUtf8();
}

void WardNetworkAccess::onGetWardList(QNetworkReply *reply)
{
QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);


}
