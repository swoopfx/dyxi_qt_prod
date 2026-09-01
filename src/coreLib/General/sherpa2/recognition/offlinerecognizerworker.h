#pragma once

#include <QObject>
#include <QMutex>
#include <memory>

#include "sherpa-onnx/csrc/offline-recognizer.h"
#include "sherpa-onnx/csrc/voice-activity-detector.h"

// Q_DECLARE_METATYPE(sherpa_onnx::SpeechSegment)

class OfflineRecognizerWorker : public QObject
{
    Q_OBJECT

public:
    explicit OfflineRecognizerWorker(
        const sherpa_onnx::OfflineRecognizerConfig &config,
        QObject *parent = nullptr);

    ~OfflineRecognizerWorker();

public slots:

    // Called when the VAD has produced one complete utterance
    void recognize(const sherpa_onnx::SpeechSegment &segment);

signals:

    void recognitionStarted();

    void recognitionFinished(const QString &text);

    void recognitionError(const QString &message);

private:

    std::unique_ptr<sherpa_onnx::OfflineRecognizer> m_recognizer;

    QMutex m_mutex;

    static constexpr int SampleRate = 16000;
};