#pragma once

#include <QObject>
#include <QAudioSource>
#include <QAudioFormat>
#include <QIODevice>
#include <QByteArray>
#include <QMutex>
#include <vector>

class AudioCaptureWorker : public QObject
{
    Q_OBJECT

public:
    explicit AudioCaptureWorker(QObject *parent = nullptr);
    ~AudioCaptureWorker();

public slots:
    void start();
    void stop();

signals:
    // 20 ms PCM frame
    void frameCaptured(const std::vector<float> &frame);

    void started();
    void stopped();

    void errorOccurred(const QString &error);

private slots:
    void readMore();

private:
    bool initializeAudio();

private:
    static constexpr int SampleRate = 16000;
    static constexpr int Channels = 1;

    // 20 ms @16 kHz
    static constexpr int FrameSamples = 320;

    QAudioFormat m_format;

    std::unique_ptr<QAudioSource> m_audioSource;

    QIODevice *m_device = nullptr;

    QByteArray m_byteBuffer;

    std::vector<float> m_sampleBuffer;

    bool m_running = false;

    QMutex m_mutex;
};