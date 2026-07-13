#include "whisperloader.h"

#include <QDebug>
#include <QFileInfo>

WhisperLoader::WhisperLoader(QObject *parent)
    : QObject(parent)
{
}

WhisperLoader::~WhisperLoader()
{
    // Safe cleanup if ownership was not transferred
    if (m_ctx)
    {
        whisper_free(m_ctx);
        m_ctx = nullptr;
    }
}

void WhisperLoader::cancel()
{
    m_cancelled = true;
}

/* =========================
 * MAIN LOADER
 * ========================= */
void WhisperLoader::loadModel(const QString &modelPath)
{
    if (m_loading.exchange(true))
    {
        emit failed("Whisper is already loading.");
        return;
    }

    m_cancelled = false;

    emit loadingStarted();
    emit progress("Initializing Whisper context...");

    if (m_cancelled)
    {
        m_loading = false;
        emit failed("Cancelled before start.");
        return;
    }

    whisper_context_params params =
        whisper_context_default_params();

    // --- optional performance tuning ---
    params.use_gpu = false; // safe fallback if unsupported

    emit progress("Loading model from disk...");

    QFileInfo info(modelPath);

    qDebug() << "Loading model:" << modelPath;
    qDebug() << "Exists:" << info.exists();
    qDebug() << "Size:" << info.size();


    whisper_context *ctx =
        whisper_init_from_file_with_params(
            modelPath.toUtf8().constData(),
            params);
      qDebug() << "CTX =" << ctx;

    qDebug() << "Testing model metadata...";

    // Depending on your whisper.cpp version:
    qDebug() << "Vocab:"
             << whisper_model_n_vocab(ctx);

    if (m_cancelled)
    {
        if (ctx)
            whisper_free(ctx);

        m_loading = false;

        emit failed("Cancelled during load.");
        return;
    }

    if (!ctx)
    {
        m_loading = false;
        emit failed("Failed to initialize Whisper context.");
        return;
    }

    emit progress("Finalizing model setup...");

    // assign only after full success
    m_ctx = ctx;

    m_loading = false;

    emit loaded();
}

/* =========================
 * TRANSFER OWNERSHIP
 * ========================= */
whisper_context *WhisperLoader::takeContext()
{
    whisper_context *ctx = m_ctx;
    m_ctx = nullptr;
    return ctx;
}