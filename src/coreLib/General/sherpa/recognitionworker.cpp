#include "recognitionworker.h"
#include <QDebug>
#include <QMutexLocker>
#include <QFile>
#include <QFileInfo>

#include "recognitionworker.h"




RecognitionWorker::RecognitionWorker(QObject *parent)
    : QObject(parent)
{
}

RecognitionWorker::~RecognitionWorker()
{
    stop();
}
bool RecognitionWorker::start()
{
    if (m_running)
        return true;

    if (!initSherpa())
    {
        emit errorOccurred("Failed to initialize Sherpa.");
        return false;
    }

    m_stopRequested = false;
    m_running = true;

    m_thread = std::thread(
        &RecognitionWorker::recognitionLoop,
        this);

    return true;
}

void RecognitionWorker::stop()
{
    if (!m_running)
        return;

    m_stopRequested = true;

    m_cv.notify_all();

    if (m_thread.joinable())
        m_thread.join();

    destroySherpa();

    m_running = false;
}

void RecognitionWorker::enqueueSamples(
    const QVector<float> &samples)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_queue.push(samples);
    }

    m_cv.notify_one();
}

void RecognitionWorker::recognitionLoop()
{
    qDebug() << "Recognition thread started";

    while (!m_stopRequested)
    {
        QVector<float> samples;

        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_cv.wait(lock,
                      [this]
                      {
                          return !m_queue.empty()
                          || m_stopRequested;
                      });

            if (m_stopRequested)
                break;

            samples = std::move(m_queue.front());
            m_queue.pop();
        }

        AcceptWaveform(
            m_stream,
            16000,
            samples.constData(),
            samples.size());

        while (IsReady(m_recognizer, m_stream))
        {
            Decode(m_recognizer, m_stream);

            SherpaNcnnResult *result =
                GetResult(
                    m_recognizer,
                    m_stream);

            if (result && result->text)
            {
                QString text =
                    QString::fromUtf8(result->text);

                emit partialResultChanged(text);

                if (IsEndpoint(
                        m_recognizer,
                        m_stream))
                {
                    emit finalResultChanged(text);

                    Reset(
                        m_recognizer,
                        m_stream);
                }
            }

            DestroyResult(result);
        }
    }

    qDebug() << "Recognition thread stopped";
}


bool RecognitionWorker::initSherpa()
{
    if (!m_extractor.extractModels())
        return false;

    SherpaNcnnRecognizerConfig config;

    memset(&config,0,sizeof(config));

    QByteArray ep =
        m_extractor.encoderParam().toUtf8();

    QByteArray eb =
        m_extractor.encoderBin().toUtf8();

    QByteArray dp =
        m_extractor.decoderParam().toUtf8();

    QByteArray db =
        m_extractor.decoderBin().toUtf8();

    QByteArray jp =
        m_extractor.joinerParam().toUtf8();

    QByteArray jb =
        m_extractor.joinerBin().toUtf8();

    QByteArray tk =
        m_extractor.tokensFile().toUtf8();

    config.model_config.encoder_param = ep.constData();
    config.model_config.encoder_bin = eb.constData();
    config.model_config.decoder_param = dp.constData();
    config.model_config.decoder_bin = db.constData();
    config.model_config.joiner_param = jp.constData();
    config.model_config.joiner_bin = jb.constData();
    config.model_config.tokens = tk.constData();
    config.model_config.num_threads = 4; // Use multi-core on Android
    config.feat_config.sampling_rate = 16000;
    config.feat_config.feature_dim = 80;
    config.enable_endpoint = 1;
    config.rule1_min_trailing_silence = 1.2f;
    config.decoder_config.decoding_method = "greedy_search";
    config.decoder_config.num_active_paths = 4;

    config.model_config.use_vulkan_compute = 0;
    config.model_config.num_threads = 4;

    config.enable_endpoint = 1;
    config.rule1_min_trailing_silence = 1.2f;
    config.rule2_min_trailing_silence = 2.4f;
    config.rule3_min_utterance_length = 20.0f;

    config.hotwords_file = "";
    config.hotwords_score = 1.5f;

    m_recognizer = CreateRecognizer(&config);

    if (!m_recognizer)
        return false;

    m_stream =
        CreateStream(m_recognizer);

    return m_stream != nullptr;
}

void RecognitionWorker::destroySherpa()
{
    if (m_stream)
    {
        DestroyStream(m_stream);
        m_stream = nullptr;
    }

    if (m_recognizer)
    {
        DestroyRecognizer(m_recognizer);
        m_recognizer = nullptr;
    }
}