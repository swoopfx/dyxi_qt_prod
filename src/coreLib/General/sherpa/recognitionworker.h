#ifndef RECOGNITIONWORKER_H
#define RECOGNITIONWORKER_H

#include <QObject>
#include <QVector>
#include <QString>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "modelextractor.h"
#include <QMetaType>

// Q_DECLARE_METATYPE(QVector<float>)

extern "C" {
#include "../../Sherpa/sherpa-ncnn/sherpa-ncnn/c-api/c-api.h"
}
class RecognitionWorker : public QObject
{
    Q_OBJECT

public:
    explicit RecognitionWorker(QObject *parent = nullptr);
    ~RecognitionWorker();

    bool start();
    void stop();

    void enqueueSamples(const QVector<float> &samples);

signals:
    void partialResultChanged(const QString &text);
    void finalResultChanged(const QString &text);
    void errorOccurred(const QString &error);

private:
    void recognitionLoop();

    bool initSherpa();
    void destroySherpa();

private:

    //-------------------------
    // Sherpa
    //-------------------------

    SherpaNcnnRecognizer *m_recognizer = nullptr;
    SherpaNcnnStream *m_stream = nullptr;

    ModelExtractor m_extractor;

    //-------------------------
    // Recognition Thread
    //-------------------------

    std::thread m_thread;

    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::queue<QVector<float>> m_queue;

    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
};

#endif