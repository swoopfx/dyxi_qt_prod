#include "speechrecognizer.h"
#include "modelmanager.h"

#include <QMediaDevices>
#include <QJSEngine>

#include <QMediaDevices>
#include <QDebug>

#include <QMediaDevices>
#include <QDebug>
#include <QFileInfo>

SpeechRecognizer *SpeechRecognizer::instance()
{
    static SpeechRecognizer inst;
    return &inst;
}

SpeechRecognizer *SpeechRecognizer::create(QQmlEngine *,
                                           QJSEngine *)
{
    return instance();
}

SpeechRecognizer::SpeechRecognizer(QObject *parent)
    : QObject(parent)
{
}

/* =========================
 * INITIALIZE PIPELINE
 * ========================= */
bool SpeechRecognizer::initialize()
{
    if (m_modelManager)
        return true;

    m_modelManager = new ModelManager(this);

    connect(m_modelManager,
            &ModelManager::downloadStarted,
            this,
            []() { qDebug() << "Model download started"; });

    connect(m_modelManager,
            &ModelManager::downloadProgress,
            this,
            &SpeechRecognizer::modelDownloadProgress);

    connect(m_modelManager,
            &ModelManager::error,
            this,
            &SpeechRecognizer::modelDownloadFailed);

    connect(m_modelManager,
            &ModelManager::modelReady,
            this,
            &SpeechRecognizer::onModelReady);

    m_modelManager->ensureModelAvailable(false);

    return true;
}

/* =========================
 * MODEL READY → LOAD WHISPER
 * ========================= */
void SpeechRecognizer::onModelReady(const QString &path)
{
    qDebug() << "Model ready:" << path;

    // Always load Whisper in separate thread
    startWhisperLoader(path);
}

/* =========================
 * WHISPER LOADER THREAD
 * ========================= */
void SpeechRecognizer::startWhisperLoader(const QString &modelPath)
{
    m_loaderThread = new QThread(this);
    m_loader = new WhisperLoader;

    m_loader->moveToThread(m_loaderThread);
    QFileInfo info(modelPath);

    qDebug() << "MODEL PATH:" << info.absoluteFilePath();
    qDebug() << "MODEL SIZE:" << info.size();


    connect(m_loaderThread,
            &QThread::started,
            [this, modelPath]()
            {
                QMetaObject::invokeMethod(
                    m_loader,
                    "loadModel",
                    Qt::QueuedConnection,
                    Q_ARG(QString, modelPath));
            });

    connect(m_loader,
            &WhisperLoader::loaded,
            this,
            &SpeechRecognizer::onWhisperLoaded);

    connect(m_loader,
            &WhisperLoader::failed,
            this,
            &SpeechRecognizer::modelDownloadFailed);

    connect(m_loaderThread,
            &QThread::finished,
            m_loader,
            &QObject::deleteLater);

    m_loaderThread->start();
}

/* =========================
 * WHISPER LOADED
 * ========================= */
void SpeechRecognizer::onWhisperLoaded()
{
    // m_ctx = m_loader->takeContext();

    // qDebug() << "Whisper loaded in background thread";

    // startWorker();
qDebug() << "Whisper context start!";
    m_ctx = m_loader->takeContext();


    if (!m_ctx)
    {
        qDebug() << "Whisper context is NULL!";
        return;
    }

    qDebug() << "Whisper context created:" << m_ctx;

    startWorker();
}

/* =========================
 * START WHISPER WORKER
 * ========================= */
void SpeechRecognizer::startWorker()
{
    m_workerThread = new QThread(this);
    m_worker = new WhisperWorker(&m_buffer, m_ctx);

    m_worker->moveToThread(m_workerThread);

    connect(m_workerThread,
            &QThread::started,
            m_worker,
            &WhisperWorker::run);

    connect(m_worker,
            &WhisperWorker::textReady,
            this,
            &SpeechRecognizer::textRecognized);

    connect(this,
            &SpeechRecognizer::stopWorker,
            m_worker,
            &WhisperWorker::stop);

    connect(m_workerThread,
            &QThread::finished,
            m_worker,
            &QObject::deleteLater);

    m_workerThread->start();

    qDebug() << "Whisper worker started";

    emit modelReady();
}

/* =========================
 * AUDIO START
 * ========================= */
