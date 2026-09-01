#include "mainviewmodel.h"
#include <QDebug>

MainViewModel::MainViewModel(
    RecognitionPipeline *pipeline,
    QObject *parent)
    : QObject(parent),
    m_pipeline(pipeline)
{
    connect(m_pipeline,
            &RecognitionPipeline::transcriptionReady,
            this,
            &MainViewModel::onTranscriptionReady);

    connect(m_pipeline,
            &RecognitionPipeline::speechStarted,
            this,
            &MainViewModel::onSpeechStarted);

    connect(m_pipeline,
            &RecognitionPipeline::speechEnded,
            this,
            &MainViewModel::onSpeechEnded);

    connect(m_pipeline,
            &RecognitionPipeline::recognitionError,
            this,
            &MainViewModel::onRecognitionError);
}

QString MainViewModel::history() const
{
    qDebug() <<  m_history;
    return m_history;
}

bool MainViewModel::listening() const
{
    return m_listening;
}

QString MainViewModel::lastError() const
{
    return m_lastError;
}

void MainViewModel::start()
{
    if (m_pipeline)
        // m_pipeline->start();
        m_pipeline->startRecording();
}

void MainViewModel::stop()
{
    if (m_pipeline)
        m_pipeline->stopRecording();
        // m_pipeline->stop();
}

void MainViewModel::clearHistory()
{
    if (m_history.isEmpty())
        return;

    m_history.clear();

    emit historyChanged();
}

void MainViewModel::onTranscriptionReady(const QString &text)
{
    if (text.trimmed().isEmpty())
        return;

    if (!m_history.isEmpty())
        m_history.append("\n");

    m_history.append(text);

    emit historyChanged();
}

void MainViewModel::onSpeechStarted()
{
    if (m_listening)
        return;

    m_listening = true;

    emit listeningChanged();
}

void MainViewModel::onSpeechEnded()
{
    if (!m_listening)
        return;

    m_listening = false;

    emit listeningChanged();
}

void MainViewModel::onRecognitionError(const QString &error)
{
    m_lastError = error;

    emit lastErrorChanged();
}