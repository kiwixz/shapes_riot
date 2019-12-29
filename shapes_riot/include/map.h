#pragma once

#include <array>
#include <vector>

#include "gfx/draw_list.h"
#include "utils/rand.h"
#include "utils/vec.h"

#include "box.h"

namespace shapes_riot {

inline constexpr int chunk_size = 16;


struct Map {
    gfx::DrawList draw() const;

    void tick(double delta, const Box& camera_view);

private:
    struct Block {
        utils::Vec3f color;
    };

    struct Chunk {
        Chunk(utils::Vec2i position, utils::RandF& rand);

        utils::Vec2i position;
        std::array<Block, chunk_size * chunk_size> blocks;
    };

    std::vector<Chunk> chunks_;
    utils::RandF rand_;
};

}  // namespace shapes_riot
