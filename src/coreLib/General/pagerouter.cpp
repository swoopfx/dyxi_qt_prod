
#include "pagerouter.h"

#include <QDebug>
#include <QMetaType>


PageRouter::PageRouter(QObject *parent)
    : QObject(parent)
    , m_authServices(new OAuth::AuthServices(this))
    , m_wardProfile(new WardProfile(this))
    , m_initialized(false)
    , m_routing(false)
    , m_waitingForAuth(false)
{
    /*
     * Authentication state changed.
     *
     * This is particularly important during application
     * startup because AuthServices::checkSavedSession()
     * is asynchronous.
     */
    connect(
        m_authServices,
        &OAuth::AuthServices::authStateChanged,
        this,
        &PageRouter::onAuthStateChanged
        );


    connect(
        m_authServices,
        &OAuth::AuthServices::loginSuccess,
        this,
        &PageRouter::onLoginSuccess
        );


    connect(
        m_authServices,
        &OAuth::AuthServices::logoutCompleted,
        this,
        &PageRouter::onLogoutCompleted
        );
}


// ============================================================
// Properties
// ============================================================

bool PageRouter::isInitialized() const
{
    return m_initialized;
}


bool PageRouter::isRouting() const
{
    return m_routing;
}


QString PageRouter::currentPage() const
{
    return m_currentPage;
}


// ============================================================
// Initialization
// ============================================================

void PageRouter::initialize()
{
    if (m_initialized)
        return;


    qDebug()
        << "[PageRouter] Starting initialization.";


    m_waitingForAuth = true;


    /*
     * AuthServices will:
     *
     * 1. Read access token from QtKeychain.
     * 2. Validate the token against /me.
     * 3. Refresh if necessary.
     * 4. Eventually emit authStateChanged().
     */
    m_authServices->checkSavedSession();


    /*
     * DO NOT call route() here.
     *
     * Authentication has not finished yet.
     */
}


// ============================================================
// Authentication state
// ============================================================

void PageRouter::onAuthStateChanged()
{
    qDebug()
    << "[PageRouter] authStateChanged:"
    << m_authServices->isSignedIn();


    if (m_waitingForAuth) {

        m_waitingForAuth = false;

        m_initialized = true;

        emit initializedChanged();
    }


    route();
}


// ============================================================
// Login
// ============================================================

void PageRouter::onLoginSuccess()
{
    qDebug()
    << "[PageRouter] Login successful.";

    route();
}


// ============================================================
// Logout
// ============================================================

void PageRouter::onLogoutCompleted()
{
    qDebug()
    << "[PageRouter] Logout completed.";

    setCurrentPage(
        pageUrl(Destination::Welcome)
        );
}


// ============================================================
// Determine destination
// ============================================================

PageRouter::Destination
PageRouter::determineDestination() const
{
    /*
     * --------------------------------------------------------
     * USER IS NOT LOGGED IN
     * --------------------------------------------------------
     */

    if (!m_authServices->isSignedIn()) {

        qDebug()
        << "[PageRouter]"
        << "User not logged in -> WelcomePage";

        return Destination::Welcome;
    }


    /*
     * --------------------------------------------------------
     * USER IS LOGGED IN
     * --------------------------------------------------------
     */

    qDebug()
        << "[PageRouter]"
        << "User is logged in.";


    /*
     * --------------------------------------------------------
     * NO WARD PROFILE
     * --------------------------------------------------------
     */

    if (!m_wardProfile->isProfileStored()) {

        qDebug()
        << "[PageRouter]"
        << "No WardProfile -> Tiles";

        return Destination::Tiles;
    }


    /*
     * --------------------------------------------------------
     * READ WARD PROFILE
     * --------------------------------------------------------
     */

    const QMap<QString, QVariant> profile =
        m_wardProfile->readProfile();


    if (profile.isEmpty()) {

        qWarning()
        << "[PageRouter]"
        << "WardProfile is empty -> Tiles";

        return Destination::Tiles;
    }


    /*
     * --------------------------------------------------------
     * CHECK ACTIVATION
     * --------------------------------------------------------
     */

    if (!m_wardProfile->isProfileActivated()) {

        qDebug()
        << "[PageRouter]"
        << "WardProfile not activated -> Tiles";

        return Destination::Tiles;
    }


    /*
     * --------------------------------------------------------
     * WARD IS ACTIVE
     * --------------------------------------------------------
     */

    qDebug()
        << "[PageRouter]"
        << "WardProfile is active.";


    /*
     * --------------------------------------------------------
     * CHECK CURRICULUM
     * --------------------------------------------------------
     */

    if (!hasCurriculumId(profile)) {

        qDebug()
        << "[PageRouter]"
        << "No curriculum ID -> StartCurriculum";

        return Destination::StartCurriculum;
    }


    /*
     * --------------------------------------------------------
     * EVERYTHING IS READY
     * --------------------------------------------------------
     */

    qDebug()
        << "[PageRouter]"
        << "Curriculum available -> Curriculum";

    return Destination::Curriculum;
}


