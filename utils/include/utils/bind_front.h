#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace utils {

template <typename F, typename... Args>
[[nodiscard]] auto bind_front(F&& f, Args&&... args);


// do not forget to wrap references in std::ref when appropriate
template <typename F, typename... Args>
auto bind_front(F&& f, Args&&... args)
{
#ifdef __cpp_lib_bind_front
    return std::bind_front(std::forward<F>(f), std::forward<Args>(args)...);
#else
    return [callable = std::forward<F>(f),
            bound_args_tuple = std::make_tuple(std::forward<Args>(args)...)](auto&&... call_args) -> decltype(auto) {  // decltype to support references
        return std::apply([&](auto&&... bound_args)
                          //noexcept(std::is_nothrow_invocable_v<F, Args..., decltype(call_args)...>)  // propagate noexcept (not working?)
                          -> decltype(auto) {  // decltype to support references
                              return (std::invoke(callable,
                                                  std::forward<Args>(bound_args)...,
                                                  std::forward<decltype(call_args)>(call_args)...));
                          },
                          decltype(bound_args_tuple){bound_args_tuple});  // force a copy so we can call it multiple times
    };
#endif
}

}  // namespace utils
