#ifndef NETWORKACCESSPRESETS_H
#define NETWORKACCESSPRESETS_H

#include <QQmlEngine>
#include <QVariantMap>
#include <QVariantList>
#include <QNetworkAccessManager>
#include "abstracterrorprocessing.h"

class NetworkAccessPresets : public AbstractErrorProcessing
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariantList gameTypeIds READ gameTypeIds WRITE setGameTypeIds NOTIFY gameTypeIdsChanged FINAL)
    Q_PROPERTY(QVariantList gameCategoryIds READ gameCategoryIds WRITE setGameCategoryIds NOTIFY gameCategoryIdsChanged FINAL)
    Q_PROPERTY(QVariantList gameProgramIds READ gameProgramIds WRITE setGameProgramIds NOTIFY gameProgramIdsChanged FINAL)

public:
    explicit NetworkAccessPresets(QObject *parent = nullptr);
    Q_INVOKABLE void getGameTypeIdRequest(const QString &url);
    Q_INVOKABLE void getGameCategoryIdRequest(const QString &url);
    Q_INVOKABLE void getProgramsIdRequest(const QString &url);
    QVariantList gameTypeIds() const;
    void setGameTypeIds(const QVariantList &newGameTypeIds);
    QVariantList gameCategoryIds() const;
    void setGameCategoryIds(const QVariantList &newGameCategoryIds);

    QVariantList gameProgramIds() ;
    void setGameProgramIds(const QVariantList &newGameProgramIds);

signals:
    void gameTypeIdsChanged();
    void gameCategoryIdsChanged();
    void gameProgramIdsChanged();

private slots:
    void onGameTypeIdFinished(QNetworkReply *reply);
    void onGamecategoryIdFinished(QNetworkReply *reply);
    void onGameProgramIdFinished(QNetworkReply *reply);
private:
    QVariantList m_gameTypeIds;
    QNetworkAccessManager *manager;

    QVariantList m_gameCategoryIds;
    QVariantList m_gameProgramIds;
};

#endif // NETWORKACCESSPRESETS_H
