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

    const Element& operator*() const
    {
        ASSERT(init_);
        return *std::launder(reinterpret_cast<const Element*>(&storage_));
    }
    Element& operator*()
    {
        ASSERT(init_);
        return *std::launder(reinterpret_cast<Element*>(&storage_));
    }

    const Element* operator->() const
    {
        ASSERT(init_);
        return std::launder(reinterpret_cast<const Element*>(&storage_));
    }
    Element* operator->()
    {
        ASSERT(init_);
        return std::launder(reinterpret_cast<Element*>(&storage_));
    }

    template <typename... Args>
    void emplace(Args&&... args)
    {
        ASSERT(!init_);
        new (&storage_) Element{std::forward<Args>(args)...};
#ifdef DEBUG
        ASSERT(!init_);
        init_ = true;
#endif
    }

    void destroy() noexcept
    {
        ASSERT(init_);
        (**this).~Element();
#ifdef DEBUG
        init_ = false;
#endif
    }

private:
    std::aligned_storage_t<sizeof(Element), alignof(Element)> storage_;

#ifdef DEBUG
    bool init_ = false;
#endif
};

}  // namespace utils
