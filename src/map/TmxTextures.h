#pragma once

#include <raylib.h>

/**
 * @brief Loads map images as raylib textures for libTMX.
 *
 * libTMX doesn't load images itself: it calls back into us for each image a
 * map references and stores whatever we return in `resource_image`.
 */
namespace tmx_textures
{

/// @brief Installs the image hooks. Must be called before any tmx_load().
void install();

[[nodiscard]] const Texture2D& fromResource(const void* resourceImage);

} // namespace tmx_textures
