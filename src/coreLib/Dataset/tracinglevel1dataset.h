#ifndef TRACINGLEVEL1DATASET_H
#define TRACINGLEVEL1DATASET_H

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QByteArray>
#include <QNetworkRequest>
#include <QJsonObject>
#include "../General/postact.h"
#include "../General/abstracterrorprocessing.h"


class TracingLevel1Dataset : public AbstractErrorProcessing
{
    Q_OBJECT
    QML_ELEMENT

    // Q_PROPERTY(QString sessionId READ sessionId WRITE setSessionId NOTIFY sessionIdChanged FINAL) // This is the session Id
    // Q_PROPERTY(QString studentId READ studentId WRITE setStudentId NOTIFY studentIdChanged FINAL) // This is the Id of the student
    // Q_PROPERTY(int gameId READ gameId WRITE setGameId NOTIFY gameIdChanged FINAL)
    // Q_PROPERTY(QString gameType READ gameType WRITE setGameType NOTIFY gameTypeChanged FINAL) // defaults to tracing
    // Q_PROPERTY(QString activity READ activity WRITE setActivity NOTIFY activityChanged FINAL) // this defines the json Object

public:
    explicit TracingLevel1Dataset(AbstractErrorProcessing *parent = nullptr);
    using AbstractErrorProcessing::AbstractErrorProcessing;

    Q_INVOKABLE void gatherData(const QString &, const QVariantMap &); // this gathers the data and make sure all validation requirements runs on it

    // QString sessionId() const;
    // void setSessionId(const QString &newSessionId);

signals:

    void postRequest(const QByteArray &, const QNetworkRequest &);
    void requestError(const QString &);

private:

    QByteArray postData;

    PostAct *postActivity;


    // QString m_sessionId;
};

#endif // TRACINGLEVEL1DATASET_H
