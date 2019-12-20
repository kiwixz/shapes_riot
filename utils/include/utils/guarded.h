#pragma once

#include <memory>
#include <mutex>
#include <optional>

#include "utils/is_pack_same.h"

namespace utils {

template <typename TType, typename TMutex = std::mutex>
struct Guarded {
    using Self = Guarded<TType, TMutex>;
    using Type = TType;
    using Mutex = TMutex;

    struct Handle {
        Handle() = default;
        Handle(Type& native, std::unique_lock<Mutex>&& lock);

        Type& operator*() const;
        Type* operator->() const;

    private:
        Type* native_ = nullptr;
        std::unique_lock<Mutex> lock_;
    };

    ~Guarded() = default;
    Guarded(const Self&) = delete;
    Self& operator=(const Self&) = delete;
    Guarded(Self&&) noexcept = default;
    Self& operator=(Self&&) noexcept = default;

    template <typename... Args, std::enable_if_t<!is_pack_same<Self, Args...>, int> = 0>
    Guarded(Args&&... args);

    Handle lock();                     ///< Thread-safe
    std::optional<Handle> try_lock();  ///< Thread-safe

private:
    Type native_;
    Mutex mutex_;
};


template <typename TType, typename TMutex>
template <typename... Args, std::enable_if_t<!is_pack_same<Guarded<TType, TMutex>, Args...>, int>>
Guarded<TType, TMutex>::Guarded(Args&&... args) :
    native_{std::forward<Args>(args)...}
{}

template <typename TType, typename TMutex>
typename Guarded<TType, TMutex>::Handle Guarded<TType, TMutex>::lock()
{
    return {native_, std::unique_lock{mutex_}};
}

template <typename TType, typename TMutex>
std::optional<typename Guarded<TType, TMutex>::Handle> Guarded<TType, TMutex>::try_lock()
{
    std::unique_lock<Mutex> lock{mutex_, std::try_to_lock};
    if (!lock)
        return {};
    return {{native_, std::move(lock)}};
}


template <typename TType, typename TMutex>
Guarded<TType, TMutex>::Handle::Handle(Type& native, std::unique_lock<Mutex>&& lock) :
    native_{&native}, lock_{std::move(lock)}
{}

template <typename TType, typename TMutex>
TType& Guarded<TType, TMutex>::Handle::operator*() const
{
    return *native_;
}

template <typename TType, typename TMutex>
TType* Guarded<TType, TMutex>::Handle::operator->() const
{
    return native_;
}

}  // namespace utils
