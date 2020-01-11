#pragma once

#include <new>
#include <type_traits>

#include "utils/pp.h"

namespace utils {

/// Allow custom lifetime management.  You have to manually destroy objects you create.
template <typename TElement>
struct Uninitialized {
    using Element = TElement;

#ifdef DEBUG
    ~Uninitialized()
    {
        ASSERT(!init_);
    }
#endif

    Element& operator*() const
    {
        return *std::launder(reinterpret_cast<const Element*>(&storage_));
    }
    Element& operator*()
    {
        return *std::launder(reinterpret_cast<Element*>(&storage_));
    }

    Element* operator->() const
    {
        return std::launder(reinterpret_cast<const Element*>(&storage_));
    }
    Element* operator->()
    {
        return std::launder(reinterpret_cast<Element*>(&storage_));
    }

    template <typename... Args>
    void emplace(Args&&... args)
    {
#ifdef DEBUG
        ASSERT(!init_);
        init_ = true;
#endif
        new (&storage_) Element{std::forward<Args>(args)...};
    }

    void destroy() noexcept
    {
#ifdef DEBUG
        ASSERT(init_);
        init_ = false;
#endif
        (*this)->~Element();
    }

private:
    std::aligned_storage_t<sizeof(Element), alignof(Element)> storage_;

#ifdef DEBUG
    bool init_ = false;
#endif
};

}  // namespace utils
