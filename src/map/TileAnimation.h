#pragma once

#include <tmx.h>

/**
 * @brief Picks the frame an animated tile should show right now.
 * @param tile The tile to animate. Returned as is if it isn't animated.
 * @param timeSeconds Global clock, shared by all tiles so identical ones stay in sync.
 * @return The tile to draw.
 */
[[nodiscard]] const tmx_tile& currentFrame(const tmx_tile& tile, double timeSeconds);
