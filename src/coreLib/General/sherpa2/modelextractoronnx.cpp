#include "modelextractoronnx.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDebug>

ModelExtractorOnnx::ModelExtractorOnnx(QObject *parent)
    : QObject(parent)
{
    m_modelDirectory =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation
            ) + "/moonshine";

    if (!QDir().mkpath(m_modelDirectory)) {
        qCritical()
        << "Failed to create model directory:"
        << m_modelDirectory;
    }

    qDebug()
        << "Moonshine model directory:"
        << m_modelDirectory;
}

bool ModelExtractorOnnx::extractModels()
{
    bool ok = true;

    ok &= copyFile(
        "assets:/moonshine/encode.int8.onnx",
        encoderOnnx()
        );

    ok &= copyFile(
        "assets:/moonshine/preprocess.onnx",
        preprocessOnnx()
        );

    ok &= copyFile(
        "assets:/moonshine/cached_decode.int8.onnx",
        cachedDecodeOnnx()
        );

    ok &= copyFile(
        "assets:/moonshine/uncached_decode.int8.onnx",
        uncachedDecodeOnnx()
        );

    ok &= copyFile(
        "assets:/moonshine/silero_vad.onnx",
        sileroVad()
        );

    ok &= copyFile(
        "assets:/moonshine/tokens.txt",
        tokensTxt()
        );

    if (ok) {
        qDebug()
        << "All Moonshine models extracted successfully.";

        emit extractionFinished();
    }
    else {
        qCritical()
        << "Failed to extract one or more Moonshine model files.";

        emit extractionFailed(
            "Failed to extract one or more model files."
            );
    }

    return ok;
}

void ModelExtractorOnnx::instantiateConfig()
{
    vadConfig.silero_vad.model = sileroVad().toStdString();
    vadConfig.sample_rate = 16000;
    vadConfig.num_threads = 2;
    vadConfig.provider = "cpu";
    vadConfig.debug = false;

    if (!vadConfig.Validate())
    {
        qFatal("Invalid Silero VAD configuration");
    }



    // Moonshine v1 models
    recognizerConfig.model_config.moonshine.preprocessor =
        preprocessOnnx().toStdString();

    recognizerConfig.model_config.moonshine.encoder = encoderOnnx().toStdString();

    recognizerConfig.model_config.moonshine.uncached_decoder =
       uncachedDecodeOnnx(). toStdString();

    recognizerConfig.model_config.moonshine.cached_decoder =
        cachedDecodeOnnx().toStdString();

    // Tokens
    recognizerConfig.model_config.tokens =
        tokensTxt().toStdString();

    // Runtime options
    recognizerConfig.model_config.provider = "cpu";
    recognizerConfig.model_config.num_threads = 2;
    recognizerConfig.model_config.debug = false;

    // Feature extractor
    recognizerConfig.feat_config.sampling_rate = 16000;
    recognizerConfig.feat_config.feature_dim = 80;

    // Decoder
    recognizerConfig.decoding_method = "greedy_search";
    recognizerConfig.max_active_paths = 4;

    if (!recognizerConfig.Validate())
    {
        qFatal("Invalid Moonshine recognizer configuration");
    }
}



sherpa_onnx::VadModelConfig ModelExtractorOnnx::getVadConfig() const
{
    return vadConfig;
}

sherpa_onnx::OfflineRecognizerConfig  ModelExtractorOnnx::getRecognizerConfig() const
{
    return recognizerConfig;
}

QString ModelExtractorOnnx::modelDirectory() const
{
    return m_modelDirectory;
}

QString ModelExtractorOnnx::preprocessOnnx() const
{
    return m_modelDirectory + "/preprocess.onnx";
}

QString ModelExtractorOnnx::encoderOnnx() const
{
    return m_modelDirectory + "/encode.int8.onnx";
}

QString ModelExtractorOnnx::cachedDecodeOnnx() const
{
    return m_modelDirectory + "/cached_decode.int8.onnx";
}

QString ModelExtractorOnnx::uncachedDecodeOnnx() const
{
    return m_modelDirectory + "/uncached_decode.int8.onnx";
}

QString ModelExtractorOnnx::sileroVad() const
{
    return m_modelDirectory + "/silero_vad.onnx";
}

QString ModelExtractorOnnx::tokensTxt() const
{
    return m_modelDirectory + "/tokens.txt";
}

bool ModelExtractorOnnx::copyFile(
    const QString &assetPath,
    const QString &destinationPath)
{
    QFileInfo info(destinationPath);

    if (!QDir().mkpath(info.absolutePath())) {
        qCritical()
        << "Cannot create directory:"
        << info.absolutePath();

        return false;
    }

    // ----------------------------------------------------
    // Already extracted
    // ----------------------------------------------------

    if (QFile::exists(destinationPath)) {

        qDebug()
        << "Model already exists:"
        << destinationPath;

        return true;
    }

    // ----------------------------------------------------
    // Open Android asset
    // ----------------------------------------------------

    QFile source(assetPath);

    if (!source.exists()) {

        qCritical()
        << "Android asset does not exist:"
        << assetPath;

        return false;
    }

    if (!source.open(QIODevice::ReadOnly)) {

        qCritical()
        << "Cannot open Android asset:"
        << assetPath
        << "error:"
        << source.errorString();

        return false;
    }

    // ----------------------------------------------------
    // Create destination
    // ----------------------------------------------------

    QFile destination(destinationPath);

    if (!destination.open(QIODevice::WriteOnly)) {

        qCritical()
        << "Cannot create:"
        << destinationPath
        << "error:"
        << destination.errorString();

        return false;
    }

    // ----------------------------------------------------
    // Copy
    // ----------------------------------------------------

    const QByteArray data = source.readAll();

    const qint64 written =
        destination.write(data);

    destination.close();
    source.close();

    if (written != data.size()) {

        qCritical()
        << "Incomplete model copy:"
        << assetPath
        << "expected:"
        << data.size()
        << "written:"
        << written;

        QFile::remove(destinationPath);

        return false;
    }

    qDebug()
        << "Extracted Android asset:"
        << assetPath
        << "->"
        << destinationPath
        << "size:"
        << written;

    return true;
}
