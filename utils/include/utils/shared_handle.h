#pragma once

#include <memory>

namespace utils {

template <void (*Tinit)(), void (*Tdestroy)()>
struct SharedHandle {
    static constexpr void (*init)() = Tinit;
    static constexpr void (*destroy)() = Tdestroy;

    SharedHandle();
    ~SharedHandle();
    SharedHandle(const SharedHandle& other);
    SharedHandle& operator=(const SharedHandle& other);
    SharedHandle(SharedHandle&& other);
    SharedHandle& operator=(SharedHandle&& other);

private:
    static int& counter();
};


template <void (*Tinit)(), void (*Tdestroy)()>
SharedHandle<Tinit, Tdestroy>::SharedHandle()
{
    int& c = counter();
    ++c;
    if (c == 1)  // was zero
        init();
}

template <void (*Tinit)(), void (*Tdestroy)()>
SharedHandle<Tinit, Tdestroy>::~SharedHandle()
{
    int& c = counter();
    --c;
    if (c == 0)
        destroy();
}

template <void (*Tinit)(), void (*Tdestroy)()>
SharedHandle<Tinit, Tdestroy>::SharedHandle(const SharedHandle& other) :
    SharedHandle{}
{}

template <void (*Tinit)(), void (*Tdestroy)()>
SharedHandle<Tinit, Tdestroy>& SharedHandle<Tinit, Tdestroy>::operator=(const SharedHandle& other)
{
    return *this;
}

template <void (*Tinit)(), void (*Tdestroy)()>
SharedHandle<Tinit, Tdestroy>::SharedHandle(SharedHandle&& other) :
    SharedHandle{}
{}

template <void (*Tinit)(), void (*Tdestroy)()>
SharedHandle<Tinit, Tdestroy>& SharedHandle<Tinit, Tdestroy>::operator=(SharedHandle&& other)
{
    return *this;
}

template <void (*Tinit)(), void (*Tdestroy)()>
int& SharedHandle<Tinit, Tdestroy>::counter()
{
    static int counter;
    return counter;
}

}  // namespace utils
