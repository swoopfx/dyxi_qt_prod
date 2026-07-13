#ifndef NEWORKACCESSMANAGER_H
#define NEWORKACCESSMANAGER_H

#include "abstracterrorprocessing.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QVariantMap>
#include <QDebug>
#include <QQmlEngine>

/*!
 * \brief The NeworkAccessManager Provide all network communication
 * \class NeworkAccessManager class
 */
class NeworkAccessManager : public AbstractErrorProcessing
{
    Q_OBJECT
    QML_ELEMENT

    // property accsible
    Q_PROPERTY(QVariantMap profile READ profile WRITE setProfile NOTIFY profileChanged FINAL)
    Q_PROPERTY(QVariantMap profileDataMap READ profileDataMap WRITE setprofileDataMap NOTIFY profileDataMapChanged FINAL)
    Q_PROPERTY(bool isLoadingData READ isLoadingData  NOTIFY isLoadingDataChanged );

public:
    using AbstractErrorProcessing::AbstractErrorProcessing;
    explicit NeworkAccessManager(QObject *parent = nullptr);
    bool isLoadingData() const{return m_isLoadingData;}
    Q_INVOKABLE void getProfileApiRequest(const QString &url);
    Q_INVOKABLE void getGamesApiRequest(const QString &url);


    QVariantMap profile() const;
    void setProfile(const QVariantMap &newProfile);

    QVariantMap profileDataMap() const;
    void setprofileDataMap(const QVariantMap &newProfileDataMap);



signals:


    void profileChanged();
    void profileDataMapChanged();

    void isLoadingDataChanged();
    void changePage(const QString &pageName);



private slots:
    void onGetProfileDetailsApiFinished(QNetworkReply *reply);



private:
    // Network objects
    QNetworkAccessManager* manager;

    // Profile at login variable Object
    QVariantMap m_profile;
    QVariantMap m_profileDataMap;

    // General Condition  watch
    bool m_isLoadingData = false;
    void setIsLoadingData(bool loading);



};

#endif // NEWORKACCESSMANAGER_H
