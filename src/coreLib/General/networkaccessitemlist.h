#ifndef NETWORKACCESSITEMLIST_H
#define NETWORKACCESSITEMLIST_H

#include "abstracterrorprocessing.h"
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QVariantMap>
#include <QQmlEngine>
#include <QNetworkAccessManager>

class NetworkAccessItemList : public AbstractErrorProcessing
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isLoadedData READ isLoadedData WRITE setIsLoadedData NOTIFY isLoadedDataChanged FINAL)
    Q_PROPERTY(QVariantList itemGameType READ itemGameType  NOTIFY itemGameTypeChanged)
    Q_PROPERTY(bool isLoadingData READ isLoadingData  NOTIFY isLoadingDataChanged );

public:
    explicit NetworkAccessItemList(QObject *parent = nullptr);

    Q_INVOKABLE void getItemGameTypeApiRequest(const QString &url);
    // Q_INVOKABLE void singleDataApiRequest(const QString &url);

    bool isLoadedData() const;
    void setIsLoadedData(bool newIsLoadedData);

   QVariantList itemGameType() const;



    bool isLoadingData() const;

signals:
    void isLoadedDataChanged();
    void itemGameTypeChanged();
    void isLoadingDataChanged();

private slots:
    void onGetItemGameTypeFinished(QNetworkReply *reply);


private:
    bool m_isLoadedData;
    QNetworkAccessManager *manager;


    // List/ Array reference this references a list of data
   QVariantList m_itemGameType;
    void setItemGameType(QVariantList item);

    void setIsLoadingData(bool);
    bool m_isLoadingData;
};

#endif // NETWORKACCESSITEMLIST_H
