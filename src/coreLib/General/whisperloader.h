#ifndef WHISPERLOADER_H
#define WHISPERLOADER_H

#include <QObject>
#include <QQmlEngine>

#include <QObject>
#include <QString>
#include <atomic>

#include "whisper.h"

class WhisperLoader : public QObject
{
    Q_OBJECT

public:
    explicit WhisperLoader(QObject *parent = nullptr);
    ~WhisperLoader();

    // Transfers ownership OUT (called once after loaded)
    whisper_context *takeContext();

public slots:
    void loadModel(const QString &modelPath);
    void cancel();

signals:
    void loadingStarted();
    void progress(const QString &stage);
    void loaded();
    void failed(const QString &error);

private:
    whisper_context *m_ctx = nullptr;

    std::atomic_bool m_cancelled{false};
    std::atomic_bool m_loading{false};
};

#endif // WHISPERLOADER_H
