#pragma once

#include <deque>
#include <mutex>
#include <thread>

template <typename T>
class TSqueue
{
public:
    TSqueue() = default;
    virtual ~TSqueue() = default;

    TSqueue(const TSqueue &) = delete;
    TSqueue& operator=(const TSqueue &) = delete;

public:
    void push_back(T item)
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        tsqueue.push_back(item);
    }

    void push_front(T item)
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        tsqueue.push_front(item);
    }

    T pop_back()
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        T item = tsqueue.back();
        tsqueue.pop_back();
        return item;
    }

    T pop_front()
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        T item = tsqueue.front();
        tsqueue.pop_front();
        return item;
    }

    T front()
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        return tsqueue.front();
    }

    T back()
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        return tsqueue.back();
    }

    uint32_t size()
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        return tsqueue.size();
    }

    bool isEmpty()
    {
        std::lock_guard<std::mutex> lock_guard(mutex);
        return tsqueue.empty();
    }

private:
    std::mutex mutex;
    std::deque<T> tsqueue;
};