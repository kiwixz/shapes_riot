#pragma once

#include "rectangle.h"
#include "utils/matrix.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Camera {
    [[nodiscard]] Rectangle bounds() const;
    [[nodiscard]] utils::Matrix4f matrix() const;

    void center_on(utils::Vec2d center);

private:
    utils::Vec2d center_;
    utils::Vec2d size_{20.0, 20.0};
};

}  // namespace shapes_riot
