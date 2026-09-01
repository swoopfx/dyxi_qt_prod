#pragma once

#include <QObject>
#include <QString>
#include "core/recognitionpipeline.h"
class RecognitionPipeline;

class MainViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString history
                   READ history
                       NOTIFY historyChanged)

    Q_PROPERTY(bool listening
                   READ listening
                       NOTIFY listeningChanged)

    Q_PROPERTY(QString lastError
                   READ lastError
                       NOTIFY lastErrorChanged)

public:
    explicit MainViewModel(
        RecognitionPipeline *pipeline,
        QObject *parent = nullptr);

    QString history() const;
    bool listening() const;
    QString lastError() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void clearHistory();

signals:

    void historyChanged();

    void listeningChanged();

    void lastErrorChanged();

private slots:

    void onTranscriptionReady(const QString &text);

    void onSpeechStarted();

    void onSpeechEnded();

    void onRecognitionError(const QString &error);

private:

    RecognitionPipeline *m_pipeline = nullptr;

    QString m_history;

    QString m_lastError;

    bool m_listening = false;
};