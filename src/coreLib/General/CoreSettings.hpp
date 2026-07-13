#ifndef CORESETTINGS_HPP
#define CORESETTINGS_HPP

#include <QObject>
#include <QQmlEngine>

class CoreSettings : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString dyxiGameTypeIds READ dyxiGameTypeIds FINAL)
    Q_PROPERTY(QString dyxiGameByParam READ dyxiGameByParam FINAL)
    Q_PROPERTY(QString dyxiRecognitionPostShapeExplorer READ dyxiRecognitionPostShapeExplorer FINAL)
    Q_PROPERTY(QString  dyxiGetGameList READ  dyxiGetGameList FINAL)
    Q_PROPERTY(QString dyxiGetGameCategoryList READ dyxiGetGameCategoryList  FINAL)
    Q_PROPERTY(QString dyxiGetGameProgramsList READ dyxiGetGameProgramsList  FINAL)
    Q_PROPERTY(QString recogShapeRecogPostUrl READ recogShapeRecogPostUrl FINAL)

public:
    // explicit MyObject(QObject *parent = nullptr) : QObject(parent)/*, m_myText("Initial Value") {}*/
    explicit CoreSettings(QObject *parent = nullptr) {}
    virtual ~CoreSettings() {}
    // Getter
    QString myText() const { return m_myText; }
    QString dyxiGameTypeIds(){
        return m_dyxiGameTypeIds;
    }
    QString dyxiGameByParam(){
        return m_dyxiGameByParam;
    }

    QString dyxiRecognitionPostShapeExplorer(){
        return m_dyxiRecognitionPostShapeExplorer;

    }
    QString dyxiGetGameCategoryList()
    {
        return m_dyxiGetGameCategoryList;
    }


 QString dyxiGetGameList()
    {
        return m_dyxiGetGameList;
    }

QString dyxiGetGameProgramsList()
    {
        return m_dyxiGetGameProgramsList;
    }


  QString recogShapeRecogPostUrl()
    {
        return m_recogShapeRecogPostUrl;
    }

signals:
    void myTextChanged();




public:
   QString appName=QStringLiteral("MVP Dyxi");
   QString companyName = QStringLiteral("Orula Deviant");
    QString organisationDomain = QStringLiteral("https://dyxi.site");





    // Endpoints
    QString baseUrl=QStringLiteral("https://mvp.dyxi.site");
    const QString dyxiApiLogin = baseUrl+QStringLiteral("/api/login");
    const QString dyxiApiregistration = baseUrl+ QStringLiteral("/api/register");

    const QString dyxiApiGetStuentDetails = baseUrl+ QStringLiteral("/api/get-student-details");

    QString m_dyxiGameByParam = baseUrl+QStringLiteral("/api/game-types");

    QString m_dyxiGameTypeIds = baseUrl + QStringLiteral("/api/game-type-id");
    const QString dyxiGameCategoryIds = baseUrl+QStringLiteral("/api/game-category-id");
    const QString dyxiGameProgramIds = baseUrl+QStringLiteral("/api/game-programs-id");

    // Pages
    // all pages should be relative to the Page/SelectPage.qml
    // beter still move the SelectPage to the root page
    static inline const QString selectGamePage =QStringLiteral("SelectGameCategoryPage");

    // Session Identifier
    static inline const QString activeUserName =QStringLiteral("active_user_name");
    static inline const QString activeUserId = QStringLiteral("active_user_id");
    static inline const QString activeUserUuid = QStringLiteral("active_user_uuid");
    static inline const QString activeUserLanguage = QStringLiteral("active_user_language");

    // Post Endpoint




    QString dyxiGetGameList() const;



    QString dyxiGetGameProgramsList() const;

    QString recogShapeRecogPostUrl() const;

private:
    QString m_myText;
    QString m_dyxiRecognitionPostShapeExplorer = baseUrl + QStringLiteral("/api-post/recognition-shared-explorer");

    QString m_dyxiGetGameList= baseUrl+QStringLiteral("/api/game-types");
    QString m_dyxiGetGameCategoryList=baseUrl+QStringLiteral("/api/game-category/");
    QString m_dyxiGetGameProgramsList = baseUrl+QStringLiteral("/api/game-programs");
    QString m_recogShapeRecogPostUrl=baseUrl+ QStringLiteral("/api-post/recog-shape-recog");
};













#endif // CORESETTINGS_HPP
