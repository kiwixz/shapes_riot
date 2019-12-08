#pragma once

#include <cmath>
#include "utils/matrix.h"
#include "utils/vec.h"

namespace gfx {

template <typename T>
constexpr utils::Matrix4<T> transformation(const utils::Vec3<T>& translation_vec,
                                           const utils::Vec3<T>& scale_factors,
                                           const utils::Vec3<T>& rotation_angles);

template <typename T>
constexpr utils::Matrix4<T> translation(const utils::Vec3<T>& vec);

template <typename T>
constexpr utils::Matrix4<T> scale(const utils::Vec3<T>& factors);

template <typename T>
constexpr utils::Matrix4<T> rotation(const utils::Vec3<T>& angles);


template <typename T>
constexpr utils::Matrix4<T> transformation(const utils::Vec3<T>& translation_vec,
                                           const utils::Vec3<T>& scale_factors,
                                           const utils::Vec3<T>& rotation_angles)
{
    return translation(translation_vec) * rotation(rotation_angles) * scale(scale_factors);
}

template <typename T>
constexpr utils::Matrix4<T> translation(const utils::Vec3<T>& vec)
{
    return {{1, 0, 0, vec.x,
             0, 1, 0, vec.y,
             0, 0, 1, vec.z,
             0, 0, 0, 1}};
}

template <typename T>
constexpr utils::Matrix4<T> scale(const utils::Vec3<T>& factors)
{
    return {{factors.x, 0, 0, 0,
             0, factors.y, 0, 0,
             0, 0, factors.z, 0,
             0, 0, 0, 1}};
}

template <typename T>
constexpr utils::Matrix4<T> rotation(const utils::Vec3<T>& angles)
{
    T yaw_cos = std::cos(angles.x);
    T yaw_sin = std::sin(angles.x);
    T pitch_cos = std::cos(angles.y);
    T pitch_sin = std::sin(angles.y);
    T roll_cos = std::cos(angles.z);
    T roll_sin = std::sin(angles.z);

    return {{yaw_cos * roll_cos + yaw_sin * pitch_sin * roll_sin,
             roll_sin * pitch_cos,
             -yaw_sin * roll_cos + yaw_cos * pitch_sin * roll_sin,
             0,
             -yaw_cos * roll_sin + yaw_sin * pitch_sin * roll_cos,
             roll_cos * pitch_cos,
             roll_sin * yaw_sin + yaw_cos * pitch_sin * roll_cos,
             0,
             yaw_sin * pitch_cos,
             -pitch_sin,
             yaw_cos * pitch_cos,
             0,
             0, 0, 0, 1}};
}

}  // namespace gfx
