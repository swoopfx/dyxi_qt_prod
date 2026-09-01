#include "audio_synth.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QTimer>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AudioSynth::AudioSynth(QObject *parent) : QObject(parent) {
    // Configure standard PCM 16-bit audio format for Qt 6
    m_format.setSampleRate(44100);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioOutput();
    m_audioSink = new QAudioSink(defaultDevice, m_format, this);
}

AudioSynth::~AudioSynth() {
    if (m_audioSink) {
        m_audioSink->stop();
    }
}

void AudioSynth::playTone(double frequency, int durationMs, double volume) {
    if (!m_audioSink) return;

    int sampleRate = m_format.sampleRate();
    int numSamples = (sampleRate * durationMs) / 1000;
    QByteArray buffer;
    buffer.resize(numSamples * 2); // 16-bit sound needs 2 bytes per sample

    int16_t *data = reinterpret_cast<int16_t*>(buffer.data());
    for (int i = 0; i < numSamples; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double sampleValue = std::sin(2.0 * M_PI * frequency * t);
        
        // Add a micro fade-out to prevent popping or cracking noise
        if (i > numSamples - 441) {
            double fade = static_cast<double>(numSamples - i) / 441.0;
            sampleValue *= fade;
        }
        
        data[i] = static_cast<int16_t>(sampleValue * 32767.0 * volume);
    }

    // Direct buffer write
    m_audioSink->stop();
    m_audioDevice = m_audioSink->start();
    if (m_audioDevice) {
        m_audioDevice->write(buffer);
    }
}

void AudioSynth::playHapticSound(const QString &type, double frequency, int durationMs) {
    if (type == "double") {
        playTone(frequency, durationMs / 2, 0.4);
        QTimer::singleShot(durationMs / 2 + 10, this, [this, frequency, durationMs]() {
            playTone(frequency * 1.1, durationMs / 2, 0.4);
        });
    } else if (type == "heavy") {
        playTone(frequency, durationMs, 0.7);
    } else if (type == "pulse") {
        playTone(frequency, durationMs, 0.5);
    } else {
        playTone(frequency, durationMs, 0.3);
    }
}

void AudioSynth::playTransition(const QVariantList &notes) {
    if (notes.isEmpty()) return;
    int delay = 0;
    for (int i = 0; i < notes.size(); ++i) {
        double noteFreq = notes[i].toDouble();
        QTimer::singleShot(delay, this, [this, noteFreq]() {
            playTone(noteFreq, 120, 0.4);
        });
        delay += 100;
    }
}

void AudioSynth::playMatchFound() {
    // Beautiful C Major arpeggio chord sequence
    QList<double> chord = {261.63, 329.63, 392.00, 523.25};
    int delay = 0;
    for (double freq : chord) {
        QTimer::singleShot(delay, this, [this, freq]() {
            playTone(freq, 200, 0.4);
        });
        delay += 90;
    }
}