#pragma once

#include <memory>

namespace utils {

template <typename TType>
struct ResourceHandle {
    using Type = TType;

    explicit ResourceHandle(std::shared_ptr<const TType> resource);

    [[nodiscard]] const Type* operator->() const;
    [[nodiscard]] const Type& operator*() const;

private:
    std::shared_ptr<const TType> resource_;
};


template <typename TType>
ResourceHandle<TType>::ResourceHandle(std::shared_ptr<const TType> resource) :
    resource_{std::move(resource)}
{}

template <typename TType>
const TType* ResourceHandle<TType>::operator->() const
{
    return resource_.get();
}

template <typename TType>
const TType& ResourceHandle<TType>::operator*() const
{
    return *resource_;
}

}  // namespace utils
