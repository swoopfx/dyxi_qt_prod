#ifndef AUDIOMANIPULATOR_H
#define AUDIOMANIPULATOR_H

#include <QObject>
#include <QQmlEngine>
#include <QVector>
#include <QByteArray>
#include <QtMath>
#include <QAudioFormat>
#include <QAudioBuffer>
#include <QVector>
#include <QtEndian>


class AudioManipulator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AudioManipulator(QObject *parent = nullptr);
    QVector<qint16> convertToPcm16kMono(const QByteArray &inputData, int originalSampleRate, int originalChannels, int bytesPerSample);

    // QVector<float> convertToPcm16kMonoFloat(const QByteArray &inputData, int originalSampleRate, int originalChannels, int bytesPerSample);

    QVector<float> convertToNormalizedMono(const QByteArray& inputData, const QAudioFormat& sourceFormat);

signals:
};

#endif // AUDIOMANIPULATOR_H
