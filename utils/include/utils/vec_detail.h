#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>

namespace utils::vec_detail {

struct VecBase {};  // simplify sfinae

template <typename T>
constexpr bool is_vec = std::is_base_of_v<VecBase, T>;


template <template <typename> typename TVec, typename TElement, int Tsize>
struct VecMeta : VecBase {
    using Vec = TVec<TElement>;
    using Element = TElement;
    static constexpr int size = Tsize;

    constexpr Element& operator[](size_t idx);
    constexpr Element operator[](size_t idx) const;

    [[nodiscard]] constexpr Element dot(const Vec& other) const;
    [[nodiscard]] constexpr Element length() const;
    [[nodiscard]] constexpr Element* ptr();
    [[nodiscard]] constexpr const Element* ptr() const;

    constexpr void clamp(Element min, Element max);
    constexpr void normalize();

private:
    [[nodiscard]] constexpr Vec& self();
    [[nodiscard]] constexpr const Vec& self() const;
};


template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr bool operator==(const TVec<TElement>& lhs, const TVec<TElement>& rhs);

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr bool operator!=(const TVec<TElement>& lhs, const TVec<TElement>& rhs);

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr TVec<TElement> operator-(const TVec<TElement>& lhs);

#define DEF_OP(op, type, operand)                                        \
    template <template <typename> typename TVec, typename TElement,      \
              std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>         \
    constexpr TVec<TElement>& operator op(TVec<TElement>& lhs, type rhs) \
    {                                                                    \
        for (int i = 0; i < TVec<TElement>::size; ++i)                   \
            lhs[i] op operand;                                           \
        return lhs;                                                      \
    }

DEF_OP(+=, const TVec<TElement>&, rhs[i])
DEF_OP(-=, const TVec<TElement>&, rhs[i])
DEF_OP(*=, const TVec<TElement>&, rhs[i])
DEF_OP(/=, const TVec<TElement>&, rhs[i])
DEF_OP(+=, TElement, rhs)
DEF_OP(-=, TElement, rhs)
DEF_OP(*=, TElement, rhs)
DEF_OP(/=, TElement, rhs)
#undef DEF_OP

#define DEF_OP(op, type)                                                      \
    template <template <typename> typename TVec, typename TElement,           \
              std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>              \
    constexpr TVec<TElement> operator op(const TVec<TElement>& lhs, type rhs) \
    {                                                                         \
        TVec<TElement> vec = lhs;                                             \
        vec op## = rhs;                                                       \
        return vec;                                                           \
    }

DEF_OP(+, const TVec<TElement>&)
DEF_OP(-, const TVec<TElement>&)
DEF_OP(*, const TVec<TElement>&)
DEF_OP(/, const TVec<TElement>&)
DEF_OP(+, TElement)
DEF_OP(-, TElement)
DEF_OP(*, TElement)
DEF_OP(/, TElement)
#undef DEF_OP

#define DEF_OP(op)                                                                \
    template <template <typename> typename TVec, typename TElement,               \
              std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>                  \
    constexpr TVec<TElement> operator op(TElement lhs, const TVec<TElement>& rhs) \
    {                                                                             \
        TVec<TElement> vec;                                                       \
        for (int i = 0; i < TVec<TElement>::size; ++i)                            \
            vec[i] = lhs op rhs[i];                                               \
        return vec;                                                               \
    }

DEF_OP(+)
DEF_OP(-)
DEF_OP(*)
DEF_OP(/)
#undef DEF_OP


template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int>>
constexpr bool operator==(const TVec<TElement>& lhs, const TVec<TElement>& rhs)
{
    for (int i = 0; i < TVec<TElement>::size; ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int>>
constexpr bool operator!=(const TVec<TElement>& lhs, const TVec<TElement>& rhs)
{
    return !(lhs == rhs);
}

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int>>
constexpr TVec<TElement> operator-(const TVec<TElement>& lhs)
{
    TVec<TElement> vec;
    for (int i = 0; i < TVec<TElement>::size; ++i)
        vec[i] = -lhs[i];
    return vec;
}


template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement& VecMeta<TVec, TElement, Tsize>::operator[](size_t idx)
{
    return reinterpret_cast<Element*>(this)[idx];
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement VecMeta<TVec, TElement, Tsize>::operator[](size_t idx) const
{
    return reinterpret_cast<const Element*>(this)[idx];
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement* VecMeta<TVec, TElement, Tsize>::ptr()
{
    static_assert(sizeof(Vec) == size * sizeof(Element));
    return reinterpret_cast<Element*>(this);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr const TElement* VecMeta<TVec, TElement, Tsize>::ptr() const
{
    static_assert(sizeof(Vec) == size * sizeof(Element));
    return reinterpret_cast<const Element*>(this);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement VecMeta<TVec, TElement, Tsize>::dot(const Vec& other) const
{
    Element r{};
    for (int i = 0; i < size; ++i)
        r += self()[i] * other[i];
    return r;
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement VecMeta<TVec, TElement, Tsize>::length() const
{
    return std::sqrt(dot(self()));
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr void VecMeta<TVec, TElement, Tsize>::clamp(Element min, Element max)
{
    for (int i = 0; i < size; ++i)
        self()[i] = std::clamp(self()[i], min, max);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr void VecMeta<TVec, TElement, Tsize>::normalize()
{
    self() /= length();
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TVec<TElement>& VecMeta<TVec, TElement, Tsize>::self()
{
    return *reinterpret_cast<Vec*>(this);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr const TVec<TElement>& VecMeta<TVec, TElement, Tsize>::self() const
{
    return *reinterpret_cast<const Vec*>(this);
}

}  // namespace utils::vec_detail
