#pragma once

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>


class ModelManager : public QObject
{
    Q_OBJECT

public:
    explicit ModelManager(QObject *parent = nullptr);

    // Starts checking/downloading asynchronously.
    void ensureModelAvailable();
   void ensureModelAvailable(bool forceDownload = false);

    // Returns the expected model path.
    QString modelPath() const;

    // Returns true if the model already exists.
    bool isModelAvailable() const;

signals:
    void downloadStarted();

    void downloadProgress(qint64 received, qint64 total);



    // Emitted when the model is ready to use.
    void modelReady(const QString &path);

    void downloadFinished();

    void error(const QString &message);

private slots:
    void onDownloadFinished();

private:
    QString modelDirectory() const;

    QString temporaryModelPath() const;

    void startDownload();

 void downloadModel();
private:
    QNetworkAccessManager m_manager;
    QNetworkReply *m_reply = nullptr;
    QFile m_outputFile;
};