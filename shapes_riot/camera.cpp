#include "camera.h"
#include "gfx/transformation.h"

namespace shapes_riot {

Rectangle Camera::bounds() const
{
    return {center_ - size_ / 2.0, center_ + size_ / 2.0};
}

utils::Matrix4f Camera::matrix() const
{
    utils::Vec3f scale{utils::Vec2f{2.0 / size_}, 1.0f};
    return gfx::transformation(utils::Vec3f{utils::Vec2f{-center_}, 0.0f} * scale, scale, {});
}

void Camera::center_on(utils::Vec2d center)
{
    center_ = center;
}

}  // namespace shapes_riot
