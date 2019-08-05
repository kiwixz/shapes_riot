#pragma once

#include <cstddef>

namespace utils {

template <typename TElement>
struct Span {
    using Element = TElement;

    constexpr Span() = default;
    constexpr Span(Element* data, size_t size);

    template <typename T>
    constexpr Span(T& container);

    constexpr Element& operator[](size_t idx) const;

    constexpr Element* begin() const;
    constexpr Element* data() const;
    constexpr Element* end() const;
    constexpr size_t size() const;


private:
    Element* data_;
    size_t size_;
};


template <typename TElement>
template <typename T>
constexpr Span<TElement>::Span(T& container) :
    Span{container.data(), container.size()}
{}

template <typename TElement>
constexpr Span<TElement>::Span(Element* data, size_t size) :
    data_{data},
    size_{size}
{}

template <typename TElement>
constexpr TElement& Span<TElement>::operator[](size_t idx) const
{
    return data_[idx];
}

template <typename TElement>
constexpr TElement* Span<TElement>::begin() const
{
    return data_;
}

template <typename TElement>
constexpr TElement* Span<TElement>::data() const
{
    return data_;
}

template <typename TElement>
constexpr TElement* Span<TElement>::end() const
{
    return data_ + size_;
}

template <typename TElement>
constexpr size_t Span<TElement>::size() const
{
    return size_;
}

}  // namespace utils
