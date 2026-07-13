#pragma once

#include <vector>
#include <mutex>
#include <algorithm>
#include <cstddef>

#include <vector>
#include <mutex>
#include <condition_variable>

class CircularAudioBuffer
{
public:
    explicit CircularAudioBuffer(size_t capacity = 16000 * 30);

    void push(const float* data, size_t count);

    bool pop(std::vector<float>& out, size_t count);

    void waitAndPop(std::vector<float>& out, size_t count);

    std::vector<float> latest(size_t sampleCount);

    void clear();

    size_t size() const;

private:
    std::vector<float> m_buffer;

    size_t m_capacity;

    size_t m_writePos = 0;
    size_t m_readPos = 0;
    size_t m_size = 0;

    mutable std::mutex m_mutex;
    std::condition_variable m_notEmpty;
};