
#include <QGuiApplication>
#include <QQmlApplicationEngine>
// #include "CoreSettings.hpp"
#include <QIcon>
#include <QSslSocket>
#include <QDebug>
// #include <QTextToSpeech>
#include <QtQml/qqml.h>
#include <QCoreApplication>
// #include <QCameraPermission>
#include <QGuiApplication>
// #include <QMicrophonePermission>
#include <QPermission>
#include <qqmlcontext.h>
#include "authservices.h"
// #include "wardprofile.h"
#include "pagerouter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Orula Deviant Limited");
    app.setOrganizationDomain("org.qtproject.oruladyxiprod");
    app.setApplicationName("Orula Dyxi");


    // 2. Instantiate WardProfile cleanly
    // WardProfile *wardProfile = new WardProfile(&app);

    qRegisterMetaType<QVector<float>>("QVector<float>");
    // app.setApplicationName("Dyxi Mvp");
    // app.setApplicationDisplayName("Dyxi Mvp");

    // Set the application-wide icon (Used for main window and taskbar)
    // app.setWindowIcon(QIcon("qrc:/ui/UIModule/images/logo.png"));


    // Mic permision acces
    QMicrophonePermission micPermission;

    switch (app.checkPermission(micPermission)) {
    case Qt::PermissionStatus::Undetermined:
        app.requestPermission(micPermission,
                              [](const QPermission &permission) {
                                  if (permission.status() == Qt::PermissionStatus::Granted) {
                                      qDebug() << "Microphone permission granted";
                                      // Proceed with microphone usage
                                  } else {
                                      qDebug() << "Microphone permission denied";
                                  }
                              });
        break;

    case Qt::PermissionStatus::Denied:
        qDebug() << "Microphone permission denied";
        // Show instructions to enable permission in system settings
        break;

    case Qt::PermissionStatus::Granted:
        qDebug() << "Microphone permission already granted";
        // Proceed with microphone usage
        break;
    }







    QCameraPermission cameraPermission;

    switch (qApp->checkPermission(cameraPermission)) {
    case Qt::PermissionStatus::Undetermined:
        // The status is undetermined, request permission
        qApp->requestPermission(cameraPermission, [](const QPermission &permission) {
            if (permission.status() == Qt::PermissionStatus::Granted) {
                qDebug() << "Camera permission granted!";
                // TODO: Initialize camera here
            } else {
                qDebug() << "Camera permission denied!";
            }
        });
        break;
    case Qt::PermissionStatus::Granted:
        qDebug() << "Camera already granted.";
        // TODO: Initialize camera here
        break;
    case Qt::PermissionStatus::Denied:
        qDebug() << "Camera permission is denied.";
        break;
    }

    OAuth::AuthServices authservice;
    PageRouter pageRouter;


    QQmlApplicationEngine engine;

    // 3. Expose EnvConfig to QML context
    engine.rootContext()->setContextProperty("authService", &authservice);
     engine.rootContext()->setContextProperty( "pageRouter", &pageRouter );

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Dyxi_prod", "Main");

    return app.exec();
}
