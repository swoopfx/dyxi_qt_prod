#ifndef WHISPERWORKER_H
#define WHISPERWORKER_H

#include <QObject>
#include <QQmlEngine>
#include <QString>
// #include "whisper.h"
#include <atomic>
#include <vector>
#include <QObject>
#include <QTimer>
#include <QThread>

#include "whisper.h"
// #include "../audio/CircularAudioBuffer.h"
#include "../General/audio/circularaudiobuffer.h"


class WhisperWorker : public QObject
{
    Q_OBJECT

public:
    explicit WhisperWorker(CircularAudioBuffer *buffer,
                           whisper_context *ctx);
 ~WhisperWorker();
public slots:
    void run();
    void stop();

signals:
    void textReady(const QString &text);

private:
    float energy(const float *data, int n);
    bool isSpeech(const std::vector<float> &audio);
   std::vector<float>teste(float);

    QString extractText();

private:
    CircularAudioBuffer *m_buffer;
    whisper_context *m_ctx;

    std::atomic_bool m_running{true};
    // NEW
    whisper_state* m_state = nullptr;
    QString m_lastText;
};
#endif // WHISPERWORKER_H
