#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H

#include <QObject>

class AudioCapture;
class RecognitionWorker;

class SpeechManager : public QObject
{
    Q_OBJECT

public:
    explicit SpeechManager(QObject *parent = nullptr);
    ~SpeechManager();

    Q_INVOKABLE void startListening();
    Q_INVOKABLE void stopListening();

signals:

    void partialTextReady(const QString &text);

    void finalTextReady(const QString &text);

    void errorOccurred(const QString &message);

private:

    AudioCapture *m_capture = nullptr;

    RecognitionWorker *m_worker = nullptr;
};

#endif