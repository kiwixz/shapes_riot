#pragma once

#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

#include "utils/exception.h"
#include "utils/function.h"
#include "utils/task.h"

namespace utils {

struct ThreadPool {
    ThreadPool() = default;
    explicit ThreadPool(size_t nr_threads);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) noexcept = delete;
    ThreadPool& operator=(ThreadPool&&) noexcept = delete;

    size_t size() const;
    void extend(size_t nr_threads);

    /// Callable only needs to be moveable, but args must be copyable.
    template <typename T, typename... Args>
    std::future<std::invoke_result_t<T, Args...>> submit(T&& callable, Args&&... args);

private:
    std::vector<std::thread> workers_;

    std::mutex tasks_mutex_;
    std::condition_variable tasks_condvar_;
    std::queue<utils::UniqueFunction<void()>> tasks_;
    bool stopping_ = false;
};


template <typename T, typename... Args>
std::future<std::invoke_result_t<T, Args...>> ThreadPool::submit(T&& callable, Args&&... args)
{
    using ReturnType = std::invoke_result_t<T, Args...>;

    utils::UniqueTask<ReturnType()> task{std::bind(std::forward<T>(callable), std::forward<Args>(args)...)};
    std::future<ReturnType> future = std::move(task.future());

    {
        std::lock_guard lock{tasks_mutex_};
        if (stopping_)
            throw MAKE_EXCEPTION("trying to add work on stopping thread pool");

        tasks_.emplace([task = std::move(task)] {
            task();
        });
    }

    tasks_condvar_.notify_one();
    return future;
}

}  // namespace utils
