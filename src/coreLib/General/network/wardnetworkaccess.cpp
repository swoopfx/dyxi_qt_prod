#include "wardnetworkaccess.h"

WardNetworkAccess::WardNetworkAccess(QObject *parent):
   AbstractErrorProcessing{parent}
 , m_authServices(new OAuth::AuthServices(this))
{
    setIsLoadingData(false);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &WardNetworkAccess::onGetWardList);
    m_authServices->retrieveKeyToken();
    m_authServices->retriveKeyRefreshToken();
}

void WardNetworkAccess::loadChildren(const QString &url)
{
    setIsLoadingData(true);
    QNetworkRequest request(url.trimmed());


    // Format the JWT Bearer token string
    QString jwtToken = m_authServices->keyToken();
    QByteArray bearerHeaderValue = QString("Bearer %1").arg(jwtToken).toUtf8();

    // Set the Authorization header
    request.setRawHeader("Authorization", bearerHeaderValue);

        manager->get(request);
}

QVariantList WardNetworkAccess::wardList() const
{
    return m_wardList;
}

void WardNetworkAccess::onGetWardList(QNetworkReply *reply)
{
QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

if(reply->error() == QNetworkReply::NoError){

    if(statusCodeVariant.isValid()){

        int statusCode = statusCodeVariant.toInt();

        if(statusCode >=200 && statusCode <300){
            // extract all data
              QByteArray responseData = reply->readAll();
               QJsonDocument doc = QJsonDocument::fromJson(responseData);
                 setIsLoadingData(false);

                  if(!doc.isNull() && doc.isObject()){

                      QJsonObject jsonObject = doc.object();
                      //Convert to QVariantMap
                      QVariantMap dataMappy = doc.object().toVariantMap();
                      if(!dataMappy.isEmpty()){
                          QVariant data = dataMappy.value("data");
                          QVariantList variantList = data.toList();
                          setWardList(variantList);
                          setIsLoadedData(true);
                      }else{
                           emit requestFailed("No Data Available");
                      }

                  }else{
                      emit requestFailed("No Data Available");
                  }

        }else{

        }

    }else{
        emit requestFailed("Network Error");
    }

}else{
    emit requestFailed("Network Error");
}


}

void WardNetworkAccess::setWardList(QVariantList item)
{

}




