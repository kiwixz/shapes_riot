#pragma once

namespace utils {
namespace detail_vec {

template <template <typename> typename Vec, typename T, int Size>
class VecBase {
public:
    using Type = T;
    static constexpr int size = Size;


    T* to_c()
    {
        static_assert(sizeof(Vec<T>) == sizeof(T) * Size);
        return reinterpret_cast<T*>(this);
    }
    const T* to_c() const
    {
        static_assert(sizeof(Vec<T>) == sizeof(T) * Size);
        return reinterpret_cast<const T*>(this);
    }

    T& operator[](size_t idx)
    {
        return to_c()[idx];
    }
    T operator[](size_t idx) const
    {
        return to_c()[idx];
    }


    Vec<T> operator-() const
    {
        Vec<T> vec;
        for (int i = 0; i < Size; ++i) {
            vec[i] = -self()[i];
        }
        return vec;
    }

#define DEF_OP(op, type, operand)        \
    Vec<T>& operator op(type other)      \
    {                                    \
        for (int i = 0; i < Size; ++i) { \
            self()[i] op operand;        \
        }                                \
        return self();                   \
    }

    DEF_OP(+=, const Vec<T>&, other[i]);
    DEF_OP(-=, const Vec<T>&, other[i]);
    DEF_OP(*=, const Vec<T>&, other[i]);
    DEF_OP(/=, const Vec<T>&, other[i]);
    DEF_OP(+=, T, other);
    DEF_OP(-=, T, other);
    DEF_OP(*=, T, other);
    DEF_OP(/=, T, other);

#undef DEF_OP
#define DEF_OP(op, type)                 \
    Vec<T> operator op(type other) const \
    {                                    \
        Vec<T> vec = self();             \
        vec op## = other;                \
        return vec;                      \
    }

    DEF_OP(+, const Vec<T>&);
    DEF_OP(-, const Vec<T>&);
    DEF_OP(*, const Vec<T>&);
    DEF_OP(/, const Vec<T>&);
    DEF_OP(+, T);
    DEF_OP(-, T);
    DEF_OP(*, T);
    DEF_OP(/, T);

#undef DEF_OP


    T dot(const Vec<T>& other) const
    {
        T r{};
        for (int i = 0; i < Size; ++i) {
            r += self()[i] * other[i];
        }
        return r;
    }

    T length() const
    {
        return std::sqrt(dot(self()));
    }

    Vec<T>& normalize()
    {
        return self() /= length();
    }


private:
    constexpr Vec<T>& self()
    {
        return *reinterpret_cast<Vec<T>*>(this);
    }

    constexpr const Vec<T>& self() const
    {
        return *reinterpret_cast<const Vec<T>*>(this);
    }
};

}  // namespace detail_vec


template <typename T>
class Vec2 : public detail_vec::VecBase<Vec2, T, 2> {
public:
    T x{};
    T y{};

    Vec2() = default;
    Vec2(const Vec2<T>&) = default;
    Vec2<T>& operator=(const Vec2<T>&) = default;
    Vec2(Vec2<T>&&) = default;
    Vec2<T>& operator=(Vec2<T>&&) = default;

    Vec2(T x, T y) :
        x{x}, y{y}
    {}

    template <typename U>
    explicit Vec2(const Vec2<U>& other) :
        x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)}
    {}

    template <typename U>
    Vec2<T>& operator=(const Vec2<U>& other)
    {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        return *this;
    }
};

template <typename T>
class Vec3 : public detail_vec::VecBase<Vec3, T, 3> {
public:
    T x{};
    T y{};
    T z{};

    Vec3() = default;
    Vec3(const Vec3<T>&) = default;
    Vec3<T>& operator=(const Vec3<T>&) = default;
    Vec3(Vec3<T>&&) = default;
    Vec3<T>& operator=(Vec3<T>&&) = default;

    Vec3(T x, T y, T z) :
        x{x}, y{y}, z{z}
    {}

    template <typename U>
    explicit Vec3(const Vec3<U>& other) :
        x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)}, z{static_cast<T>(other.z)}
    {}

    template <typename U>
    Vec3<T>& operator=(const Vec3<U>& other)
    {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        z = static_cast<T>(other.z);
        return *this;
    }
};

template <typename T>
class Vec4 : public detail_vec::VecBase<Vec4, T, 4> {
public:
    T x{};
    T y{};
    T z{};
    T w{};

    Vec4() = default;
    Vec4(const Vec4<T>&) = default;
    Vec4<T>& operator=(const Vec4<T>&) = default;
    Vec4(Vec4<T>&&) = default;
    Vec4<T>& operator=(Vec4<T>&&) = default;

    Vec4(T x, T y, T z, T w) :
        x{x}, y{y}, z{z}, w{w}
    {}

    template <typename U>
    explicit Vec4(const Vec4<U>& other) :
        x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)}, z{static_cast<T>(other.z)}, w{static_cast<T>(other.w)}
    {}

    template <typename U>
    Vec4<T>& operator=(const Vec4<U>& other)
    {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        z = static_cast<T>(other.z);
        w = static_cast<T>(other.w);
        return *this;
    }
};


using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned>;

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned>;

using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;
using Vec4u = Vec4<unsigned>;

}  // namespace utils
