#pragma once

#include "utils/flag.h"

namespace gfx {

enum class Anchor : unsigned {
    center,

    top = 1 << 0,
    right = 1 << 1,
    bottom = 1 << 2,
    left = 1 << 3,

    top_left = top | left,
    top_right = top | right,
    bottom_left = bottom | left,
    bottom_right = bottom | right,
};

}  // namespace gfx


template <>
struct utils::IsFlag<gfx::Anchor> : std::true_type {};
