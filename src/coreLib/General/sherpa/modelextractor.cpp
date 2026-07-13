#include "modelextractor.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDebug>

ModelExtractor::ModelExtractor(QObject *parent)
    : QObject(parent)
{
    m_modelDirectory =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation)
        + "/sherpa";

    QDir().mkpath(m_modelDirectory);
}

bool ModelExtractor::extractModels()
{
    bool ok = true;

    ok &= copyFile(
        ":/models/encoder_jit_trace-pnnx.ncnn.param",
        encoderParam());

    ok &= copyFile(
        ":/models/encoder_jit_trace-pnnx.ncnn.bin",
        encoderBin());

    ok &= copyFile(
        ":/models/decoder_jit_trace-pnnx.ncnn.param",
        decoderParam());

    ok &= copyFile(
        ":/models/decoder_jit_trace-pnnx.ncnn.bin",
        decoderBin());

    ok &= copyFile(
        ":/models/joiner_jit_trace-pnnx.ncnn.param",
        joinerParam());

    ok &= copyFile(
        ":/models/joiner_jit_trace-pnnx.ncnn.bin",
        joinerBin());

    ok &= copyFile(
        ":/models/tokens.txt",
        tokensFile());

    if (ok)
    {
        emit extractionFinished();
    }
    else
    {
        emit extractionFailed(
            "Failed to extract one or more model files.");
    }

    return ok;
}

bool ModelExtractor::copyFile(
    const QString &resourcePath,
    const QString &destinationPath)
{
    QFileInfo info(destinationPath);

    QDir().mkpath(info.absolutePath());

    //--------------------------------------------------------
    // Skip copy if already exists
    //--------------------------------------------------------

    if (QFile::exists(destinationPath))
    {
        return true;
    }

    QFile source(resourcePath);

    if (!source.exists())
    {
        qCritical()
        << "Missing resource:"
        << resourcePath;

        return false;
    }

    if (!source.open(QIODevice::ReadOnly))
    {
        qCritical()
        << "Cannot open resource:"
        << resourcePath;

        return false;
    }

    QFile destination(destinationPath);

    if (!destination.open(QIODevice::WriteOnly))
    {
        qCritical()
        << "Cannot create:"
        << destinationPath;

        return false;
    }

    destination.write(source.readAll());

    destination.close();

    source.close();

    qDebug()
        << "Copied:"
        << destinationPath;

    return true;
}

QString ModelExtractor::modelDirectory() const
{
    return m_modelDirectory;
}

QString ModelExtractor::encoderParam() const
{
    return m_modelDirectory
           + "/encoder_jit_trace-pnnx.ncnn.param";
}

QString ModelExtractor::encoderBin() const
{
    return m_modelDirectory
           + "/encoder_jit_trace-pnnx.ncnn.bin";
}

QString ModelExtractor::decoderParam() const
{
    return m_modelDirectory
           + "/decoder_jit_trace-pnnx.ncnn.param";
}

QString ModelExtractor::decoderBin() const
{
    return m_modelDirectory
           + "/decoder_jit_trace-pnnx.ncnn.bin";
}

QString ModelExtractor::joinerParam() const
{
    return m_modelDirectory
           + "/joiner_jit_trace-pnnx.ncnn.param";
}

QString ModelExtractor::joinerBin() const
{
    return m_modelDirectory
           + "/joiner_jit_trace-pnnx.ncnn.bin";
}

QString ModelExtractor::tokensFile() const
{
    return m_modelDirectory
           + "/tokens.txt";
}