#include "audiocapture.h"
#include "recognitionworker.h"

#include <QDebug>

AudioCapture::AudioCapture(QObject *parent)
    : QObject(parent)
{
}

AudioCapture::~AudioCapture()
{
    stop();

    delete m_audioSource;
}

void AudioCapture::setRecognitionWorker(
    RecognitionWorker *worker)
{
    m_worker = worker;
}

bool AudioCapture::initialize()
{
    QMutexLocker locker(&m_mutex);

    if (m_recording)
    {
        locker.unlock();
        stop();
        locker.relock();
    }

    delete m_audioSource;
    m_audioSource = nullptr;

    if (!setupFormat())
    {
        emit errorOccurred(
            "Unsupported audio format.");

        return false;
    }

    auto device =
        QMediaDevices::defaultAudioInput();

    if (device.isNull())
    {
        emit errorOccurred(
            "No microphone.");

        return false;
    }

    m_audioSource =
        new QAudioSource(
            device,
            m_format,
            this);

    int bufferBytes =
        m_format.sampleRate()
        * sizeof(int16_t)
        * BufferMilliseconds
        /1000;

    m_audioSource->setBufferSize(bufferBytes);

    connect(
        m_audioSource,
        &QAudioSource::stateChanged,
        this,
        [this](QtAudio::State state)
        {
            if(state ==
                    QtAudio::StoppedState &&
                m_audioSource->error()!=QtAudio::NoError)
            {
                emit errorOccurred(
                    QStringLiteral(
                        "Audio Error"));
            }
        });

    return true;
}


void AudioCapture::start()
{
    QMutexLocker locker(&m_mutex);

    if (m_recording)
        return;

    if (!m_audioSource)
    {
        locker.unlock();

        if (!initialize())
            return;

        locker.relock();
    }

    m_audioDevice =
        m_audioSource->start();

    if (!m_audioDevice)
    {
        emit errorOccurred(
            "Cannot open microphone.");

        return;
    }

    connect(
        m_audioDevice,
        &QIODevice::readyRead,
        this,
        &AudioCapture::readMore);

    m_recording = true;

    emit recordingChanged(true);

    qDebug()
        << "Audio started.";
}


void AudioCapture::stop()
{
    QMutexLocker locker(&m_mutex);

    if (!m_recording)
        return;

    if (m_audioSource)
        m_audioSource->stop();

    m_audioDevice = nullptr;

    m_recording = false;

    emit recordingChanged(false);

    qDebug()
        << "Audio stopped.";
}


void AudioCapture::readMore()
{
    if (!m_audioDevice)
        return;

    QByteArray pcm =
        m_audioDevice->readAll();

    if (pcm.isEmpty())
        return;

    const int sampleCount =
        pcm.size()/sizeof(int16_t);

    QVector<float> samples(sampleCount);

    const int16_t *pcmData =
        reinterpret_cast<
            const int16_t*>(
            pcm.constData());

    constexpr float scale =
        1.0f/32768.0f;

    for(int i=0;i<sampleCount;i++)
    {
        samples[i]=
            pcmData[i]*scale;
    }

    if(m_worker)
    {
        m_worker->enqueueSamples(samples);
    }
}

bool AudioCapture::setupFormat()
{
    auto device =
        QMediaDevices::defaultAudioInput();

    if(device.isNull())
        return false;

    m_format.setSampleRate(16000);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(
        QAudioFormat::Int16);

    if(!device.isFormatSupported(m_format))
    {
        m_format =
            device.preferredFormat();

        m_format.setSampleRate(16000);
        m_format.setChannelCount(1);
        m_format.setSampleFormat(
            QAudioFormat::Int16);
    }

    return true;
}