// ============================================================
// Curriculum validation
// ============================================================

bool PageRouter::hasCurriculumId(
    const QMap<QString, QVariant> &profile) const
{
    constexpr const char *CurriculumKey =
        "curriculum_id";


    if (!profile.contains(
            QString::fromLatin1(CurriculumKey))) {

        return false;
    }


    const QVariant value =
        profile.value(
            QString::fromLatin1(CurriculumKey)
            );


    if (!value.isValid() || value.isNull())
        return false;


    /*
     * String curriculum ID.
     */
    if (value.typeId() == QMetaType::QString) {

        return !value.toString()
        .trimmed()
            .isEmpty();
    }


    /*
     * Integer curriculum ID.
     */
    if (value.canConvert<int>()) {

        return value.toInt() > 0;
    }


    return false;
}


// ============================================================
// Page URLs
// ============================================================

QString PageRouter::pageUrl(
    Destination destination) const
{
    switch (destination) {

    case Destination::Welcome:

        return QStringLiteral(
            "qrc:/ui/UIModule/Pages/WelcomePage.qml"
            );


    case Destination::Tiles:

        return QStringLiteral(
            "qrc:/ui/UIModule/Pages/Tiles.qml"
            );


    case Destination::Curriculum:

        return QStringLiteral(
            "qrc:/ui/UIModule/Pages/Curriculum.qml"
            );


    case Destination::StartCurriculum:

        return QStringLiteral(
            "qrc:/ui/UIModule/Pages/StartCurriculum.qml"
            );
    }


    return QStringLiteral(
        "qrc:/ui/UIModule/Pages/WelcomePage.qml"
        );
}


// ============================================================
// Route
// ============================================================

void PageRouter::route()
{
    if (m_routing)
        return;


    /*
     * Do not route until session restoration has finished.
     */
    if (m_waitingForAuth)
        return;


    m_routing = true;

    emit routingChanged();
    emit routeStarted();


    const Destination destination =
        determineDestination();


    const QString url =
        pageUrl(destination);


    qDebug()
        << "[PageRouter] Routing to:"
        << url;


    setCurrentPage(url);


    m_routing = false;

    emit routingChanged();
    emit routeFinished();
}


// ============================================================
// Set current page
// ============================================================

void PageRouter::setCurrentPage(
    const QString &page)
{
    if (m_currentPage == page)
        return;


    m_currentPage = page;

    emit currentPageChanged();
}


// ============================================================
// Refresh
// ============================================================

void PageRouter::refreshRoute()
{
    route();
}


// ============================================================
// Ward profile changed
// ============================================================

void PageRouter::routeAfterWardProfileChanged()
{
    qDebug()
    << "[PageRouter]"
    << "WardProfile changed.";

    route();
}
