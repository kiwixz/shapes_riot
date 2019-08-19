#pragma once

#include <array>

namespace utils {

template <typename TElement, int Tdim>
struct Matrix {
    using Self = Matrix<TElement, Tdim>;
    using Element = TElement;
    static constexpr int dim = Tdim;
    static constexpr int size = dim * dim;

    constexpr Matrix() = default;
    ~Matrix() = default;
    constexpr Matrix(const Self&) = default;
    constexpr Self& operator=(const Self&) = default;
    constexpr Matrix(Self&&) noexcept = default;
    constexpr Self& operator=(Self&&) noexcept = default;

    template <typename T>
    constexpr explicit Matrix(const Matrix<T, dim>& other);

    template <typename T>
    constexpr Self& operator=(const Matrix<T, dim>& other);

    constexpr Element& operator()(size_t row, size_t col);
    constexpr Element operator()(size_t row, size_t col) const;

    [[nodiscard]] constexpr Element* ptr();
    [[nodiscard]] constexpr const Element* ptr() const;

private:
    std::array<Element, size> data_{};
};


template <typename T>
using Matrix2 = Matrix<T, 2>;
using Matrix2d = Matrix2<double>;
using Matrix2f = Matrix2<float>;
using Matrix2i = Matrix2<int>;
using Matrix2u = Matrix2<unsigned>;

template <typename T>
using Matrix3 = Matrix<T, 3>;
using Matrix3d = Matrix3<double>;
using Matrix3f = Matrix3<float>;
using Matrix3i = Matrix3<int>;
using Matrix3u = Matrix3<unsigned>;

template <typename T>
using Matrix4 = Matrix<T, 4>;
using Matrix4d = Matrix4<double>;
using Matrix4f = Matrix4<float>;
using Matrix4i = Matrix4<int>;
using Matrix4u = Matrix4<unsigned>;


template <typename TElement, int Tdim>
constexpr bool operator==(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs);

template <typename TElement, int Tdim>
constexpr bool operator!=(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs);


template <typename TElement, int Tdim>
template <typename T>
constexpr Matrix<TElement, Tdim>::Matrix(const Matrix<T, dim>& other)
{
    *this = other;
}

template <typename TElement, int Tdim>
template <typename T>
constexpr Matrix<TElement, Tdim>& Matrix<TElement, Tdim>::operator=(const Matrix<T, dim>& other)
{
    std::copy(other.data_.begin(), other.data_.end(), data_);
    return *this;
}

template <typename TElement, int Tdim>
constexpr TElement& Matrix<TElement, Tdim>::operator()(size_t row, size_t col)
{
    return data_[row * dim + col];
}

template <typename TElement, int Tdim>
constexpr TElement Matrix<TElement, Tdim>::operator()(size_t row, size_t col) const
{
    return data_[row * dim + col];
}

template <typename TElement, int Tdim>
constexpr TElement* Matrix<TElement, Tdim>::ptr()
{
    static_assert(sizeof(Self) == size * sizeof(Element));
    return reinterpret_cast<Element*>(this);
}

template <typename TElement, int Tdim>
constexpr const TElement* Matrix<TElement, Tdim>::ptr() const
{
    static_assert(sizeof(Self) == size * sizeof(Element));
    return reinterpret_cast<const Element*>(this);
}


template <typename TElement, int Tdim>
constexpr bool operator==(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs)
{
    for (int i = 0; i < Matrix<TElement, Tdim>::size; ++i)
        if (lhs.data_[i] != rhs.data_[i])
            return false;
    return true;
}

template <typename TElement, int Tdim>
constexpr bool operator!=(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs)
{
    return !(lhs == rhs);
}

}  // namespace utils
