#include "whisperworker.h"

#include <QThread>
#include <QElapsedTimer>
#include <cmath>

static constexpr int SAMPLE_RATE = 16000;
const int WINDOW = 16000 * 5;

static constexpr int WINDOW_SECONDS = 5;
static constexpr int WINDOW_SIZE = SAMPLE_RATE * WINDOW_SECONDS;

static constexpr int INTERVAL_MS = 250;
// static constexpr int INTERVAL_MS = 1000; // 1 second

WhisperWorker::WhisperWorker(CircularAudioBuffer *buffer,
                             whisper_context *ctx)
    : m_buffer(buffer),
    m_ctx(ctx)
{
}

void WhisperWorker::stop()
{
    m_running = false;
}

float WhisperWorker::energy(const float *data, int n)
{
    float sum = 0.0f;

    for (int i = 0; i < n; ++i)
        sum += data[i] * data[i];

    return std::sqrt(sum / n);
}

bool WhisperWorker::isSpeech(const std::vector<float> &audio)
{
    if (audio.empty())
        return false;

    return energy(audio.data(), audio.size()) > 0.008f;
}



QString WhisperWorker::extractText()
{
    QString result;

    int segments =
        whisper_full_n_segments(m_ctx);

    for (int i = 0; i < segments; i++)
    {
        result += QString::fromUtf8(
            whisper_full_get_segment_text(
                m_ctx,
                i));
    }

    return result.trimmed();
}



void WhisperWorker::run()
{
    qDebug() << "Whisper worker started";

    // Create ONE state for this worker
    m_state = whisper_init_state(m_ctx);

    qDebug() << "CTX =" << m_ctx;
    qDebug() << "STATE =" << m_state;
    qDebug() << whisper_print_system_info();

    if (!m_state)
    {
        qDebug() << "Failed to create whisper_state";
        return;
    }

    qDebug() << "State created:" << m_state;

    constexpr int SAMPLE_RATE = 16000;
    constexpr int WINDOW_SIZE = SAMPLE_RATE * 5;

    while (m_running)
    {
        QThread::msleep(250);

        auto audio =
            m_buffer->latest(WINDOW_SIZE);

        if (audio.size() < SAMPLE_RATE)
            continue;

        if (!isSpeech(audio))
            continue;

        // whisper_full_params params =
        //     whisper_full_default_params(
        //         WHISPER_SAMPLING_GREEDY);

        // params.print_progress = false;
        // params.print_realtime = false;
        // params.print_special = false;

        // params.translate = false;

        // params.no_context = true;
        // params.single_segment = false;

        // params.n_threads = 1;
        // params.max_tokens = 64;

        // qDebug()
        //     << "Running whisper on"
        //     << audio.size()
        //     << "samples";

        // int ret =
        //     whisper_full_with_state(
        //         m_ctx,
        //         m_state,
        //         params,
        //         audio.data(),
        //         static_cast<int>(audio.size()));

        // qDebug() << "Whisper ret =" << ret;

        qDebug() << "Running synthetic inference";

        std::vector<float> testAudio(16000);

        for (int i = 0; i < 16000; ++i)
        {
            testAudio[i] =
                0.5f * sinf(
                    2.0f * M_PI * 440.0f *
                    i / 16000.0f);
        }

        whisper_full_params params =
            whisper_full_default_params(
                WHISPER_SAMPLING_GREEDY);

        params.n_threads = 1;
        params.no_context = true;
        params.single_segment = true;

        std::vector<float> silence(16000 * 5, 0.0f);
         qDebug() << whisper_print_system_info();

         qDebug() << "SILENCE BEGIN " ;
        int ret =
            whisper_full_with_state(
                m_ctx,
                m_state,
                params,
                silence.data(),
                silence.size());

        qDebug() << "SILENCE RET =" << ret;

        if (ret != 0)
            continue;

        QString text = extractText();

        if (text.isEmpty())
            continue;

        if (!m_lastText.isEmpty() &&
            text.startsWith(m_lastText))
        {
            QString delta =
                text.mid(m_lastText.size())
                    .trimmed();

            if (!delta.isEmpty())
            {
                emit textReady(delta);
            }
        }
        else
        {
            emit textReady(text);
        }

        m_lastText = text;
    }

    qDebug() << "Whisper worker stopped";
}

WhisperWorker::~WhisperWorker()
{
    if (m_state)
    {
        whisper_free_state(m_state);
        m_state = nullptr;
    }
}