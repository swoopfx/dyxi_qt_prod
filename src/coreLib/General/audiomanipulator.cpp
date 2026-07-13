#include "audiomanipulator.h"

AudioManipulator::AudioManipulator(QObject *parent)
    : QObject{parent}
{}

QVector<qint16> AudioManipulator::convertToPcm16kMono(const QByteArray &inputData, int originalSampleRate, int originalChannels, int bytesPerSample)
{
    QVector<qint16> pcmData;

    // 1. First, convert original data to flat float array (-1.0 to 1.0)
    QVector<float> floatAudio;
    floatAudio.reserve(inputData.size() / bytesPerSample);

    const unsigned char *data = reinterpret_cast<const unsigned char*>(inputData.constData());
    for(int i = 0; i < inputData.size(); i += bytesPerSample) {
        float val = 0.0f;
        if (bytesPerSample == 2) {
            val = static_cast<float>(*reinterpret_cast<const qint16*>(data + i)) / 32768.0f;
        } else if (bytesPerSample == 1) {
            val = (static_cast<float>(*reinterpret_cast<const quint8*>(data + i)) - 128.0f) / 128.0f;
        }
        floatAudio.append(val);
    }

    // 2. Downmix to Mono and Resample to 16 kHz
    const int targetRate = 16000;
    const float durationSeconds = static_cast<float>(floatAudio.size() / originalChannels) / originalSampleRate;
    const int totalTargetSamples = static_cast<int>(durationSeconds * targetRate);

    pcmData.reserve(totalTargetSamples);

    for (int i = 0; i < totalTargetSamples; ++i) {
        // Find corresponding floating point sample index in the original audio
        float targetTime = static_cast<float>(i) / targetRate;
        float originalPos = targetTime * originalSampleRate;

        int p1 = qFloor(originalPos);
        int p2 = qCeil(originalPos);
        float fraction = originalPos - p1;

        // Ensure indices are within bounds
        if (p2 >= floatAudio.size() / originalChannels) p2 = p1;

        // Handle multi-channel to mono mix & interpolate
        float sample1 = 0.0f;
        float sample2 = 0.0f;
        for(int c = 0; c < originalChannels; ++c) {
            sample1 += floatAudio[(p1 * originalChannels) + c];
            sample2 += floatAudio[(p2 * originalChannels) + c];
        }
        sample1 /= originalChannels;
        sample2 /= originalChannels;

        float monoSample = sample1 + (sample2 - sample1) * fraction;

        // 3. Normalize (clamp to [-1.0, 1.0] and convert to 16-bit integer)
        monoSample = qBound(-1.0f, monoSample, 1.0f);
        pcmData.append(static_cast<qint16>(monoSample * 32767.0f));
    }

    return pcmData;
}

// QVector<float> AudioManipulator::convertToPcm16kMonoFloat(const QByteArray &inputData, int originalSampleRate, int originalChannels, int bytesPerSample)
// {

// }

QVector<float> AudioManipulator::convertToNormalizedMono(const QByteArray &inputData, const QAudioFormat &sourceFormat)
{

    QVector<float> normalizedAudio;

    // 1. Initialize input buffer
    QAudioBuffer audioBuffer(inputData, sourceFormat);

    // 2. We need mono, 16-bit signed PCM (S16LE is standard for 16kHz)
    QAudioFormat targetFormat;
    targetFormat.setSampleRate(16000);
    targetFormat.setChannelCount(1);
    targetFormat.setSampleFormat(QAudioFormat::Int16); // or QAudioFormat::Float

    // NOTE: For Qt 6, utilize QAudioConverter for format & sample rate conversion.
    // Ensure the input audio data is converted/resampled to the target spec before processing.
    // (Assuming audioBuffer now contains 16kHz mono PCM data due to your prior resampling step)

    // 3. Setup normalization boundary
    // Max value for 16-bit signed is 32767.0
    const float normalizationFactor = 1.0f / 32767.0f;

    // 4. Retrieve audio data to normalize
    const qint16* data = audioBuffer.constData<qint16>();
    int sampleCount = audioBuffer.frameCount() * audioBuffer.format().channelCount();

    normalizedAudio.reserve(sampleCount);

    // 5. Convert and normalize
    for (int i = 0; i < sampleCount; ++i) {
        // Convert to float, then scale down to [-1.0, 1.0]
        float sample = static_cast<float>(data[i]) * normalizationFactor;

        // Ensure bounds are strictly clipped between -1.0 and 1.0
        sample = qBound(-1.0f, sample, 1.0f);

        normalizedAudio.append(sample);
    }

    return normalizedAudio;
}
