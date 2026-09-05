#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "backend/AppDifficulty.hpp"
#include "backend/TracingEngine.hpp"
#include "backend/MediaController.hpp"
#include "backend/TelemetryManager.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setApplicationName("LittleScribblers");
    app.setOrganizationName("LittleScribblersTeam");

    QQmlApplicationEngine engine;

    // Register C++ backend services for heavy-duty computation & media
    auto tracingEngine = std::make_unique<TracingEngine>();
    auto mediaController = std::make_unique<MediaController>();
    auto telemetryManager = std::make_unique<TelemetryManager>();

    engine.rootContext()->setContextProperty("cppTracingEngine", tracingEngine.get());
    engine.rootContext()->setContextProperty("cppMediaController", mediaController.get());
    engine.rootContext()->setContextProperty("cppTelemetryManager", telemetryManager.get());

    const QUrl url(QStringLiteral("qrc:/LittleScribblers/Core/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.load(url);

    return app.exec();
}
