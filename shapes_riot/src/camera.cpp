#include "camera.h"
#include "gfx/transformation.h"

namespace shapes_riot {

utils::Matrix4f Camera::matrix() const
{
    utils::Vec3f scale{utils::Vec2f{1.0 / half_size_}, 1.0f};
    return gfx::transformation(utils::Vec3f{utils::Vec2f{-center_}, 0.0f} * scale, scale, {});
}

double Camera::ratio() const
{
    return half_size_.x / half_size_.y;
}

Box Camera::view() const
{
    return {center_, half_size_};
}

void Camera::set_ratio(double ratio)
{
    half_size_.x = half_size_.y * ratio;
}

void Camera::center_on(utils::Vec2d center)
{
    center_ = center;
}

}  // namespace shapes_riot
