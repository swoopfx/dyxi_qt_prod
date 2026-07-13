#include "networkaccessitemlist.h"

NetworkAccessItemList::NetworkAccessItemList(QObject *parent)
    : AbstractErrorProcessing{parent}
{
    setIsLoadingData(false);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkAccessItemList::onGetItemGameTypeFinished);
}

void NetworkAccessItemList::getItemGameTypeApiRequest(const QString &url)
{
    setIsLoadingData(true);
    qInfo() << url;
    QNetworkRequest request(url.trimmed());
    manager->get(request);
}

bool NetworkAccessItemList::isLoadedData() const
{
    return m_isLoadedData;
}

void NetworkAccessItemList::setIsLoadedData(bool newIsLoadedData)
{
    if (m_isLoadedData == newIsLoadedData)
        return;
    m_isLoadedData = newIsLoadedData;
    emit isLoadedDataChanged();
}

QVariantList NetworkAccessItemList::itemGameType() const
{
    return m_itemGameType;
}

bool NetworkAccessItemList::isLoadingData() const
{
    return m_isLoadingData;
}

void NetworkAccessItemList::onGetItemGameTypeFinished(QNetworkReply *reply)
{
    QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qInfo() << "Request finished and processing started";
    if(reply->error() == QNetworkReply::NoError){

        if(statusCodeVariant.isValid()){
            int statusCode = statusCodeVariant.toInt();
            qInfo() << "RProcessingd";
            if(statusCode >=200 && statusCode <300){
                QByteArray responseData = reply->readAll(); // extract all data
                QJsonDocument doc = QJsonDocument::fromJson(responseData);
                setIsLoadingData(false);
                if(!doc.isNull() && doc.isObject()){
                    //Convert the document to array
                    // QJsonArray jsonArray = doc.array();
                    qInfo() << "isObjectd";
                    QJsonObject jsonObject = doc.object();

                    //Convert to QVariantMap
                    QVariantMap dataMappy = doc.object().toVariantMap();

                    if(!dataMappy.isEmpty()){
                        QVariant data = dataMappy.value("data");
                        QVariantList variantList = data.toList();


                        //  convert to variant list
                        // QVariantList variantList = jsonArray.toVariantList();

                        // QList<QVariantMap> variantMapList;
                        // qInfo() << "VariantMap";
                        // foreach (const QVariant& variant, variantList) {
                        //     if (variant.canConvert<QVariantMap>()) {
                        //         variantMapList.append(variant.toMap());
                        //         // setItemGameType(variantMapList);
                        //     } else {
                        //         //TODO  throw error if converting from and log error
                        //         qWarning() << "QVariant is not a QVariantMap, skipping.";
                        //         emit requestFailed("Data Conversion Error");
                        //     }
                        // }

                        // int index = 0;
                        // for (const QVariant &v : variantList) {
                        //     // v.toString() works for many types, or use switch(v.userType())
                        //     qDebug() << "Item" << index++ << ":" << v.value<QMap>;

                        //     // If the item is another list or map, handle it recursively
                        //     if (v.canConvert<QVariantList>()) {
                        //         qDebug() << "Nested List:" << v.toList();
                        //     }
                        // }

                        // setItemGameType
                        setItemGameType(variantList);
                        setIsLoadedData(true);
                    }
                }else{
                    emit requestFailed("Can Find User");
                }

            }
        }else{
            emit requestFailed("Cant Identify datatype");
            setIsLoadingData(false);
        } // TODO define else condition
    }else{
        emit requestFailed("Network Error");
    }

    reply->deleteLater();
}
void NetworkAccessItemList::setIsLoadingData(bool loading)
{
    if (m_isLoadingData == loading)
        return;
    m_isLoadingData = loading;
    emit isLoadingDataChanged();
}



void  NetworkAccessItemList::setItemGameType(QVariantList item)
{
    m_itemGameType = item;
    emit itemGameTypeChanged();
}








