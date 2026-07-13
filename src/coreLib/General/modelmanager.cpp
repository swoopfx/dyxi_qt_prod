#include "modelmanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

static const QString MODEL_URL =
    "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en-q5_1.bin?download=true";

static const QString MODEL_FILENAME =
    "ggml-tiny.en-q5_1.bin";

ModelManager::ModelManager(QObject *parent)
    : QObject(parent)
{
}

QString ModelManager::modelDirectory() const
{
    QString dir =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);

    dir += "/models";

    QDir().mkpath(dir);

    return dir;
}

QString ModelManager::modelPath() const
{
    return modelDirectory()
    + "/"
        + MODEL_FILENAME;
}

QString ModelManager::temporaryModelPath() const
{
    return modelDirectory()
    + "/"
        + MODEL_FILENAME
        + ".part";
}

bool ModelManager::isModelAvailable() const
{
    return QFile::exists(modelPath());
}

void ModelManager::ensureModelAvailable()
{
    if(isModelAvailable())
    {
        qDebug() << "Model already installed.";

        emit modelReady(modelPath());

        return;
    }

    startDownload();
}

void ModelManager::ensureModelAvailable(bool forceDownload)
{
    QString path = modelPath();

    // Force delete existing model
    if (forceDownload && QFile::exists(path))
    {
        qDebug() << "Removing existing model...";

        if (!QFile::remove(path))
        {
            emit error("Failed to delete existing model.");
            return;
        }

        qDebug() << "Old model removed.";
    }

    if (QFile::exists(path))
    {
        emit modelReady(path);
        return;
    }

    downloadModel();
}

void ModelManager::startDownload()
{
    if(m_reply)
        return;

    emit downloadStarted();

    QFile::remove(temporaryModelPath());

    m_outputFile.setFileName(
        temporaryModelPath());

    if(!m_outputFile.open(
            QIODevice::WriteOnly))
    {
        emit error("Cannot create model file.");

        return;
    }

    QNetworkRequest request{ QUrl(MODEL_URL) };
    m_reply = m_manager.get(request);

    connect(m_reply,
            &QNetworkReply::readyRead,
            this,
            [this]()
            {
                if(m_outputFile.isOpen())
                {
                    m_outputFile.write(
                        m_reply->readAll());
                }
            });

    connect(m_reply,
            &QNetworkReply::downloadProgress,
            this,
            &ModelManager::downloadProgress);

    connect(m_reply,
            &QNetworkReply::finished,
            this,
            &ModelManager::onDownloadFinished);
}


void ModelManager::downloadModel()
{
    if(m_reply)
        return;

    emit downloadStarted();

    QFile::remove(temporaryModelPath());

    m_outputFile.setFileName(
        temporaryModelPath());

    if(!m_outputFile.open(
            QIODevice::WriteOnly))
    {
        emit error("Cannot create model file.");

        return;
    }

    QNetworkRequest request{ QUrl(MODEL_URL) };
    m_reply = m_manager.get(request);

    connect(m_reply,
            &QNetworkReply::readyRead,
            this,
            [this]()
            {
                if(m_outputFile.isOpen())
                {
                    m_outputFile.write(
                        m_reply->readAll());
                }
            });

    connect(m_reply,
            &QNetworkReply::downloadProgress,
            this,
            &ModelManager::downloadProgress);

    connect(m_reply,
            &QNetworkReply::finished,
            this,
            &ModelManager::onDownloadFinished);
}

void ModelManager::onDownloadFinished()
{
    if(!m_reply)
        return;

    m_outputFile.flush();
    m_outputFile.close();

    if(m_reply->error() !=
        QNetworkReply::NoError)
    {
        QFile::remove(
            temporaryModelPath());

        emit error(
            m_reply->errorString());

        m_reply->deleteLater();
        m_reply = nullptr;

        return;
    }

    QFile::remove(modelPath());

    if(!QFile::rename(
            temporaryModelPath(),
            modelPath()))
    {
        QFile::remove(
            temporaryModelPath());

        emit error(
            "Unable to save model.");

        m_reply->deleteLater();
        m_reply = nullptr;

        return;
    }

    qDebug()
        << "Model downloaded successfully.";

    emit downloadFinished();

    emit modelReady(modelPath());

    m_reply->deleteLater();
    m_reply = nullptr;
}