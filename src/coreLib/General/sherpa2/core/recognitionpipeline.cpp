#include "recognitionpipeline.h"
#include <QDebug>
#include <QMetaObject>
#include <QMetaMethod>

RecognitionPipeline::RecognitionPipeline(
    const sherpa_onnx::VadModelConfig &vadConfig,
    const sherpa_onnx::OfflineRecognizerConfig &recognizerConfig,
    QObject *parent)
    : QObject(parent)
{
    createWorkers(vadConfig, recognizerConfig);

    createThreads();

    connectPipeline();
}

RecognitionPipeline::~RecognitionPipeline()
{
    stop();

    delete m_audioWorker;
    delete m_vadWorker;
    delete m_recognizerWorker;

    m_audioWorker = nullptr;
    m_vadWorker = nullptr;
    m_recognizerWorker = nullptr;


    destroyThreads();
}

void RecognitionPipeline::createWorkers(
    const sherpa_onnx::VadModelConfig &vadConfig,
    const sherpa_onnx::OfflineRecognizerConfig &recognizerConfig)
{
    m_audioWorker = new AudioCaptureWorker();

    m_vadWorker = new VADWorker(vadConfig);

    m_recognizerWorker =
        new OfflineRecognizerWorker(recognizerConfig);
}

void RecognitionPipeline::createThreads()
{
    m_audioWorker->moveToThread(&m_audioThread);

    m_vadWorker->moveToThread(&m_vadThread);

    m_recognizerWorker->moveToThread(&m_recognitionThread);

    // connect(&m_audioThread,
    //         &QThread::finished,
    //         m_audioWorker,
    //         &QObject::deleteLater);

    // connect(&m_vadThread,
    //         &QThread::finished,
    //         m_vadWorker,
    //         &QObject::deleteLater);

    // connect(&m_recognitionThread,
    //         &QThread::finished,
    //         m_recognizerWorker,
    //         &QObject::deleteLater);
}

void RecognitionPipeline::connectPipeline()
{
    //
    // Audio ---> VAD
    //
    connect(m_audioWorker,
            &AudioCaptureWorker::frameCaptured,
            m_vadWorker,
            &VADWorker::processFrame,
            Qt::QueuedConnection);

    //
    // VAD ---> Offline Recognizer
    //
    connect(m_vadWorker,
            &VADWorker::utteranceReady,
            m_recognizerWorker,
            &OfflineRecognizerWorker::recognize,
            Qt::QueuedConnection);

    //
    // Recognition status
    //
    connect(m_recognizerWorker,
            &OfflineRecognizerWorker::recognitionStarted,
            this,
            &RecognitionPipeline::recognitionStarted,
            Qt::QueuedConnection);

    connect(m_recognizerWorker,
            &OfflineRecognizerWorker::recognitionFinished,
            this,
            &RecognitionPipeline::transcriptionReady,
            Qt::QueuedConnection);

    connect(m_recognizerWorker,
            &OfflineRecognizerWorker::recognitionError,
            this,
            &RecognitionPipeline::recognitionError,
            Qt::QueuedConnection);

    //
    // VAD state
    //
    connect(m_vadWorker,
            &VADWorker::speechStarted,
            this,
            &RecognitionPipeline::speechStarted,
            Qt::QueuedConnection);

    connect(m_vadWorker,
            &VADWorker::speechEnded,
            this,
            &RecognitionPipeline::speechEnded,
            Qt::QueuedConnection);
}

// bool RecognitionPipeline::start()
// {
//     qDebug() << "RecognitionPipeline::start()";
//     if (m_running)
//         return true;

//     m_audioThread.start();

//     m_vadThread.start();

//     m_recognitionThread.start();
//     const QMetaObject *mo = m_audioWorker->metaObject();


//     // QMetaObject::invokeMethod(
//     //     m_audioWorker,
//     //      "start",
//     //     Qt::QueuedConnection);
//     QMetaObject::invokeMethod(
//         m_audioWorker,
//         &AudioCaptureWorker::start,
//         Qt::QueuedConnection);

//     m_running = true;

//     emit pipelineStarted();
// qDebug() << "RecognitionPipeline::stop()";
//     return true;
// }

bool RecognitionPipeline::start()
{
    if (m_running)
        return true;

    m_audioThread.start();
    m_vadThread.start();
    m_recognitionThread.start();

    m_running = true;

    emit pipelineStarted();

    return true;
}

void RecognitionPipeline::startRecording()
{
    if (!m_running)
        return;

    QMetaObject::invokeMethod(
        m_audioWorker,
        &AudioCaptureWorker::start,
        Qt::QueuedConnection);
}

bool RecognitionPipeline::initialize()
{
    if (m_running)
        return true;

    m_audioThread.start();
    m_vadThread.start();
    m_recognitionThread.start();

    m_running = true;

    return true;
}

void RecognitionPipeline::stopRecording()
{
    if (!m_running)
        return;

    QMetaObject::invokeMethod(
        m_audioWorker,
        &AudioCaptureWorker::stop,
        Qt::QueuedConnection);
}
void RecognitionPipeline::stop()
{
    if (!m_running)
        return;

    //
    // Stop microphone first
    //
    QMetaObject::invokeMethod(
        m_audioWorker,
        "stop",
        Qt::BlockingQueuedConnection);

    //
    // Flush pending speech so the last utterance
    // is recognized before shutdown.
    //
    QMetaObject::invokeMethod(
        m_vadWorker,
        "flush",
        Qt::BlockingQueuedConnection);

    //
    // Stop worker threads
    //
    m_audioThread.quit();

    m_vadThread.quit();

    m_recognitionThread.quit();

    m_audioThread.wait();

    m_vadThread.wait();

    m_recognitionThread.wait();

    m_running = false;

    emit pipelineStopped();
}

void RecognitionPipeline::destroyThreads()
{
    if (m_audioThread.isRunning())
    {
        m_audioThread.quit();
        m_audioThread.wait();
    }

    if (m_vadThread.isRunning())
    {
        m_vadThread.quit();
        m_vadThread.wait();
    }

    if (m_recognitionThread.isRunning())
    {
        m_recognitionThread.quit();
        m_recognitionThread.wait();
    }
}