#include "neworkaccessmanager.h"
#include "usersession.h"
#include "CoreSettings.hpp"

NeworkAccessManager::NeworkAccessManager(QObject *parent)
    : AbstractErrorProcessing{parent}
{
    setIsLoadingData(false);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NeworkAccessManager::onGetProfileDetailsApiFinished );

}

void NeworkAccessManager::getProfileApiRequest(const QString &url)
{
    setIsLoadingData(true);
    QNetworkRequest request(url);
    manager->get(request);
     qDebug() << "SFinished Request";

}

/*!
 * \brief NeworkAccessManager::getGamesApiRequest
 * \param url
 */
void NeworkAccessManager::getGamesApiRequest(const QString &url)
{
    setIsLoadingData(true);
    QNetworkRequest request(url);
    manager->get(request);
}



QVariantMap NeworkAccessManager::profile() const
{
    return m_profile;
}


/*!
 * \brief NeworkAccessManager::setProfile
 * \param newProfile
 */
void NeworkAccessManager::setProfile(const QVariantMap &newProfile)
{
    if (m_profile == newProfile)
        return;
    m_profile = newProfile;
    emit profileChanged();
}

QVariantMap NeworkAccessManager::profileDataMap() const
{
    return m_profileDataMap;
}

void NeworkAccessManager::setprofileDataMap(const QVariantMap &newProfileDataMap)
{
    if (m_profileDataMap == newProfileDataMap)
        return;
    m_profileDataMap = newProfileDataMap;
    emit profileDataMapChanged();
}


/**
 * @brief NeworkAccessManager::onGetProfileDetailsApiFinished
 * @param reply
 * @param
 *
 */
void NeworkAccessManager::onGetProfileDetailsApiFinished(QNetworkReply *reply)
{
    qDebug() << "Started processing";
    QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(reply->error() == QNetworkReply::NoError){
        if(statusCodeVariant.isValid()){
            int statusCode = statusCodeVariant.toInt();  // extract status code
             qDebug() << "SCode status";
            if(statusCode >= 200 && statusCode < 300){
                QByteArray responseData = reply->readAll(); // extract all data
                QJsonDocument doc = QJsonDocument::fromJson(responseData);
 qDebug() << "Dat Processing";
                if(!doc.isNull() && doc.isObject()){
                    setIsLoadingData(false);
                    m_profile = doc.object().toVariantMap();
                    if(!m_profile.isEmpty()){
                        QVariant data = m_profile.value("data");
                        QVariantMap dataMap = data.toMap();
                        // setStudentDataMap(dataMap);
                        setprofileDataMap(dataMap);
 qDebug() << "Profile setup";
                        // setUser Session Here which include age name and id

                        UserSession::instance()->setUserId(dataMap.value("id").toString());
                        UserSession::instance()->setUserFullName(dataMap.value("studentName").toString());
                        UserSession::instance()->setUserAge(dataMap.value("age").toString());
                        UserSession::instance()->setageBracket(dataMap.value("studentAge").toString());

                        // Get Student Age

                        // qDebug() << dataMap.value("studentName");


                        emit profileChanged();
                         emit changePage(CoreSettings::selectGamePage);
                        // emit changePage(AllString::selectGamePage);
                    }

                }else{
                    emit requestFailed("Failed to Process Document");
                    setIsLoadingData(false);
                }

            }else{
                emit requestFailed("Status Code Error");
                setIsLoadingData(false);
            }


        }else{
            // qDebug() << statusCodeVariant.isValid().
             qDebug() << "invalid Statuscode";
             emit requestFailed("nvalid Statuscode");
              setIsLoadingData(false);
        }
    }else{
        qDebug() << "Network Error";
          emit requestFailed("Network Error");
         setIsLoadingData(false);
    }
     reply->deleteLater();
}




void NeworkAccessManager::setIsLoadingData(bool loading)
{
    if (m_isLoadingData == loading)
        return;
    m_isLoadingData = loading;
    emit isLoadingDataChanged();
}

// void NeworkAccessManager::setItemGameType(QList<QVariantMap> item)
// {
//     m_itemGameType = item;
//     emit itemGameTypeChanged();
// }

// QList<QVariantMap> NeworkAccessManager::itemGameType() const
// {
//     return m_itemGameType;
// }

// bool NeworkAccessManager::isLoadedData() const
// {
//     return m_isLoadedData;
// }

// void NeworkAccessManager::setIsLoadedData(bool newIsLoadedData)
// {
//     if (m_isLoadedData == newIsLoadedData)
//         return;
//     m_isLoadedData = newIsLoadedData;
//     emit isLoadedDataChanged();
// }
