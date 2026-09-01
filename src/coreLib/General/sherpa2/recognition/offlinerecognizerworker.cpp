#include "offlinerecognizerworker.h"

#include <QMutexLocker>

OfflineRecognizerWorker::OfflineRecognizerWorker(
    const sherpa_onnx::OfflineRecognizerConfig &config,
    QObject *parent)
    : QObject(parent)
{
    if (!config.Validate())
    {
        throw std::runtime_error("Invalid OfflineRecognizer configuration.");
    }

    m_recognizer =
        std::make_unique<sherpa_onnx::OfflineRecognizer>(config);
}

OfflineRecognizerWorker::~OfflineRecognizerWorker() = default;

void OfflineRecognizerWorker::recognize(
    const sherpa_onnx::SpeechSegment &segment)
{
    if (!m_recognizer || segment.samples.empty())
        return;

    emit recognitionStarted();

    try
    {
        QMutexLocker locker(&m_mutex);

        // Create a new stream for this utterance
        auto stream = m_recognizer->CreateStream();

        // Feed the complete utterance
        stream->AcceptWaveform(
            SampleRate,
            segment.samples.data(),
            static_cast<int32_t>(segment.samples.size()));

        // Decode
        m_recognizer->DecodeStream(stream.get());

        // Get the recognition result from the stream
        const sherpa_onnx::OfflineRecognitionResult &result =
            stream->GetResult();

        locker.unlock();

        emit recognitionFinished(
            QString::fromStdString(result.text));
    }
    catch (const std::exception &e)
    {
        emit recognitionError(
            QString::fromStdString(e.what()));
    }
}