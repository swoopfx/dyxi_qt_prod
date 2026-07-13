#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include <QObject>
#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QIODevice>
#include <QMutex>

class RecognitionWorker;

class AudioCapture : public QObject
{
    Q_OBJECT

public:
    explicit AudioCapture(QObject *parent =nullptr);
    ~AudioCapture();

    bool initialize();

    void start();
    void stop();

    bool isRecording() const
    {
        return m_recording;
    }

    void setRecognitionWorker(RecognitionWorker *worker);

signals:

    void recordingChanged(bool recording);

    void errorOccurred(const QString &message);

private slots:

    void readMore();

private:

    bool setupFormat();

private:

    static constexpr int TargetSampleRate = 16000;
    static constexpr int BufferMilliseconds = 100;

    RecognitionWorker *m_worker = nullptr;

    QAudioFormat m_format;

    QAudioSource *m_audioSource = nullptr;

    QIODevice *m_audioDevice = nullptr;

    bool m_recording = false;

    mutable QMutex m_mutex;
};

#endif