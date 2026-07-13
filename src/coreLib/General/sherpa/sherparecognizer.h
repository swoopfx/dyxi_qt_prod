#ifndef SHERPARECOGNIZER_H
#define SHERPARECOGNIZER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMutex>


#include "../../Sherpa/sherpa-ncnn/sherpa-ncnn/c-api/c-api.h"


struct RecognitionResult
{
    QString partialText;
    QString finalText;
    bool endpoint = false;
};

class SherpaRecognizer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool initialized READ isInitialized NOTIFY initializedChanged)
    Q_PROPERTY(QString partialResult READ partialResult NOTIFY partialResultChanged)
    Q_PROPERTY(QString finalResult READ finalResult NOTIFY finalResultChanged)

public:
    explicit SherpaRecognizer(QObject *parent = nullptr);
    ~SherpaRecognizer();

    /**
     * @brief Loads the models and prepares the recognizer.
     */
    bool initialize(
        const QString &encoderParam,
        const QString &encoderBin,
        const QString &decoderParam,
        const QString &decoderBin,
        const QString &joinerParam,
        const QString &joinerBin,
        const QString &tokens);

    /**
     * @brief Cleans up Sherpa resources and resets state.
     */
    void destroy();

    /**
     * @brief Feeds audio samples into the recognizer.
     * @param samples Audio buffer (PCM Float 32-bit).
     * @param sampleRate Typically 16000.
     */
    void acceptWaveform(const QVector<float> &samples, float sampleRate = 16000.0f);
    RecognitionResult processAudio(const QVector<float> &samples,  float sampleRate = 16000.0f);

    /**
     * @brief Signals that no more audio is coming for the current stream.
     */
    void inputFinished();

    /**
     * @brief Resets the internal decoder state.
     */
    void reset();

    // Getters
    bool isInitialized() const { return mInitialized; }
    QString partialResult() const;
    QString finalResult() const;

signals:
    void errorOccurred(const QString &message);
    void initializedChanged();
    void partialResultChanged(const QString &text);
    void finalResultChanged(const QString &text);
    void endpointDetected();

private:
    // Internal processing helpers (called with mutex held)
    RecognitionResult decodeInternal();
    RecognitionResult updateResultInternal();

    mutable QMutex mMutex;

    // Sherpa NCNN handles
    SherpaNcnnRecognizer *mRecognizer = nullptr;
    SherpaNcnnStream *mStream = nullptr;

    bool mInitialized = false;
    QString mPartialResult;
    QString mFinalResult;
};

#endif // SHERPARECOGNIZER_H