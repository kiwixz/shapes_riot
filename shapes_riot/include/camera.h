#pragma once

#include "box.h"
#include "utils/matrix.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Camera {
    Box view() const;
    utils::Matrix4f matrix() const;

    void set_ratio(double ratio);
    void center_on(utils::Vec2d center);

private:
    utils::Vec2d center_;
    utils::Vec2d half_size_{5.0, 5.0};
};

}  // namespace shapes_riot
