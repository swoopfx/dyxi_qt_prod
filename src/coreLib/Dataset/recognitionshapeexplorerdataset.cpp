#include "recognitionshapeexplorerdataset.h"
#include <QUuid>

RecognitionShapeExplorerDataset::RecognitionShapeExplorerDataset(AbstractErrorProcessing *parent)
    : AbstractErrorProcessing{parent}
{

    postActivity =  PostAct::instance();
    connect(this, &RecognitionShapeExplorerDataset::postRequest, postActivity, &PostAct::handlePostReqest);
}

RecognitionShapeExplorerDataset::~RecognitionShapeExplorerDataset()
{
qDebug() << "RecognitionShapeExplorerDataset destroyed";
}

void RecognitionShapeExplorerDataset::gatherData(const QString &urlEnpoint, const QVariantMap &data)
{
    QByteArray byte;
    QJsonObject jsonObject;
    // QUrl urlPost(url);

    QUrl url(urlEnpoint.trimmed());
    if (!url.isValid()) {
        emit requestError("Invalid URL format: " +urlEnpoint);
        return;
    }

    // Validate The data Packests
    if(data.isEmpty()){
        // emit  validation Error
    }else{

        qInfo() << "Ready to send" << urlEnpoint ;
        QNetworkRequest request(url);

        //Set Headers
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        // request.setRawHeader("Authorization", "Bearer AccessToken");

        // Set and convert Data
        // jsonObject["session_id"] = data.value("sessionid").toString();
        // jsonObject["userid"] = data.value("userId").toString();

        // QVariantMap jsonObjectMap = jsonObject.toVariantMap();
        //  jsonObjectMap.insert(data);
        QUuid uuid = QUuid::createUuid();
        data["users_id"] = m_userId;
        data["session_id"] = uuid.toString();
       QJsonObject finalJsonObj = QJsonObject::fromVariantMap(data);
        // jsonObject["dataPacket"] = data.value();
        QJsonDocument doc(finalJsonObj);
        byte = doc.toJson();
        // The rest does to

   qDebug() <<  "ISent data and emited Post";
        //Trigger the Post
        emit postRequest(byte, request);
    }
    // Serach For empty Data
    //


}

QString RecognitionShapeExplorerDataset::pageName() const
{
    return m_pageName;
}

void RecognitionShapeExplorerDataset::setPageName(const QString &newPageName)
{
    if (m_pageName == newPageName)
        return;
    m_pageName = newPageName;
    emit pageNameChanged();
}

QString RecognitionShapeExplorerDataset::studentId() const
{
    return m_studentId;
}

void RecognitionShapeExplorerDataset::setStudentId(const QString &newStudentId)
{
    if (m_studentId == newStudentId)
        return;
    m_studentId = newStudentId;
    emit studentIdChanged();
}

QString RecognitionShapeExplorerDataset::gameId() const
{
    return m_gameId;
}

void RecognitionShapeExplorerDataset::setGameId(const QString &newGameId)
{
    if (m_gameId == newGameId)
        return;
    m_gameId = newGameId;
    emit gameIdChanged();
}

int RecognitionShapeExplorerDataset::gameType() const
{
    return m_gameType;
}

void RecognitionShapeExplorerDataset::setGameType(const int &newGameType)
{
    if (m_gameType == newGameType)
        return;
    m_gameType = newGameType;
    emit gameTypeChanged();
}

QString RecognitionShapeExplorerDataset::gameCategory() const
{
    return m_gameCategory;
}

void RecognitionShapeExplorerDataset::setGameCategory(const QString &newGameCategory)
{
    if (m_gameCategory == newGameCategory)
        return;
    m_gameCategory = newGameCategory;
    emit gameCategoryChanged();
}

QString RecognitionShapeExplorerDataset::activity() const
{
    return m_activity;
}

void RecognitionShapeExplorerDataset::setActivity(const QString &newActivity)
{
    if (m_activity == newActivity)
        return;
    m_activity = newActivity;
    emit activityChanged();
}

QString RecognitionShapeExplorerDataset::userId() const
{
    return m_userId;
}

void RecognitionShapeExplorerDataset::setuserId(const QString &newUserId)
{
    if (m_userId == newUserId)
        return;
    m_userId = newUserId;
    emit userIdChanged();
}

