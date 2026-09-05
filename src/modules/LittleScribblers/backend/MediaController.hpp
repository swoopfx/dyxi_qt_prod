#pragma once

#include <QObject>
#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QtQml/qqmlregistration.h>

class MediaController : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool soundEffectsEnabled READ soundEffectsEnabled WRITE setSoundEffectsEnabled NOTIFY soundEffectsEnabledChanged)
    Q_PROPERTY(bool voicePraiseEnabled READ voicePraiseEnabled WRITE setVoicePraiseEnabled NOTIFY voicePraiseEnabledChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    explicit MediaController(QObject *parent = nullptr);

    bool soundEffectsEnabled() const { return m_sfxEnabled; }
    void setSoundEffectsEnabled(bool enabled);

    bool voicePraiseEnabled() const { return m_voiceEnabled; }
    void setVoicePraiseEnabled(bool enabled);

    qreal volume() const { return m_volume; }
    void setVolume(qreal vol);

    // Audio triggers invokable from QML
    Q_INVOKABLE void playPop();
    Q_INVOKABLE void playSparkle();
    Q_INVOKABLE void playFanfare();
    Q_INVOKABLE void playChime();
    Q_INVOKABLE void playDeviationBuzz();
    Q_INVOKABLE void speakPraise(const QString &phrase);
    Q_INVOKABLE void speakShapeName(const QString &shapeName);

signals:
    void soundEffectsEnabledChanged();
    void voicePraiseEnabledChanged();
    void volumeChanged();
    void speechRequested(const QString &phrase);

private:
    bool m_sfxEnabled = true;
    bool m_voiceEnabled = true;
    qreal m_volume = 0.85;

    QAudioOutput *m_audioOutput = nullptr;
    QMediaPlayer *m_player = nullptr;
};
