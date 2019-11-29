#pragma once

#include "utils/vec.h"

namespace shapes_riot {

struct Box {
    utils::Vec2d center;
    utils::Vec2d half_size;
};

}  // namespace shapes_riot
