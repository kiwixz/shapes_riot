#include "map.h"

#include "utils/math.h"

namespace shapes_riot {

gfx::DrawList Map::draw() const
{
    gfx::DrawList draw_list;
    for (const Chunk& chunk : chunks_) {
        utils::Vec2f chunk_base{chunk.position * chunk_size};

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
    utils::Vec2d cam_min{camera_view.center - camera_view.half_size};
    utils::Vec2d cam_max{camera_view.center + camera_view.half_size};

    utils::Vec2i min = cam_min.transform<int>([](double a) { return utils::ntrunc<int>(a / chunk_size); });
    utils::Vec2i max = cam_max.transform<int>([](double a) { return utils::ntrunc<int>(a / chunk_size); });

    // delete chunks too far
    auto it = std::remove_if(chunks_.begin(), chunks_.end(), [&](const Chunk& chunk) {
        return chunk.position.x < min.x || chunk.position.x > max.x
               || chunk.position.y < min.y || chunk.position.y > max.y;
    });
    size_t nr_chunks = std::distance(it, chunks_.end());
    if (nr_chunks > 0) {
        logger_(utils::LogLevel::info, "deleting {} chunks", nr_chunks);
        chunks_.erase(it, chunks_.end());
    }

    // generate missing chunks
    for (int x = max.x; x >= min.x; --x) {
        for (int y = max.y; y >= min.y; --y) {
            utils::Vec2i pos{x, y};
            if (!std::any_of(chunks_.begin(), chunks_.end(), [&](const Chunk& chunk) {
                    return chunk.position == pos;
                })) {
                logger_(utils::LogLevel::info, "creating chunk at ({};{})", x, y);
                chunks_.emplace_back(pos, rand_);
            }
        }
    }
}


Map::Chunk::Chunk(utils::Vec2i _position, utils::RandF& rand) :
    position{_position}
{
    constexpr double luminosity = 0.2;

    for (Block& block : blocks)
        block.color = {static_cast<float>(rand() * luminosity),
                       static_cast<float>(rand() * luminosity),
                       static_cast<float>(rand() * luminosity)};
}

}  // namespace shapes_riot
