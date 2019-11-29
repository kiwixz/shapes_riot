#pragma once

namespace utils {

template <auto Tinit, auto Tdestroy>
struct SharedHandle {
    static constexpr auto init = Tinit;
    static constexpr auto destroy = Tdestroy;

    SharedHandle();
    ~SharedHandle();
    SharedHandle(const SharedHandle& other);
    SharedHandle& operator=(const SharedHandle& other) = default;
    SharedHandle(SharedHandle&& other) noexcept;
    SharedHandle& operator=(SharedHandle&& other) noexcept = default;

private:
    static int& counter();
};


template <auto Tinit, auto Tdestroy>
SharedHandle<Tinit, Tdestroy>::SharedHandle()
{
    int& c = counter();
    if (c == 0)
        Tinit();  // msvc doesnt like init();
    ++c;
}

template <auto Tinit, auto Tdestroy>
SharedHandle<Tinit, Tdestroy>::~SharedHandle()
{
    int& c = counter();
    --c;
    if (c == 0)
        Tdestroy();  // msvc doesnt like destroy();
}

template <auto Tinit, auto Tdestroy>
SharedHandle<Tinit, Tdestroy>::SharedHandle(const SharedHandle& /*other*/) :
    SharedHandle{}
{}

template <auto Tinit, auto Tdestroy>
SharedHandle<Tinit, Tdestroy>::SharedHandle(SharedHandle&& /*other*/) noexcept :
    SharedHandle{}
{}

template <auto Tinit, auto Tdestroy>
int& SharedHandle<Tinit, Tdestroy>::counter()
{
    // TODO thread-safety
    static int counter;
    return counter;
}

}  // namespace utils
