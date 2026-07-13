#include "usersession.h"

UserSession::UserSession(QObject *parent)
    : QObject{parent}
{}

UserSession* UserSession::instance() {
    static UserSession* instance = new UserSession();
    return instance;
}

QString UserSession::userId() const
{
    return m_userId;
}

void UserSession::setUserId(const QString &newUserId)
{
    if (m_userId == newUserId)
        return;
    m_userId = newUserId;
    emit userIdChanged();
}

QString UserSession::userFullName() const
{
    return m_userFullName;
}

void UserSession::setUserFullName(const QString &newUserFullName)
{
    if (m_userFullName == newUserFullName)
        return;
    m_userFullName = newUserFullName;
    emit userFullNameChanged();
}

QString UserSession::userAge() const
{
    return m_userAge;
}



void UserSession::setUserAge(const QString &newUserAge)
{
    if (m_userAge == newUserAge)
        return;
    m_userAge = newUserAge;
    emit userAgeChanged();
}

int UserSession::gameId() const
{
    return m_gameId;
}

void UserSession::setGameId(int newGameId)
{
    if (m_gameId == newGameId)
        return;
    m_gameId = newGameId;
    emit gameIdChanged();
}

int UserSession::activeGameId() const
{
    return m_activeGameId;
}

void UserSession::setActiveGameId(int newActiveGameId)
{
    if (m_activeGameId == newActiveGameId)
        return;
    m_activeGameId = newActiveGameId;
    emit activeGameIdChanged();
}

int UserSession::activeGameTypeId() const
{
    return m_activeGameTypeId;
}

void UserSession::setActiveGameTypeId(int newActiveGameTypeId)
{
    if (m_activeGameTypeId == newActiveGameTypeId)
        return;
    m_activeGameTypeId = newActiveGameTypeId;
    emit activeGameTypeIdChanged();
}

int UserSession::activeGameCategory() const
{
    return m_activeGameCategory;
}

void UserSession::setActiveGameCategory(int newActiveGameCategory)
{
    if (m_activeGameCategory == newActiveGameCategory)
        return;
    m_activeGameCategory = newActiveGameCategory;
    emit activeGameCategoryChanged();
}

QString UserSession::ageBracket() const
{
    return m_ageBracket;
}

void UserSession::setageBracket(const QString &newAgeBracket)
{
    if (m_ageBracket == newAgeBracket)
        return;
    m_ageBracket = newAgeBracket;
    emit ageBracketChanged();
}