void SpeechRecognizer::start()
{
    qDebug() << "Starting audio capture...";
    if (!m_ctx)
    {
        qDebug() << "Whisper not ready yet";
        return;
    }

    if (m_audioSource)
        return;

    // QAudioFormat format;
    // format.setSampleRate(16000);
    // format.setChannelCount(1);
    // format.setSampleFormat(QAudioFormat::Int16);

    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice device = QMediaDevices::defaultAudioInput();

    if (!device.isFormatSupported(format))
    {
        qDebug() << "Format not supported, switching...";
        format = device.preferredFormat();
    }


    m_audioSource = new QAudioSource(
        QMediaDevices::defaultAudioInput(),
        format,
        this);

    m_audioDevice = m_audioSource->start();
    if (!m_audioDevice)
    {
        qDebug() << "❌ Audio device failed to start!";
        return;
    }

    qDebug() << "✅ Audio device started";

    connect(m_audioDevice,
            &QIODevice::readyRead,
            this,
            &SpeechRecognizer::processAudio);

    qDebug() << "Audio capture started";
}

/* =========================
 * AUDIO CALLBACK
 * ========================= */
// void SpeechRecognizer::processAudio()
// {
//     QByteArray data = m_audioDevice->readAll();
//      // qDebug() << "Audio bytes received:" << data.size();

//     const int16_t *pcm =
//         reinterpret_cast<const int16_t *>(data.constData());

//     int samples = data.size() / sizeof(int16_t);

//     std::vector<float> audio(samples);
//     // qDebug() << "RAW SIZE:" << data.size();
//     // qDebug() << "FIRST BYTES:" << data.left(20).toHex();


//     for (int i = 0; i < samples; i++)
//         audio[i] = pcm[i] / 32768.0f;

//     float sum = 0;
//     for (int i = 0; i < audio.size(); i++)
//         sum += std::abs(audio[i]);

//     // qDebug() << "AUDIO ENERGY:" << sum;

//     auto deviceRate =
//         QMediaDevices::defaultAudioInput()
//             .preferredFormat()
//             .sampleRate();

//     audio = resampleTo16k(audio, deviceRate);



//     m_buffer.push(audio.data(), audio.size());

// }

void SpeechRecognizer::processAudio()
{
    QByteArray data = m_audioDevice->readAll();

    const int16_t* pcm =
        reinterpret_cast<const int16_t*>(data.constData());

    int n = data.size() / sizeof(int16_t);

    std::vector<float> audio;
    audio.reserve(n);

    for (int i = 0; i < n; i++)
    {
        audio.push_back(pcm[i] / 32768.0f);
    }

    int rate = m_audioSource->format().sampleRate();
    audio = resampleTo16k(audio, rate);

    // //  IMPORTANT: RESAMPLE HERE
    // audio = resample48kTo16k(audio);

    m_buffer.push(audio.data(), audio.size());
}

/* =========================
 * STOP EVERYTHING
 * ========================= */
void SpeechRecognizer::stop()
{
    emit stopWorker();

    /* ---- stop audio ---- */
    if (m_audioSource)
    {
        m_audioSource->stop();
        m_audioSource->deleteLater();
        m_audioSource = nullptr;
        m_audioDevice = nullptr;
    }

    /* ---- stop worker thread ---- */
    if (m_workerThread)
    {
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_workerThread;
        m_workerThread = nullptr;
    }

    /* ---- stop loader thread ---- */
    if (m_loaderThread)
    {
        m_loaderThread->quit();
        m_loaderThread->wait();
        delete m_loaderThread;
        m_loaderThread = nullptr;
    }

    /* ---- cleanup whisper ---- */
    if (m_ctx)
    {
        whisper_free(m_ctx);
        m_ctx = nullptr;
    }

    delete m_worker;
    m_worker = nullptr;

    delete m_loader;
    m_loader = nullptr;

    m_modelManager = nullptr;

    qDebug() << "SpeechRecognizer stopped";
}

std::vector<float>  SpeechRecognizer::resampleTo16k(const std::vector<float>& in, int inRate)
{
    const int outRate = 16000;
    double ratio = (double)inRate / outRate;

    std::vector<float> out;
    out.reserve(in.size() / ratio);

    for (double i = 0; i < in.size(); i += ratio)
    {
        int idx = (int)i;
        if (idx < in.size())
            out.push_back(in[idx]);
    }

    return out;
}

std::vector<float> SpeechRecognizer::resample48kTo16k(
    const std::vector<float>& in)
{
    const int inRate = 48000;
    const int outRate = 16000;

    double ratio = (double)inRate / outRate;

    std::vector<float> out;
    out.reserve(in.size() / ratio);

    for (double i = 0; i < in.size(); i += ratio)
    {
        int idx = (int)i;
        if (idx < in.size())
            out.push_back(in[idx]);
    }

    return out;
}

