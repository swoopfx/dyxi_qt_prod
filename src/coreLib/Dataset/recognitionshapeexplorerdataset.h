#ifndef RECOGNITIONSHAPEEXPLORERDATASET_H
#define RECOGNITIONSHAPEEXPLORERDATASET_H

#include <QObject>
#include <QQmlEngine>
#include "../General/postact.h"
#include "../General/abstracterrorprocessing.h"

class RecognitionShapeExplorerDataset : public AbstractErrorProcessing
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString pageName READ pageName WRITE setPageName NOTIFY pageNameChanged FINAL)
    Q_PROPERTY(QString studentId READ studentId WRITE setStudentId NOTIFY studentIdChanged FINAL)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)
    Q_PROPERTY(QString userAge READ userAge WRITE setuserAge NOTIFY userAgeChanged FINAL)
    Q_PROPERTY(QString gameId READ gameId WRITE setGameId NOTIFY gameIdChanged FINAL)
    Q_PROPERTY(QString gameLevel READ gameLevel WRITE setgameLevel NOTIFY gameLevelChanged FINAL)
    Q_PROPERTY(int gameType READ gameType WRITE setGameType NOTIFY gameTypeChanged FINAL) // defaults to tracing
    Q_PROPERTY(QString gameCategory READ gameCategory WRITE setGameCategory NOTIFY gameCategoryChanged FINAL)
    Q_PROPERTY(double problemSolvingIndex READ problemSolvingIndex WRITE setProblemSolvingIndex NOTIFY problemSolvingIndexChanged FINAL)
    Q_PROPERTY(double creativeIndex READ creativeIndex WRITE setCreativeIndex NOTIFY creativeIndexChanged FINAL)
    Q_PROPERTY(double averageTimeCorrect READ averageTimeCorrect WRITE setAverageTimeCorrect NOTIFY averageTimeCorrectChanged FINAL)
    Q_PROPERTY(double averageTimeFailed READ averageTimeFailed WRITE setAverageTimeFailed NOTIFY averageTimeFailedChanged FINAL)
    Q_PROPERTY(QString activity READ activity WRITE setActivity NOTIFY activityChanged FINAL)

    Q_PROPERTY(double totalGameTime READ totalGameTime WRITE setTotalGameTime NOTIFY totalGameTimeChanged FINAL)
    Q_PROPERTY(double startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged FINAL)
    Q_PROPERTY(int totalCorrect READ totalCorrect WRITE setTotalCorrect NOTIFY totalCorrectChanged FINAL)
    Q_PROPERTY(int totalFailed READ totalFailed WRITE setTotalFailed NOTIFY totalFailedChanged FINAL)
    Q_PROPERTY(int totalTries READ totalTries WRITE setTotalTries NOTIFY totalTriesChanged FINAL)

public:
    explicit RecognitionShapeExplorerDataset(AbstractErrorProcessing *parent = nullptr);
    ~RecognitionShapeExplorerDataset();

    Q_INVOKABLE void gatherData(const QString &, const QVariantMap &); // this gathers the data and make sure all validation requirements runs on it

    QString pageName() const;
    void setPageName(const QString &newPageName);

    QString studentId() const;
    void setStudentId(const QString &newStudentId);

    QString gameId() const;
    void setGameId(const QString &newGameId);

    int gameType() const;
    void setGameType(const int &newGameType);

    QString gameCategory() const;
    void setGameCategory(const QString &newGameCategory);

    QString activity() const;
    void setActivity(const QString &newActivity);

    QString userId() const;
    void setuserId(const QString &newUserId);

    double problemSolvingIndex() const;
    void setProblemSolvingIndex(double newProblemSolvingIndex);

    double creativeIndex() const;
    void setCreativeIndex(double newCreativeIndex);

    double averageTimeCorrect() const;
    void setAverageTimeCorrect(double newAverageTimeCorrect);

    double averageTimeFailed() const;
    void setAverageTimeFailed(double newAverageTimeFailed);

    QString gameLevel() const;
    void setgameLevel(const QString &newGameLevel);

    void setUserId(const QString &newUserId);

    double totalGameTime() const;
    void setTotalGameTime(double newTotalGameTime);

    double startTime() const;
    void setStartTime(double newStartTime);

    int totalCorrect() const;
    void setTotalCorrect(int newTotCorrect);

    int totalFailed() const;
    void setTotalFailed(int newTotalFailed);

    int totalTries() const;
    void setTotalTries(int newTotalTries);

    QString userAge() const;
    void setuserAge(const QString &newUserAge);

signals:
    void pageNameChanged();
    void studentIdChanged();

    void gameIdChanged();

    void gameTypeChanged();

    void gameCategoryChanged();

    void activityChanged();

    void userIdChanged();

    void problemSolvingIndexChanged();

    void creativeIndexChanged();

    void averageTimeCorrectChanged();

    void averageTimeFailedChanged();

    void postRequest(const QByteArray &, const QNetworkRequest &);

    void gameLevelChanged();

    void totalGameTimeChanged();

    void startTimeChanged();

    void totalCorrectChanged();

    void totalFailedChanged();

    void totalTriesChanged();

    // Error

     void requestError(const QString &);

    void userAgeChanged();

private:
    QString m_pageName;
    QString m_studentId;
    QString m_gameId;
    int m_gameType;
    QString m_gameCategory;
    QString m_activity;

    PostAct *postActivity;
    QString m_userId;
    double m_problemSolvingIndex;
    double m_creativeIndex;
    double m_averageTimeCorrect;
    double m_averageTimeFailed;
    QString m_gameLevel;
    double m_totalGameTime;
    double m_startTime;
    int m_totCorrect;
    int m_totalFailed;
    int m_totalTries;
    QString m_userAge;
};

#endif // RECOGNITIONSHAPEEXPLORERDATASET_H
