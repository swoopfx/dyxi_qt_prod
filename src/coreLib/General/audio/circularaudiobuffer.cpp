#include "circularaudiobuffer.h"

#include <algorithm>

CircularAudioBuffer::CircularAudioBuffer(size_t capacity)
    : m_buffer(capacity),
    m_capacity(capacity)
{
}

size_t CircularAudioBuffer::size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_size;
}

void CircularAudioBuffer::push(const float *data,
                               size_t count)
{
    if (!data || count == 0)
        return;

    std::unique_lock<std::mutex> lock(m_mutex);

    for (size_t i = 0; i < count; ++i)
    {
        m_buffer[m_writePos] = data[i];

        m_writePos = (m_writePos + 1) % m_capacity;

        if (m_size < m_capacity)
        {
            ++m_size;
        }
        else
        {
            // Buffer full.
            // Drop oldest sample.
            m_readPos = (m_readPos + 1) % m_capacity;
        }
    }

    lock.unlock();

    m_notEmpty.notify_one();
}

bool CircularAudioBuffer::pop(std::vector<float>& out,
                              size_t count)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_size == 0)
        return false;

    count = std::min(count, m_size);

    out.resize(count);

    for (size_t i = 0; i < count; ++i)
    {
        out[i] = m_buffer[m_readPos];

        m_readPos = (m_readPos + 1) % m_capacity;
    }

    m_size -= count;

    return true;
}

void CircularAudioBuffer::waitAndPop(std::vector<float>& out,
                                     size_t count)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    m_notEmpty.wait(lock,
                    [&]()
                    {
                        return m_size >= count;
                    });

    out.resize(count);

    for (size_t i = 0; i < count; ++i)
    {
        out[i] = m_buffer[m_readPos];

        m_readPos = (m_readPos + 1) % m_capacity;
    }

    m_size -= count;
}

std::vector<float> CircularAudioBuffer::latest(size_t sampleCount)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    sampleCount = std::min(sampleCount, m_size);

    std::vector<float> out(sampleCount);

    size_t start =
        (m_writePos + m_capacity - sampleCount)
        % m_capacity;

    for (size_t i = 0; i < sampleCount; ++i)
    {
        out[i] =
            m_buffer[(start + i) % m_capacity];
    }

    return out;
}

void CircularAudioBuffer::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_readPos = 0;
    m_writePos = 0;
    m_size = 0;
}