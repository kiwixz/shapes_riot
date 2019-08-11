#pragma once

#include "utils/span.h"
#include <cstddef>

namespace @prefix@::embed
{
namespace details
{

extern "C"
{
    extern const std::byte* @prefix@_embed_@name@_begin;
    extern const std::byte* @prefix@_embed_@name@_end;
    extern const size_t @prefix@_embed_@name@_size;
}

}  // namespace @prefix@::embed::details


inline utils::Span<const std::byte> @name@() {
    static utils::Span<const std::byte> span{details::@prefix@_embed_@name@_begin,
                                             details::@prefix@_embed_@name@_size};
    return span;
}

}  // namespace @prefix@::embed
