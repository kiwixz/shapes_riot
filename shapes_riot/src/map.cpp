#include "map.h"

#include "utils/math.h"

namespace shapes_riot {

gfx::DrawList Map::draw() const
{
    return {};
}

void Map::tick(double /*delta*/, const Box& camera_view)
{
    utils::Vec2i min = utils::Vec2i{camera_view.center - camera_view.half_size} / chunk_size;
    utils::Vec2i max = utils::Vec2i{camera_view.center + camera_view.half_size} / chunk_size;
}

}  // namespace shapes_riot
