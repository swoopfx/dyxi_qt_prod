#include "vadworker.h"

#include <QMutexLocker>

VADWorker::VADWorker(
    const sherpa_onnx::VadModelConfig &config,
    QObject *parent)
    : QObject(parent)
{
    if (!config.Validate())
    {
        qFatal("Invalid VAD configuration.");
    }

    m_vad =
        std::make_unique<sherpa_onnx::VoiceActivityDetector>(
            config,
            60.0f);        // 60-second internal buffer
}

VADWorker::~VADWorker() = default;

void VADWorker::reset()
{
    QMutexLocker locker(&m_mutex);

    if (!m_vad)
        return;

    m_vad->Reset();

    m_inSpeech = false;
}

void VADWorker::flush()
{
    QMutexLocker locker(&m_mutex);

    if (!m_vad)
        return;

    m_vad->Flush();

    while (!m_vad->Empty())
    {
        const auto &segment = m_vad->Front();

        if (m_inSpeech)
            emit speechEnded();

        emit utteranceReady(segment);

        m_vad->Pop();

        m_inSpeech = false;
    }
}

void VADWorker::processFrame(const std::vector<float> &frame)
{
    if (!m_vad || frame.empty())
        return;

    QMutexLocker locker(&m_mutex);

    try
    {
        m_vad->AcceptWaveform(
            frame.data(),
            static_cast<int32_t>(frame.size()));

        if (!m_inSpeech &&
            m_vad->IsSpeechDetected())
        {
            m_inSpeech = true;

            emit speechStarted();
        }

        while (!m_vad->Empty())
        {
            const auto &segment = m_vad->Front();

            if (m_inSpeech)
                emit speechEnded();

            emit utteranceReady(segment);

            m_vad->Pop();

            m_inSpeech = false;
        }
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString::fromUtf8(e.what()));
    }
}