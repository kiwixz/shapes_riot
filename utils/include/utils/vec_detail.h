#pragma once

#include <cmath>

namespace utils::vec_detail {

template <template <typename> typename TVec, typename TElement, int Tsize>
struct VecMeta {
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

    constexpr Vec operator-() const
    {
        Vec vec;
        for (int i = 0; i < size; ++i)
            vec[i] = -self()[i];
        return vec;
    }

#define DEF_OP(op, type, operand)          \
    constexpr Vec& operator op(type other) \
    {                                      \
        for (int i = 0; i < size; ++i)     \
            self()[i] op operand;          \
        return self();                     \
    }

    DEF_OP(+=, const Vec&, other[i]);
    DEF_OP(-=, const Vec&, other[i]);
    DEF_OP(*=, const Vec&, other[i]);
    DEF_OP(/=, const Vec&, other[i]);
    DEF_OP(+=, Element, other);
    DEF_OP(-=, Element, other);
    DEF_OP(*=, Element, other);
    DEF_OP(/=, Element, other);
#undef DEF_OP

#define DEF_OP(op, type)                        \
    constexpr Vec operator op(type other) const \
    {                                           \
        Vec vec = self();                       \
        vec op## = other;                       \
        return vec;                             \
    }

    DEF_OP(+, const Vec&);
    DEF_OP(-, const Vec&);
    DEF_OP(*, const Vec&);
    DEF_OP(/, const Vec&);
    DEF_OP(+, Element);
    DEF_OP(-, Element);
    DEF_OP(*, Element);
    DEF_OP(/, Element);
#undef DEF_OP

    constexpr Element* ptr()
    {
        static_assert(sizeof(Vec) == size * sizeof(Element));
        return reinterpret_cast<Element*>(this);
    }

    constexpr const Element* ptr() const
    {
        static_assert(sizeof(Vec) == size * sizeof(Element));
        return reinterpret_cast<const Element*>(this);
    }

    constexpr Element dot(const Vec& other) const
    {
        Element r{};
        for (int i = 0; i < size; ++i)
            r += self()[i] * other[i];
        return r;
    }

    constexpr Element length() const
    {
        return std::sqrt(dot(self()));
    }

    constexpr Vec& normalize()
    {
        return self() /= length();
    }

private:
    constexpr Vec& self()
    {
        return *reinterpret_cast<Vec*>(this);
    }

    constexpr const Vec& self() const
    {
        return *reinterpret_cast<const Vec*>(this);
    }
};

}  // namespace utils::vec_detail
