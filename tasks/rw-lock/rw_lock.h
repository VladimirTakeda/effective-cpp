#pragma once
#include <mutex>
#include <condition_variable>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        mutex_.lock();
        ++blocked_readers_;
        mutex_.unlock();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    template <class Func>
    void Write(Func func) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (blocked_readers_ > 0) {
            cv_.wait(lock);
        }
        func();
        cv_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;

    int blocked_readers_ = 0;

    void EndRead() {
        mutex_.lock();
        --blocked_readers_;
        if (blocked_readers_ == 0) {
            cv_.notify_all();
        }
        mutex_.unlock();
    }
};
