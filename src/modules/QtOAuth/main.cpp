#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

#include "src/envconfig.h"
#include "src/keychainhelper.h"
#include "src/authservice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Organization metadata (Used by OS Keychain to isolate credentials)
    app.setOrganizationName("EnterpriseCompany");
    app.setOrganizationDomain("enterprise-security.internal");
    app.setApplicationName("HeroGateSecureClient");
    app.setApplicationVersion("1.0.0");

    qInfo() << "🚀 [Bootstrap] Starting HeroGate Qt 6.11 Secure Client...";

    QQmlApplicationEngine engine;

    // Load initial environment configuration
    EnvConfig::instance().loadConfig();

    const QUrl url(QStringLiteral("qrc:/Enterprise/Security/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
         if (!obj && url == objUrl) {
             qCritical() << "❌ [Fatal] Failed to load QML root object from" << url;
             QCoreApplication::exit(-1);
         }
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
