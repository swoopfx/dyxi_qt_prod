#include "networkaccesspresets.h"


NetworkAccessPresets::NetworkAccessPresets(QObject *parent)
    : AbstractErrorProcessing{parent}
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkAccessPresets::onGameTypeIdFinished);
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkAccessPresets::onGamecategoryIdFinished);
}

void NetworkAccessPresets::getGameTypeIdRequest(const QString &url)
{
    setIsLoadingData(true);
    QNetworkRequest request(url);
    manager->get(request);


}

void NetworkAccessPresets::getGameCategoryIdRequest(const QString &url)
{
    setIsLoadingData(true);
    QNetworkRequest request(url);
    manager->get(request);
}

void NetworkAccessPresets::getProgramsIdRequest(const QString &url)
{    setIsLoadingData(true);
    QNetworkRequest request(url);
    manager->get(request);
}

QVariantList NetworkAccessPresets::gameTypeIds() const
{
    return m_gameTypeIds;
}

void NetworkAccessPresets::setGameTypeIds(const QVariantList &newGameTypeIds)
{
    if (m_gameTypeIds == newGameTypeIds)
        return;
    m_gameTypeIds = newGameTypeIds;
    emit gameTypeIdsChanged();
}

void NetworkAccessPresets::onGameTypeIdFinished(QNetworkReply *reply)
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
                        // qInfo() << variant ;
                        setGameTypeIds(variantList);
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

void NetworkAccessPresets::onGamecategoryIdFinished(QNetworkReply *reply)
{
    QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qInfo() << "Request finished and processing started";
    if(reply->error() == QNetworkReply::NoError){

        if(statusCodeVariant.isValid()){
            int statusCode = statusCodeVariant.toInt();
            // qInfo() << "RProcessingd";
            if(statusCode >=200 && statusCode <300){
                QByteArray responseData = reply->readAll(); // extract all data
                QJsonDocument doc = QJsonDocument::fromJson(responseData);
                setIsLoadingData(false);
                if(!doc.isNull() && doc.isObject()){
                    //Convert the document to array
                    // QJsonArray jsonArray = doc.array();
                    // qInfo() << "isObjectd";
                    QJsonObject jsonObject = doc.object();

                    //Convert to QVariantMap
                    QVariantMap dataMappy = doc.object().toVariantMap();

                    if(!dataMappy.isEmpty()){
                        QVariant data = dataMappy.value("data");
                        QVariantList variantList = data.toList();
                        // qInfo() << variant ;
                        setGameCategoryIds(variantList);
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

void NetworkAccessPresets::onGameProgramIdFinished(QNetworkReply *reply)
{

    QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(reply->error() == QNetworkReply::NoError){

        if(statusCodeVariant.isValid()){
            int statusCode = statusCodeVariant.toInt();

            if(statusCode >=200 && statusCode <300){
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
                        // qInfo() << variant ;

                       setGameProgramIds(variantList);
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

QVariantList NetworkAccessPresets::gameCategoryIds() const
{
    return m_gameCategoryIds;
}

void NetworkAccessPresets::setGameCategoryIds(const QVariantList &newGameCategoryIds)
{
    if (m_gameCategoryIds == newGameCategoryIds)
        return;
    m_gameCategoryIds = newGameCategoryIds;
    emit gameCategoryIdsChanged();
}

QVariantList NetworkAccessPresets::gameProgramIds()
{
    return m_gameProgramIds;
}

void NetworkAccessPresets::setGameProgramIds(const QVariantList &newGameProgramIds)
{
    if (m_gameProgramIds == newGameProgramIds)
        return;
    m_gameProgramIds = newGameProgramIds;
    emit gameProgramIdsChanged();
}
