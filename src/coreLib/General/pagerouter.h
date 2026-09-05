
#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>

#include "authservices.h"
#include "wardprofile.h"

    class PageRouter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        bool initialized
            READ isInitialized
                NOTIFY initializedChanged
        )

    Q_PROPERTY(
        bool routing
            READ isRouting
                NOTIFY routingChanged
        )

    Q_PROPERTY(
        QString currentPage
            READ currentPage
                NOTIFY currentPageChanged
        )

public:
    explicit PageRouter(QObject *parent = nullptr);

    bool isInitialized() const;
    bool isRouting() const;

    QString currentPage() const;

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void route();
    Q_INVOKABLE void refreshRoute();

    Q_INVOKABLE void routeAfterWardProfileChanged();

signals:
    void initializedChanged();
    void routingChanged();
    void currentPageChanged();

    void routeStarted();
    void routeFinished();

    void routingError(const QString &message);

private slots:
    void onAuthStateChanged();
    void onLoginSuccess();
    void onLogoutCompleted();

private:
    enum class Destination {
        Welcome,
        Tiles,
        Curriculum,
        StartCurriculum
    };

    Destination determineDestination() const;

    QString pageUrl(
        Destination destination) const;

    bool hasCurriculumId(
        const QMap<QString, QVariant> &profile) const;

    void setCurrentPage(
        const QString &page);

private:
    OAuth::AuthServices *m_authServices;
    WardProfile *m_wardProfile;

    QString m_currentPage;

    bool m_initialized;
    bool m_routing;
    bool m_waitingForAuth;
};

