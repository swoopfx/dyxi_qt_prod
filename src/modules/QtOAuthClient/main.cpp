#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // CRITICAL: QtKeychain uses organization/application credentials 
    // to map separate sandbox namespaces in the credentials safe.
    app.setOrganizationName("EnterpriseCompany");
    app.setOrganizationDomain("enterprise-security.internal");
    app.setApplicationName("HeroGateSecureClient");

    QQmlApplicationEngine engine;

    // Load main.qml from the modern QML module resource path
    const QUrl url(QStringLiteral("qrc:/Enterprise/Security/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
         if (!obj && url == objUrl)
             QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}
