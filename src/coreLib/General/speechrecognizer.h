#ifndef SPEECHRECOGNIZER_H
#define SPEECHRECOGNIZER_H

#include <QObject>
#include <QThread>
#include <QAudioSource>
#include <QAudioDevice>
#include <QQmlEngine>
#include <QJSEngine>
#include "whisper.h"
#include "audio/circularaudiobuffer.h"
#include "../WhisperApp/whisperworker.h"

#include "modelmanager.h"
#include "whisperloader.h"
// #include "whisperworker.h"

#include "whisper.h"

class SpeechRecognizer : public QObject
{
    Q_OBJECT

public:
    static SpeechRecognizer *instance();
    static SpeechRecognizer *create(QQmlEngine *engine,
                                    QJSEngine *scriptEngine);

    explicit SpeechRecognizer(QObject *parent = nullptr);

    // Starts full pipeline (download → load → worker)
    bool initialize();

public slots:
    void start();
    void stop();

signals:
    void textRecognized(const QString &text);

    // Model events
    void modelReady();
    void modelDownloadProgress(qint64 received, qint64 total);
    void modelDownloadFailed(const QString &error);

    // Internal control
    void stopWorker();

private slots:
    // Model pipeline
    void onModelReady(const QString &path);
    void onWhisperLoaded();

    // Audio processing
    void processAudio();

private:
    void startWhisperLoader(const QString &modelPath);
    void startWorker();
    std::vector<float> resampleTo16k(const std::vector<float>& in, int inRate);
    std::vector<float> resample48kTo16k(const std::vector<float>& in);

private:
    /* =========================
     * CORE COMPONENTS
     * ========================= */

    ModelManager *m_modelManager = nullptr;

    WhisperLoader *m_loader = nullptr;
    QThread *m_loaderThread = nullptr;

    WhisperWorker *m_worker = nullptr;
    QThread *m_workerThread = nullptr;

    whisper_context *m_ctx = nullptr;

    /* =========================
     * AUDIO SYSTEM
     * ========================= */

    QAudioSource *m_audioSource = nullptr;
    QIODevice *m_audioDevice = nullptr;

    CircularAudioBuffer m_buffer;

    /* =========================
     * STATE
     * ========================= */

    bool m_initialized = false;
};

#endif // SPEECHRECOGNIZER_H
