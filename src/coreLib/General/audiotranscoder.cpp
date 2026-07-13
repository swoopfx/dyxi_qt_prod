#include "audiotranscoder.h"
#include <QCoreApplication>
#include <QTimer>

AudioTranscoder::AudioTranscoder(QObject *parent)
    : QObject(parent)
{
    // Configure the desired format: 16kHz, Mono, 16-bit PCM
    m_requestedFormat.setSampleRate(16000);
    m_requestedFormat.setChannelCount(1);
    m_requestedFormat.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice info = QMediaDevices::defaultAudioInput();

    // In Qt 6, QAudioSource will attempt to resample if the hardware doesn't
    // support the requested format directly, provided the backend supports it.
    if (!info.isFormatSupported(m_requestedFormat)) {
        qWarning() << "Requested format (16kHz Mono Int16) not natively supported by hardware.";
        qWarning() << "Qt will attempt to resample if possible.";
    }

    m_audioSource = new QAudioSource(info, m_requestedFormat, this);
    connect(m_audioSource, &QAudioSource::stateChanged, this, &AudioTranscoder::handleStateChanged);

    m_outputFile.setFileName("output_16khz.pcm");
}

AudioTranscoder::~AudioTranscoder()
{
    stop();
}

void AudioTranscoder::start()
{
    if (m_outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Starting capture to" << m_outputFile.fileName();
        qDebug() << "Requested Format:" << m_requestedFormat.sampleRate() << "Hz,"
                 << m_requestedFormat.channelCount() << "channels";

        m_inputDevice = m_audioSource->start();

        if (m_inputDevice) {
            connect(m_inputDevice, &QIODevice::readyRead, this, &AudioTranscoder::readAudio);
        } else {
            qCritical() << "Failed to start audio source:" << m_audioSource->error();
        }
    } else {
        qCritical() << "Could not open output file for writing.";
    }
}

void AudioTranscoder::stop()
{
    if (m_audioSource) {
        m_audioSource->stop();
    }
    if (m_outputFile.isOpen()) {
        m_outputFile.close();
    }
    qDebug() << "Capture stopped.";
}

void AudioTranscoder::readAudio()
{
    if (!m_inputDevice) return;

    QByteArray data = m_inputDevice->readAll();
    if (!data.isEmpty()) {
        m_outputFile.write(data);
        // Data is now in the requested 16kHz PCM format (transcoded by Qt if needed)
    }
}

void AudioTranscoder::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
    case QAudio::StoppedState:
        if (m_audioSource->error() != QAudio::NoError) {
            qCritical() << "Audio Source Error:" << m_audioSource->error();
        }
        break;
    case QAudio::ActiveState:
        qDebug() << "Audio Source is now active.";
        break;
    default:
        break;
    }
}