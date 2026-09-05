#include "MediaController.hpp"
#include <QDebug>
#include <QUrl>

MediaController::MediaController(QObject *parent)
    : QObject(parent)
{
    m_audioOutput = new QAudioOutput(this);
    m_audioOutput->setVolume(static_cast<float>(m_volume));

    m_player = new QMediaPlayer(this);
    m_player->setAudioOutput(m_audioOutput);
}

void MediaController::setSoundEffectsEnabled(bool enabled) {
    if (m_sfxEnabled != enabled) {
        m_sfxEnabled = enabled;
        emit soundEffectsEnabledChanged();
    }
}

void MediaController::setVoicePraiseEnabled(bool enabled) {
    if (m_voiceEnabled != enabled) {
        m_voiceEnabled = enabled;
        emit voicePraiseEnabledChanged();
    }
}

void MediaController::setVolume(qreal vol) {
    vol = qBound(0.0, vol, 1.0);
    if (!qFuzzyCompare(m_volume, vol)) {
        m_volume = vol;
        if (m_audioOutput) {
            m_audioOutput->setVolume(static_cast<float>(m_volume));
        }
        emit volumeChanged();
    }
}

void MediaController::playPop() {
    if (!m_sfxEnabled) return;
    qDebug() << "[C++ MediaController] Playing Pop SFX";
    m_player->setSource(QUrl("qrc:/LittleScribblers/sounds/pop.wav"));
    m_player->play();
}

void MediaController::playSparkle() {
    if (!m_sfxEnabled) return;
    qDebug() << "[C++ MediaController] Playing Sparkle SFX";
    m_player->setSource(QUrl("qrc:/LittleScribblers/sounds/sparkle.wav"));
    m_player->play();
}

void MediaController::playFanfare() {
    if (!m_sfxEnabled) return;
    qDebug() << "[C++ MediaController] Playing Fanfare Celebration SFX";
    m_player->setSource(QUrl("qrc:/LittleScribblers/sounds/fanfare.wav"));
    m_player->play();
}

void MediaController::playChime() {
    if (!m_sfxEnabled) return;
    qDebug() << "[C++ MediaController] Playing Star Chime SFX";
    m_player->setSource(QUrl("qrc:/LittleScribblers/sounds/chime.wav"));
    m_player->play();
}

void MediaController::playDeviationBuzz() {
    if (!m_sfxEnabled) return;
    qDebug() << "[C++ MediaController] Playing Subtle Corridor Deviation Buzz";
}

void MediaController::speakPraise(const QString &phrase) {
    if (!m_voiceEnabled) return;
    qDebug() << "[C++ MediaController] TTS Praise:" << phrase;
    emit speechRequested(phrase);
}

void MediaController::speakShapeName(const QString &shapeName) {
    if (!m_voiceEnabled) return;
    qDebug() << "[C++ MediaController] Announcing shape:" << shapeName;
    emit speechRequested(QString("Let's trace the %1!").arg(shapeName));
}
