#pragma once

#include <array>
#include <type_traits>

#include "utils/pp.h"

namespace utils {

template <typename TElement>
struct CircularIterator {
    using Element = TElement;

    CircularIterator() = default;

    CircularIterator(Element* element, Element* begin, Element* end) :
        element_{element}, begin_{begin}, end_{end}
    {}

    template <typename T, typename std::enable_if_t<std::is_same_v<const T, Element>, int> = 0>
    CircularIterator(const CircularIterator<T>& other) :
        CircularIterator{other.element_, other.begin_, other.end_}
    {}

    Element& operator*() const
    {
        return *element_;
    }
    Element* operator->() const
    {
        return element_;
    }

    CircularIterator<Element>& operator++()
    {
        element_ = (element_ == end_ - 1 ? begin_ : element_ + 1);
        return *this;
    }
    CircularIterator<Element> operator++(int)
    {
        CircularIterator<Element> r = *this;
        ++*this;
        return r;
    }

    CircularIterator<Element>& operator--()
    {
        element_ = (element_ == 0 ? end_ - 1 : element_ - 1);
        return *this;
    }
    CircularIterator<Element> operator--(int)
    {
        CircularIterator<Element> r = *this;
        --*this;
        return r;
    }

#define DEF_OP(op)                                           \
    bool operator op(const CircularIterator<Element>& other) \
    {                                                        \
        return element_ op other.element_;                   \
    }

    DEF_OP(==)
    DEF_OP(!=)
    DEF_OP(<)
    DEF_OP(>)
    DEF_OP(<=)
    DEF_OP(>=)
#undef DEF_OP

private:
    Element* element_ = nullptr;
    Element* begin_;
    Element* end_;
};


template <typename TElement, size_t Tsize>
struct Circular {
    using Element = TElement;

    using ConstIterator = CircularIterator<const Element>;
    using Iterator = CircularIterator<Element>;

    Iterator begin()
    {
        return {storage_.data() + begin_, storage_.data(), storage_.data() + storage_.size()};
    }
    ConstIterator begin() const
    {
        return {storage_.data() + begin_, storage_.data(), storage_.data() + storage_.size()};
    }

    Iterator end()
    {
        return {storage_.data() + end_, storage_.data(), storage_.data() + storage_.size()};
    }
    ConstIterator end() const
    {
        return {storage_.data() + end_, storage_.data(), storage_.data() + storage_.size()};
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

    void push_back(Element element)
    {
        if (size() == max_size())
            pop_front();

        storage_[end_] = std::move(element);
        end_ = increment(end_);
    }

    void push_front(Element element)
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

    size_t increment(size_t index) const
    {
        return index == storage_.size() - 1 ? 0 : index + 1;
    }

    size_t decrement(size_t index) const
    {
        return index == 0 ? storage_.size() - 1 : index - 1;
    }
};

}  // namespace utils
