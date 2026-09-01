#include "audiocapture.h"

#include <QMediaDevices>
#include <QAudioDevice>
#include <QDebug>

AudioCaptureWorker::AudioCaptureWorker(QObject *parent)
    : QObject(parent)
{
}

AudioCaptureWorker::~AudioCaptureWorker()
{
    qDebug() << "~AudioCaptureWorker";
    stop();
}

bool AudioCaptureWorker::initializeAudio()
{
    auto input = QMediaDevices::defaultAudioInput();

    if (input.isNull())
    {
        emit errorOccurred("No microphone found.");
        return false;
    }

    m_format.setSampleRate(SampleRate);
    m_format.setChannelCount(Channels);
    m_format.setSampleFormat(QAudioFormat::Float);

    if (!input.isFormatSupported(m_format))
    {
        emit errorOccurred(
            "Microphone does not support 16 kHz mono float.");
        return false;
    }

    m_audioSource =
        std::make_unique<QAudioSource>(input, m_format);

    m_audioSource->setBufferSize(FrameSamples * sizeof(float) * 4);

    return true;
}

void AudioCaptureWorker::start()
{
    qDebug() << "AudioCapture::start()";
    QMutexLocker locker(&m_mutex);

    if (m_running)
        return;

    if (!initializeAudio())
        return;

    m_device = m_audioSource->start();

    if (!m_device)
    {
        emit errorOccurred("Unable to start microphone.");
        return;
    }

    connect(m_device,
            &QIODevice::readyRead,
            this,
            &AudioCaptureWorker::readMore);

    m_running = true;
    qDebug() << "QAudioSource started";
    emit started();
}

void AudioCaptureWorker::stop()
{
    QMutexLocker locker(&m_mutex);

    if (!m_running)
        return;

    if (m_audioSource)
    {
        m_audioSource->stop();
    }

    m_byteBuffer.clear();
    m_sampleBuffer.clear();

    m_audioSource.reset();

    m_running = false;

    emit stopped();
}

void AudioCaptureWorker::readMore()
{

    qDebug() << "Received audio from microphone";
    if (!m_device)
        return;

    QByteArray bytes = m_device->readAll();

    if (bytes.isEmpty())
        return;

    m_byteBuffer.append(bytes);

    const int bytesPerSample = sizeof(float);

    while (m_byteBuffer.size() >= bytesPerSample)
    {
        float sample;

        memcpy(&sample,
               m_byteBuffer.constData(),
               bytesPerSample);

        m_sampleBuffer.push_back(sample);

        m_byteBuffer.remove(0, bytesPerSample);

        if (m_sampleBuffer.size() == FrameSamples)
        {
            emit frameCaptured(m_sampleBuffer);

            m_sampleBuffer.clear();
        }
    }
}