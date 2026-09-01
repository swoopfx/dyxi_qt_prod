#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "audio_synth.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Dyxi");
    app.setApplicationName("DyxiPhonics");

    // Construct the procedural multi-sensory sound synthesizer
    AudioSynth audioSynth;

    QQmlApplicationEngine engine;
    
    // Inject synthesizer backend context to QML runtime
    engine.rootContext()->setContextProperty("audioSynth", &audioSynth);

    // Qt 6 style path pointing to the registered module target
    const QUrl url(QStringLiteral("qrc:/qt/qml/DyxiPhonics/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}