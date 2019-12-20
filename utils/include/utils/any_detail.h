#pragma once

#include <type_traits>

#include "utils/c_ptr.h"

namespace utils::any_detail {

using Storage = CPtr < void, operator delete>;


enum class ProxyOp {
    destroy,
    copy,
    move,
};


template <typename T>
void proxy(ProxyOp op, Storage* a, Storage* b);


template <typename T>
void proxy(ProxyOp op, Storage* a, Storage* b)
{
    switch (op) {
    case ProxyOp::destroy:
        std::launder(reinterpret_cast<T*>(a->get()))->~T();
        a->reset();
        break;
    case ProxyOp::copy:
        *b = make_c_ptr(operator new(sizeof(T)));
        new (b->get()) T{*std::launder(reinterpret_cast<T*>(a->get()))};
        break;
    case ProxyOp::move:
        *b = make_c_ptr(operator new(sizeof(T)));
        new (b->get()) T{std::move(*std::launder(reinterpret_cast<T*>(a->get())))};
        break;
    }
}

}  // namespace utils::any_detail
