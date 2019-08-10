#pragma once

#include "utils/exception.h"
#include <memory>
#include <string>
#include <unordered_map>

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


struct ResourcesManager {
    template <typename T>
    [[nodiscard]] ResourceHandle<T> get(const std::string& id) const;

    template <typename T, typename F>
    [[nodiscard]] ResourceHandle<T> get(const std::string& id, const F& make);

    template <typename T>
    void get_to(const std::string& id, ResourceHandle<T>& handle) const;

    template <typename T, typename F>
    void get_to(const std::string& id, ResourceHandle<T>& handle, const F& make);

    void cleanup();

private:
    std::unordered_map<std::string, std::weak_ptr<const void>> store_;
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


template <typename T>
ResourceHandle<T> ResourcesManager::get(const std::string& id) const
{
    std::shared_ptr<const void> resource = store_.at(id).lock();
    if (!resource)
        throw MAKE_EXCEPTION("resource expired");
    return ResourceHandle<T>{std::reinterpret_pointer_cast<const T>(resource)};
}

template <typename T, typename F>
ResourceHandle<T> ResourcesManager::get(const std::string& id, const F& make)
{
    std::pair ret = store_.try_emplace(id);
    auto it = ret.first;

    std::shared_ptr<const void> resource;
    if (ret.second) {  // inserted an empty weak_ptr
        resource = make();
        it->second = resource;
    }
    else
        resource = it->second.lock();

    return ResourceHandle<T>{std::reinterpret_pointer_cast<const T>(resource)};
}

template <typename T>
void ResourcesManager::get_to(const std::string& id, ResourceHandle<T>& handle) const
{
    handle = get<T>(id);
}

template <typename T, typename F>
void ResourcesManager::get_to(const std::string& id, ResourceHandle<T>& handle, const F& make)
{
    handle = get<T>(id, make);
}

}  // namespace utils
