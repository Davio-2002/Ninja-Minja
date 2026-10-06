#include <map/TileAnimation.h>

#include <numeric>
#include <span>

const tmx_tile& currentFrame(const tmx_tile& tile, const double timeSeconds)
{
    const std::span<const tmx_anim_frame> frames{tile.animation, tile.animation_len};

    const unsigned int loopMs =
        std::accumulate(frames.begin(), frames.end(), 0U,
                        [](const unsigned int sum, const tmx_anim_frame& frame) { return sum + frame.duration; });
    if (loopMs == 0)
    {
        return tile;
    }

    auto timeInLoopMs = static_cast<unsigned int>(timeSeconds * 1000) % loopMs;
    for (const auto& [tile_id, duration] : frames)
    {
        if (timeInLoopMs < duration)
        {
            return tile.tileset->tiles[tile_id];
        }
        timeInLoopMs -= duration;
    }
    return tile;
}
