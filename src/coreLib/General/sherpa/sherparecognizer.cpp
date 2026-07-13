#include "sherparecognizer.h"
#include <QDebug>
#include <QMutexLocker>

SherpaRecognizer::SherpaRecognizer(QObject *parent)
    : QObject(parent), mRecognizer(nullptr), mStream(nullptr), mInitialized(false)
{
}

SherpaRecognizer::~SherpaRecognizer()
{
    destroy();
}

bool SherpaRecognizer::initialize(
    const QString &encoderParam,
    const QString &encoderBin,
    const QString &decoderParam,
    const QString &decoderBin,
    const QString &joinerParam,
    const QString &joinerBin,
    const QString &tokens)
{
    QMutexLocker locker(&mMutex);

    // Clean up any existing instance
    destroy();

    SherpaNcnnRecognizerConfig config;
    memset(&config, 0, sizeof(config));

    // 1. Keep QByteArrays in scope until CreateRecognizer is finished.
    // This prevents pointers from pointing to deleted memory.
    QByteArray ep = encoderParam.toUtf8();
    QByteArray eb = encoderBin.toUtf8();
    QByteArray dp = decoderParam.toUtf8();
    QByteArray db = decoderBin.toUtf8();
    QByteArray jp = joinerParam.toUtf8();
    QByteArray jb = joinerBin.toUtf8();
    QByteArray tk = tokens.toUtf8();

    // 2. Model configuration
    config.model_config.encoder_param = ep.constData();
    config.model_config.encoder_bin = eb.constData();
    config.model_config.decoder_param = dp.constData();
    config.model_config.decoder_bin = db.constData();
    config.model_config.joiner_param = jp.constData();
    config.model_config.joiner_bin = jb.constData();
    config.model_config.tokens = tk.constData();
    config.model_config.use_vulkan_compute = 0;
    config.model_config.num_threads = 4; // Adjusted to 4 for better performance

    // 3. Feature configuration
    config.feat_config.sampling_rate = 16000;
    config.feat_config.feature_dim = 80;

    // 4. Decoder configuration
    // FIXED: Direct assignment for const char* pointer
    config.decoder_config.decoding_method = "greedy_search";
    config.decoder_config.num_active_paths = 4;

    // 5. Endpoint detection
    config.enable_endpoint = 1;
    config.rule1_min_trailing_silence = 2.4f;
    config.rule2_min_trailing_silence = 1.2f;
    config.rule3_min_utterance_length = 20.0f;

    // 6. Hotwords
    config.hotwords_file = "";
    config.hotwords_score = 1.5f;

    // 7. Create the Recognizer
    mRecognizer = CreateRecognizer(&config);

    if (!mRecognizer) {
        emit errorOccurred("Unable to create Sherpa recognizer.");
        return false;
    }

    // 8. Create the Stream
    mStream = CreateStream(mRecognizer);
    if (!mStream) {
        DestroyRecognizer(mRecognizer);
        mRecognizer = nullptr;
        emit errorOccurred("Unable to create Sherpa stream.");
        return false;
    }

    mInitialized = true;
    emit initializedChanged();
    qDebug() << "Sherpa recognizer initialized successfully.";

    return true;
}


void SherpaRecognizer::destroy()
{
    // Note: If mMutex is not recursive, ensure this isn't called
    // from within another locked method.
    QMutexLocker locker(&mMutex);

    if (mStream) {
        DestroyStream(mStream);
        mStream = nullptr;
    }

    if (mRecognizer) {
        DestroyRecognizer(mRecognizer);
        mRecognizer = nullptr;
    }

    if (mInitialized) {
        mInitialized = false;
        mPartialResult.clear();
        mFinalResult.clear();
        emit initializedChanged();
    }
}

void SherpaRecognizer::acceptWaveform(const QVector<float> &samples, float sampleRate)
{
    QMutexLocker locker(&mMutex);

    if (!mInitialized || samples.isEmpty())
        return;

    AcceptWaveform(mStream, sampleRate, samples.constData(), samples.size());

    // Internal processing
    decodeInternal();
}

// used within c++ to ectract txt
RecognitionResult SherpaRecognizer::processAudio(
    const QVector<float> &samples,
    float sampleRate)
{
    QMutexLocker locker(&mMutex);

    RecognitionResult result;

    if (!mInitialized || samples.isEmpty())
        return result;

    AcceptWaveform(mStream,
                   sampleRate,
                   samples.constData(),
                   samples.size());

    return decodeInternal();
}

// Private helper - assumes mutex is already locked
RecognitionResult SherpaRecognizer::decodeInternal()
{
    // if (!mRecognizer || !mStream) return;

    // while (IsReady(mRecognizer, mStream)) {
    //     Decode(mRecognizer, mStream);
    // }

    // updateResultInternal();

    // if (IsEndpoint(mRecognizer, mStream)) {
    //     emit endpointDetected();
    //     Reset(mRecognizer, mStream);
    // }

    RecognitionResult result;

    while (IsReady(mRecognizer, mStream))
    {
        Decode(mRecognizer, mStream);
    }

    result = updateResultInternal();

    if (IsEndpoint(mRecognizer, mStream))
    {
        result.endpoint = true;
        emit endpointDetected();

        Reset(mRecognizer, mStream);
    }

    return result;
}

// Private helper - assumes mutex is already locked
RecognitionResult SherpaRecognizer::updateResultInternal()
{
    // SherpaNcnnResult *result = GetResult(mRecognizer, mStream);
    // if (!result) return;

    // QString text = QString::fromUtf8(result->text);
    // if (text != mPartialResult) {
    //     mPartialResult = text;
    //     emit partialResultChanged(mPartialResult);
    // }

    // DestroyResult(result);

    RecognitionResult out;

    SherpaNcnnResult *result = GetResult(mRecognizer, mStream);
    if (!result)
        return out;

    out.partialText = QString::fromUtf8(result->text);

    if (out.partialText != mPartialResult)
    {
        mPartialResult = out.partialText;
        emit partialResultChanged(mPartialResult);
    }

    DestroyResult(result);

    return out;
}

QString SherpaRecognizer::partialResult() const
{
    QMutexLocker locker(&mMutex);
    return mPartialResult;
}

QString SherpaRecognizer::finalResult() const
{
    QMutexLocker locker(&mMutex);
    return mFinalResult;
}

void SherpaRecognizer::inputFinished()
{
    QMutexLocker locker(&mMutex);
    if (!mInitialized) return;

    InputFinished(mStream);

    while (IsReady(mRecognizer, mStream)) {
        Decode(mRecognizer, mStream);
    }

    SherpaNcnnResult *result = GetResult(mRecognizer, mStream);
    if (result) {
        mFinalResult = QString::fromUtf8(result->text);
        emit finalResultChanged(mFinalResult);
        DestroyResult(result);
    }
}

void SherpaRecognizer::reset()
{
    QMutexLocker locker(&mMutex);
    if (!mInitialized) return;

    Reset(mRecognizer, mStream);
    mPartialResult.clear();
    mFinalResult.clear();

    emit partialResultChanged("");
    emit finalResultChanged("");
}



// RecognitionResult r = recognizer.processAudio(samples);

// if (!r.partialText.isEmpty())
//     qDebug() << r.partialText;

// if (r.endpoint)
//     qDebug() << "End of utterance";