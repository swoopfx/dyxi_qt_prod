#ifndef WARDNETWORKACCESS_H
#define WARDNETWORKACCESS_H

#include "../abstracterrorprocessing.h"
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QVariantMap>
#include <QQmlEngine>
#include <QNetworkAccessManager>
#include "authservices.h"
// #include "jwtaccessor.h"

class WardNetworkAccess : public AbstractErrorProcessing
{
    Q_OBJECT
    QML_ELEMENT


       Q_PROPERTY(bool isLoadedData READ isLoadedData WRITE setIsLoadedData NOTIFY isLoadedDataChanged FINAL)

           Q_PROPERTY(bool isLoadingData READ isLoadingData  NOTIFY isLoadingDataChanged );

       Q_PROPERTY(QVaraintList wardList READ wardList  NOTIFY wardListChanged FINAL)


public:
    explicit WardNetworkAccess(QObject *parent = nullptr);
     Q_INVOKABLE void loadChildren(const QString &url);

    bool isLoadedData() const;
    void setIsLoadedData(bool newIsLoadedData);
    QVariantList wardList() const;



signals:
     void isLoadedDataChanged();
    void childrenLoaded(const QVariantList &children);
    void childrenLoadFailed(const QString &message);
    void wardListChanged();

private slots:
    void onGetWardList(QNetworkReply *reply);

private :
    bool m_isLoadingData;
    QNetworkAccessManager *manager;
    // JwtAccessor *tokenManager;
    OAuth::AuthServices *m_authServices;

    QVariantList m_wardList;
    void setWardList(QVariantList item);
};

#endif // WARDNETWORKACCESS_H