double RecognitionShapeExplorerDataset::problemSolvingIndex() const
{
    return m_problemSolvingIndex;
}

void RecognitionShapeExplorerDataset::setProblemSolvingIndex(double newProblemSolvingIndex)
{
    if (qFuzzyCompare(m_problemSolvingIndex, newProblemSolvingIndex))
        return;
    m_problemSolvingIndex = newProblemSolvingIndex;
    emit problemSolvingIndexChanged();
}

double RecognitionShapeExplorerDataset::creativeIndex() const
{
    return m_creativeIndex;
}

void RecognitionShapeExplorerDataset::setCreativeIndex(double newCreativeIndex)
{
    if (qFuzzyCompare(m_creativeIndex, newCreativeIndex))
        return;
    m_creativeIndex = newCreativeIndex;
    emit creativeIndexChanged();
}

double RecognitionShapeExplorerDataset::averageTimeCorrect() const
{
    return m_averageTimeCorrect;
}

void RecognitionShapeExplorerDataset::setAverageTimeCorrect(double newAverageTimeCorrect)
{
    if (qFuzzyCompare(m_averageTimeCorrect, newAverageTimeCorrect))
        return;
    m_averageTimeCorrect = newAverageTimeCorrect;
    emit averageTimeCorrectChanged();
}

double RecognitionShapeExplorerDataset::averageTimeFailed() const
{
    return m_averageTimeFailed;
}

void RecognitionShapeExplorerDataset::setAverageTimeFailed(double newAverageTimeFailed)
{
    if (qFuzzyCompare(m_averageTimeFailed, newAverageTimeFailed))
        return;
    m_averageTimeFailed = newAverageTimeFailed;
    emit averageTimeFailedChanged();
}

QString RecognitionShapeExplorerDataset::gameLevel() const
{
    return m_gameLevel;
}

void RecognitionShapeExplorerDataset::setgameLevel(const QString &newGameLevel)
{
    if (m_gameLevel == newGameLevel)
        return;
    m_gameLevel = newGameLevel;
    emit gameLevelChanged();
}

void RecognitionShapeExplorerDataset::setUserId(const QString &newUserId)
{
    if (m_userId == newUserId)
        return;
    m_userId = newUserId;
    emit userIdChanged();
}

double RecognitionShapeExplorerDataset::totalGameTime() const
{
    return m_totalGameTime;
}

void RecognitionShapeExplorerDataset::setTotalGameTime(double newTotalGameTime)
{
    if (qFuzzyCompare(m_totalGameTime, newTotalGameTime))
        return;
    m_totalGameTime = newTotalGameTime;
    emit totalGameTimeChanged();
}

double RecognitionShapeExplorerDataset::startTime() const
{
    return m_startTime;
}

void RecognitionShapeExplorerDataset::setStartTime(double newStartTime)
{
    if (qFuzzyCompare(m_startTime, newStartTime))
        return;
    m_startTime = newStartTime;
    emit startTimeChanged();
}

int RecognitionShapeExplorerDataset::totalCorrect() const
{
    return m_totCorrect;
}

void RecognitionShapeExplorerDataset::setTotalCorrect(int newTotCorrect)
{
    if (m_totCorrect == newTotCorrect)
        return;
    m_totCorrect = newTotCorrect;
    emit totalCorrectChanged();
}

int RecognitionShapeExplorerDataset::totalFailed() const
{
    return m_totalFailed;
}

void RecognitionShapeExplorerDataset::setTotalFailed(int newTotalFailed)
{
    if (m_totalFailed == newTotalFailed)
        return;
    m_totalFailed = newTotalFailed;
    emit totalFailedChanged();
}

int RecognitionShapeExplorerDataset::totalTries() const
{
    return m_totalTries;
}

void RecognitionShapeExplorerDataset::setTotalTries(int newTotalTries)
{
    if (m_totalTries == newTotalTries)
        return;
    m_totalTries = newTotalTries;
    emit totalTriesChanged();
}

QString RecognitionShapeExplorerDataset::userAge() const
{
    return m_userAge;
}

void RecognitionShapeExplorerDataset::setuserAge(const QString &newUserAge)
{
    if (m_userAge == newUserAge)
        return;
    m_userId = newUserAge;
    emit userAgeChanged();
}
