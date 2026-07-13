#ifndef AUDIOTRANSCODER_H
#define AUDIOTRANSCODER_H

#include <QObject>
#include <QQmlEngine>
#include <QAudioSource>
#include <QAudioFormat>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QIODevice>
#include <QDebug>
#include <QFile>
#include <QAudio>

class AudioTranscoder : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AudioTranscoder(QObject *parent = nullptr);
    ~AudioTranscoder();

    void start();
    void stop();

private slots:
    void handleStateChanged(QAudio::State newState);
    void readAudio();

private:
    QAudioSource *m_audioSource = nullptr;
    QIODevice *m_inputDevice = nullptr;
    QFile m_outputFile;
    QAudioFormat m_requestedFormat;
};

#endif // AUDIOTRANSCODER_H


// AudioCapture *audio = new AudioCapture(this);

// QThread *workerThread = new QThread;
// WhisperWorker *worker = new WhisperWorker();

// worker->moveToThread(workerThread);

// connect(workerThread, &QThread::started,
//         worker, &WhisperWorker::start);

// connect(audio, &AudioCapture::audioChunkReady,
//         worker, &WhisperWorker::pushAudio);

// connect(worker, &WhisperWorker::textReady,
//         this, [](QString text){
//             qDebug() << "TRANSCRIPT:" << text;
//         });

// workerThread->start();
// audio->start();









// #include "WhisperWorker.h"

// WhisperWorker::WhisperWorker(QObject *parent)
//     : QObject(parent) {}

// WhisperWorker::~WhisperWorker() {
//     if (ctx) {
//         whisper_free(ctx);
//     }
// }

// void WhisperWorker::start() {
//     whisper_context_params cparams = whisper_context_default_params();
//     ctx = whisper_init_from_file_with_params("models/ggml-base.en.bin", cparams);

//     running = true;

//     while (running) {
//         processBuffer();
//         QThread::msleep(50);
//     }
// }

// void WhisperWorker::stop() {
//     running = false;
// }

// void WhisperWorker::pushAudio(QByteArray data) {
//     QMutexLocker locker(&mutex);
//     queue.enqueue(data);
// }

// void WhisperWorker::processBuffer() {
//     QByteArray data;

//     {
//         QMutexLocker locker(&mutex);
//         if (queue.isEmpty()) return;
//         data = queue.dequeue();
//     }

//     if (!ctx) return;

//     // convert int16 PCM → float
//     const int16_t *samples = reinterpret_cast<const int16_t*>(data.constData());
//     int n = data.size() / sizeof(int16_t);

//     std::vector<float> pcm(n);
//     for (int i = 0; i < n; i++) {
//         pcm[i] = samples[i] / 32768.0f;
//     }

//     whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
//     params.print_progress = false;
//     params.print_realtime = false;
//     params.print_timestamps = false;

//     if (whisper_full(ctx, params, pcm.data(), pcm.size()) == 0) {
//         QString result;

//         int n_segments = whisper_full_n_segments(ctx);
//         for (int i = 0; i < n_segments; i++) {
//             result += whisper_full_get_segment_text(ctx, i);
//         }

//         emit textReady(result);
//     }
// }










// #pragma once

// #include <QObject>
// #include <QAudioSource>
// #include <QMediaDevices>
// #include <QIODevice>

// class AudioCapture : public QObject {
//     Q_OBJECT

// public:
//     explicit AudioCapture(QObject *parent = nullptr);

//     void start();
//     void stop();

// signals:
//     void audioChunkReady(QByteArray data);

// private:
//     QAudioSource *audioSource = nullptr;
//     QIODevice *device = nullptr;
// };





// #include "AudioCapture.h"

// AudioCapture::AudioCapture(QObject *parent)
//     : QObject(parent) {}

// void AudioCapture::start() {
//     QAudioFormat format;
//     format.setSampleRate(16000);
//     format.setChannelCount(1);
//     format.setSampleFormat(QAudioFormat::Int16);

//     auto deviceInfo = QMediaDevices::defaultAudioInput();
//     audioSource = new QAudioSource(deviceInfo, format, this);

//     device = audioSource->start();

//     connect(device, &QIODevice::readyRead, this, [this]() {
//         QByteArray data = device->readAll();
//         emit audioChunkReady(data);
//     });
// }

// void AudioCapture::stop() {
//     if (audioSource) {
//         audioSource->stop();
//     }
// }
