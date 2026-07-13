#ifndef USERSESSION_H
#define USERSESSION_H

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>
#include <QString>

class UserSession : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    // Make it a Singleton Implementation

    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString userFullName READ userFullName WRITE setUserFullName NOTIFY userFullNameChanged FINAL)
    Q_PROPERTY(QString userAge READ userAge WRITE setUserAge NOTIFY userAgeChanged FINAL)
    Q_PROPERTY(QString ageBracket READ ageBracket WRITE setageBracket NOTIFY ageBracketChanged FINAL)

    // Game Data
    Q_PROPERTY(int gameId READ gameId WRITE setGameId NOTIFY gameIdChanged FINAL)
    Q_PROPERTY(int activeGameId READ activeGameId WRITE setActiveGameId NOTIFY activeGameIdChanged FINAL)
    Q_PROPERTY(int activeGameTypeId READ activeGameTypeId WRITE setActiveGameTypeId NOTIFY activeGameTypeIdChanged FINAL)
    Q_PROPERTY(int activeGameCategory READ activeGameCategory WRITE setActiveGameCategory NOTIFY activeGameCategoryChanged FINAL)
    // Q_PROPERTY(int activeUserAge READ activeUserAge WRITE setActiveUserAge NOTIFY activeUserAgeChanged FINAL)
public:
    static UserSession* instance();

    static UserSession *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        // Ensure the engine doesn't delete your static instance
        QJSEngine::setObjectOwnership(instance(), QJSEngine::CppOwnership);
        return instance();
    }

    QString userId() const;
    void setUserId(const QString &newUserId);

    QString userFullName() const;
    void setUserFullName(const QString &newUserFullName);

    QString userAge() const;
    void setUserAge(const QString &newUserAge);

    int gameId() const;
    void setGameId(int newGameId);

    int activeGameId() const;
    void setActiveGameId(int newActiveGameId);

    int activeGameTypeId() const;
    void setActiveGameTypeId(int newActiveGameTypeId);

    int activeGameCategory() const;
    void setActiveGameCategory(int newActiveGameCategory);

    QString ageBracket() const;
    void setageBracket(const QString &newAgeBracket);

signals:
    void userIdChanged();
    void userFullNameChanged();

    void userAgeChanged();

    void gameIdChanged();

    void activeGameIdChanged();

    void activeGameTypeIdChanged();

    void activeGameCategoryChanged();

    void ageBracketChanged();

private:
    QString m_userId;
    QString m_userFullName;
    // QString m_useAge;
    explicit UserSession(QObject *parent = nullptr);

    QString m_userAge;
    int m_gameId;
    int m_activeGameId;
    int m_activeGameTypeId;
    int m_activeGameCategory;
    QString m_ageBracket;
};

#endif // USERSESSION_H
