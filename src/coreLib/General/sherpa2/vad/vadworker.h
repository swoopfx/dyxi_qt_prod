#pragma once

#include <QObject>
#include <QMutex>
#include <memory>
#include <vector>

#include "sherpa-onnx/csrc/voice-activity-detector.h"

Q_DECLARE_METATYPE(sherpa_onnx::SpeechSegment)

class VADWorker : public QObject
{
    Q_OBJECT

public:
    explicit VADWorker(
        const sherpa_onnx::VadModelConfig &config,
        QObject *parent = nullptr);

    ~VADWorker();

public slots:

    void processFrame(const std::vector<float> &frame);

    void flush();

    void reset();

signals:

    void speechStarted();

    void speechEnded();

    void utteranceReady(
        const sherpa_onnx::SpeechSegment &segment);

    void errorOccurred(const QString &message);

private:

    std::unique_ptr<sherpa_onnx::VoiceActivityDetector> m_vad;

    bool m_inSpeech = false;

    QMutex m_mutex;
};