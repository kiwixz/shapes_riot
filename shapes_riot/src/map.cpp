#include "map.h"

#include "utils/math.h"

namespace shapes_riot {

gfx::DrawList Map::draw() const
{
    gfx::DrawList draw_list;
    for (const Chunk& chunk : chunks_) {
        utils::Vec2f chunk_base{chunk.position() * chunk_size};

        auto it = chunk.blocks.begin();
        for (int y = 0; y < chunk_size; ++y) {
            for (int x = 0; x < chunk_size; ++x) {
                const Block& block = *it;

                draw_list.push_quad({{chunk_base.x + x, chunk_base.y + y + 1, 0.0f}, block.color},
                                    {{chunk_base.x + x + 1, chunk_base.y + y + 1, 0.0f}, block.color},
                                    {{chunk_base.x + x + 1, chunk_base.y + y, 0.0f}, block.color},
                                    {{chunk_base.x + x, chunk_base.y + y, 0.0f}, block.color});

                ++it;
            }
        }
    }
    return draw_list;
}

void Map::tick(double /*delta*/, const Box& camera_view)
{
    utils::Vec2i min = utils::Vec2i{camera_view.center - camera_view.half_size} / chunk_size;
    utils::Vec2i max = utils::Vec2i{camera_view.center + camera_view.half_size} / chunk_size;

    // delete chunks too far
    chunks_.erase(std::remove_if(chunks_.begin(), chunks_.end(), [&](const Chunk& chunk) {
                      return chunk.position().x < min.x || chunk.position().x > max.x
                             || chunk.position().y < min.y || chunk.position().y > max.y;
                  }),
                  chunks_.end());

    // generate missing chunks
    for (int x = max.x; x >= min.x; --x) {
        for (int y = max.y; y >= min.y; --y) {
            utils::Vec2i pos{x, y};
            if (!std::any_of(chunks_.begin(), chunks_.end(), [&](const Chunk& chunk) {
                    return chunk.position() == pos;
                })) {
                chunks_.emplace_back(pos, rand_);
            }
        }
    }
}


Map::Chunk::Chunk(utils::Vec2i position, utils::RandF& rand) :
    position_{position}
{
    for (Block& block : blocks)
        block.color = {static_cast<float>(rand()),
                       static_cast<float>(rand()),
                       static_cast<float>(rand())};
}

utils::Vec2i Map::Chunk::position() const
{
    return position_;
}

}  // namespace shapes_riot
