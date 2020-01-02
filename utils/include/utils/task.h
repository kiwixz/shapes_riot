#pragma once

#include <exception>
#include <future>

#include "utils/function.h"

namespace utils {

template <typename, typename>
struct TaskImpl;

template <typename Function, typename Result, typename... Args>
struct TaskImpl<Function, Result(Args...)> {
    TaskImpl() = default;

    TaskImpl(Function function)
    {
        function_ = [this, function = std::move(function)](Args... args) {
            try {
                if constexpr (std::is_same_v<Result, void>) {
                    function(std::forward<Args>(args)...);
                    promise_.set_value();
                }
                else {
                    promise_.set_value(function(std::forward<Args>(args)...));
                }
            }
            catch (...) {
                promise_.set_exception(std::current_exception());
            }
        };
    }

    explicit operator bool() const
    {
        return function_;
    }

    std::future<Result> get_future()
    {
        return promise_.get_future();
    }

    Result operator()(Args... args) const
    {
        return function_(std::forward<Args>(args)...);
    }

private:
    std::promise<Result> promise_;
    Function function_;
};


template <typename... Args>
using Task = TaskImpl<Function<Args...>, Args...>;

template <typename... Args>
using UniqueTask = TaskImpl<UniqueFunction<Args...>, Args...>;

}  // namespace utils
