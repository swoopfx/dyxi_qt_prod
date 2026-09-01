#pragma once
#include <QObject>
#include <QAudioSink>
#include <QIODevice>
#include <QByteArray>
#include <QVariantList>
#include <QQmlEngine>

class AudioSynth : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AudioSynth(QObject *parent = nullptr);
    ~AudioSynth();

    // Invokable multi-sensory sound actions from QML
    Q_INVOKABLE void playHapticSound(const QString &type, double frequency, int durationMs);
    Q_INVOKABLE void playTransition(const QVariantList &notes);
    Q_INVOKABLE void playMatchFound();

private:
    void playTone(double frequency, int durationMs, double volume = 0.5);
    
    QAudioFormat m_format;
    QAudioSink *m_audioSink = nullptr;
    QIODevice *m_audioDevice = nullptr;
};