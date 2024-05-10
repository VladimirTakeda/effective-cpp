#pragma once

#include <utility>
#include <optional>
#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : buffer_size(size) {}

    void Send(const T& value) {
        std::unique_lock<std::mutex> lock(bufferLock);
        inputWait.wait(lock, [this] { return buffer_.size() < buffer_size || is_closed; });
        if (!is_closed) {
            buffer_.push_back(value);
            outputWait.notify_one();
        }
        else{
            throw std::runtime_error("the channel was closed");
        }
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> lock(bufferLock);
        outputWait.wait(lock, [this] { return !buffer_.empty() || is_closed; });
        if (buffer_.empty() && is_closed) {
            return std::nullopt;
        }
        T value = std::move(buffer_.front());
        buffer_.pop_front();
        inputWait.notify_one();
        return std::optional<T>(std::move(value));
    }

    void Close() {
        std::lock_guard<std::mutex> lock(bufferLock);
        is_closed = true;
        inputWait.notify_all();
        outputWait.notify_all();
    }

private:
    std::deque<T> buffer_;
    const int buffer_size;
    std::atomic_bool is_closed = false;
    std::mutex bufferLock;
    std::condition_variable inputWait;
    std::condition_variable outputWait;
};