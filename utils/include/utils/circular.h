#pragma once

#include <array>
#include <type_traits>

#include "utils/pp.h"

namespace utils {

template <typename TElement, size_t Tsize>
struct Circular {
    using Element = TElement;

    template <typename TElement>
    struct IteratorImpl {
        using Element = TElement;

        IteratorImpl() = default;

        IteratorImpl(Element* element, const Element* begin, const Element* end) :
            element_{element}, begin_{begin}, end_{end}
        {}

        template <typename T, typename std::enable_if_t<std::is_same_v<const T, Element>, int> = 0>
        IteratorImpl(const IteratorImpl<T>& other) :
            IteratorImpl{other.element_, other.begin_, other.end_}
        {}

    private:
        Element* element_ = nullptr;
        const Element* begin_;
        const Element* end_;
    };

    using ConstIterator = IteratorImpl<const Element>;
    using Iterator = IteratorImpl<Element>;

    Iterator begin()
    {
        return Iterator{storage_.data() + begin_};
    }
    ConstIterator begin() const
    {
        return ConstIterator{storage_.data() + begin_};
    }

    Iterator end()
    {
        return Iterator{storage_.data() + end_};
    }
    ConstIterator end() const
    {
        return ConstIterator{storage_.data() + end_};
    }

    bool empty() const
    {
        return begin_ == end_;
    }

    size_t size() const
    {
        return begin_ <= end_ ? end_ - begin_ : (storage_.size() - begin_) + end_;
    }
    size_t max_size() const
    {
        return Tsize;
    }

    Element& front()
    {
        ASSERT(size() > 0);
        return storage_[begin_];
    }
    const Element& front() const
    {
        ASSERT(size() > 0);
        return storage_[begin_];
    }

    Element& back()
    {
        ASSERT(size() > 0);
        return storage_[decrement(end_)];
    }
    const Element& back() const
    {
        ASSERT(size() > 0);
        return storage_[decrement(end_)];
    }

    void pop_back()
    {
        ASSERT(size() > 0);
        end_ = decrement(end_);
    }

    void pop_front()
    {
        ASSERT(size() > 0);
        begin_ = increment(begin_);
    }

    void push_back(Element&& element)
    {
        if (size() == max_size())
            pop_front();

        storage_[end_] = std::move(element);
        end_ = increment(end_);
    }

    void push_front(Element&& element)
    {
        if (size() == max_size())
            pop_back();

        begin_ = decrement(begin_);
        storage_[begin_] = std::move(element);
    }

private:
    // we need Tsize+1 to ensure that begin_!=end_ when full
    std::array<Element, Tsize + 1> storage_;
    size_t begin_ = 0;
    size_t end_ = 0;

    size_t increment(size_t index)
    {
        return index == storage_.size() - 1 ? 0 : index + 1;
    }

    size_t decrement(size_t index)
    {
        return index == 0 ? storage_.size() - 1 : index - 1;
    }
};

}  // namespace utils
