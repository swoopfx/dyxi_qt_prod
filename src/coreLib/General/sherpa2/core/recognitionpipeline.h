#pragma once

#include <QObject>
#include <QThread>
#include <memory>

#include "../audio/audiocapture.h"
#include "../vad/vadworker.h"
#include "../recognition/offlinerecognizerworker.h"

class RecognitionPipeline : public QObject
{
    Q_OBJECT

public:
    RecognitionPipeline(
        const sherpa_onnx::VadModelConfig &vadConfig,
        const sherpa_onnx::OfflineRecognizerConfig &recognizerConfig,
        QObject *parent = nullptr);

    ~RecognitionPipeline();

    bool start();
    void stop();

    void startRecording();
    void stopRecording();
    bool initialize();

signals:
    void pipelineStarted();
    void pipelineStopped();

    void speechStarted();
    void speechEnded();

    void transcriptionReady(const QString &text);

    void recognitionStarted();
    void recognitionError(const QString &message);

private:
    void createWorkers(
        const sherpa_onnx::VadModelConfig &vadConfig,
        const sherpa_onnx::OfflineRecognizerConfig &recognizerConfig);

    void createThreads();

    void connectPipeline();

    void destroyThreads();

private:

    AudioCaptureWorker *m_audioWorker = nullptr;
    VADWorker *m_vadWorker = nullptr;
    OfflineRecognizerWorker *m_recognizerWorker = nullptr;

    QThread m_audioThread;
    QThread m_vadThread;
    QThread m_recognitionThread;

    bool m_running = false;
};