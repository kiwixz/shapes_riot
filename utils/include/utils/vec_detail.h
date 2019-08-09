#pragma once

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

    constexpr Element& operator[](size_t idx)
    {
        return ptr()[idx];
    }

    constexpr Element operator[](size_t idx) const
    {
        return ptr()[idx];
    }

    [[nodiscard]] constexpr Element* ptr()
    {
        static_assert(sizeof(Vec) == size * sizeof(Element));
        return reinterpret_cast<Element*>(this);
    }

    [[nodiscard]] constexpr const Element* ptr() const
    {
        static_assert(sizeof(Vec) == size * sizeof(Element));
        return reinterpret_cast<const Element*>(this);
    }

    [[nodiscard]] constexpr Element dot(const Vec& other) const
    {
        Element r{};
        for (int i = 0; i < size; ++i)
            r += self()[i] * other[i];
        return r;
    }

    [[nodiscard]] constexpr Element length() const
    {
        return std::sqrt(dot(self()));
    }

    constexpr Vec& normalize()
    {
        return self() /= length();
    }

private:
    [[nodiscard]] constexpr Vec& self()
    {
        return *reinterpret_cast<Vec*>(this);
    }

    [[nodiscard]] constexpr const Vec& self() const
    {
        return *reinterpret_cast<const Vec*>(this);
    }
};


template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr TVec<TElement> operator-(const TVec<TElement>& lhs)
{
    TVec<TElement> vec;
    for (int i = 0; i < TVec<TElement>::size; ++i)
        vec[i] = -lhs[i];
    return vec;
}

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

}  // namespace utils::vec_detail
