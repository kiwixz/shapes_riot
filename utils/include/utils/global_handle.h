#pragma once

namespace utils {

template <auto Tinit, auto Tdestroy>
struct GlobalHandle {
    static constexpr auto init = Tinit;
    static constexpr auto destroy = Tdestroy;

    GlobalHandle();
    ~GlobalHandle();
    GlobalHandle(const GlobalHandle& other);
    GlobalHandle& operator=(const GlobalHandle& other) = default;
    GlobalHandle(GlobalHandle&& other) noexcept;
    GlobalHandle& operator=(GlobalHandle&& other) noexcept = default;

private:
    static int& counter();
};


template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::GlobalHandle()
{
    int& c = counter();
    if (c == 0)
        Tinit();  // msvc doesnt like init();
    ++c;
}

template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::~GlobalHandle()
{
    int& c = counter();
    --c;
    if (c == 0)
        Tdestroy();  // msvc doesnt like destroy();
}

template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::GlobalHandle(const GlobalHandle& /*other*/) :
    GlobalHandle{}
{}

template <auto Tinit, auto Tdestroy>
GlobalHandle<Tinit, Tdestroy>::GlobalHandle(GlobalHandle&& /*other*/) noexcept :
    GlobalHandle{}
{}

template <auto Tinit, auto Tdestroy>
int& GlobalHandle<Tinit, Tdestroy>::counter()
{
    // TODO thread-safety
    static int counter;
    return counter;
}

}  // namespace utils
