#include "speechmanager.h"

#include "audiocapture.h"
#include "recognitionworker.h"

#include <QDebug>

SpeechManager::SpeechManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Creating SpeechManager";

    m_capture = new AudioCapture(this);

    m_worker = new RecognitionWorker(this);

    //
    // Give AudioCapture direct access to the worker
    //
    m_capture->setRecognitionWorker(m_worker);

    //
    // Recognition -> UI
    //
    connect(m_worker,
            &RecognitionWorker::partialResultChanged,
            this,
            &SpeechManager::partialTextReady);

    connect(m_worker,
            &RecognitionWorker::finalResultChanged,
            this,
            &SpeechManager::finalTextReady);

    connect(m_worker,
            &RecognitionWorker::errorOccurred,
            this,
            &SpeechManager::errorOccurred);

    //
    // Audio errors
    //
    connect(m_capture,
            &AudioCapture::errorOccurred,
            this,
            &SpeechManager::errorOccurred);
}

SpeechManager::~SpeechManager()
{
    stopListening();
}

void SpeechManager::startListening()
{
    qDebug() << "SpeechManager::startListening()";

    //
    // Start recognition thread first.
    //
    if (!m_worker->start())
    {
        emit errorOccurred(
            "Failed to start recognition.");
        return;
    }

    //
    // Then start microphone.
    //
    m_capture->start();
}

void SpeechManager::stopListening()
{
    qDebug() << "SpeechManager::stopListening()";

    //
    // Stop microphone first.
    //
    m_capture->stop();

    //
    // Then stop recognition thread.
    //
    m_worker->stop();
}