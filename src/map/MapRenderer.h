#pragma once

#include <functional>
#include <string_view>

class TileMap;

/**
 * @brief Draws every visible layer of the map, back to front.
 * @param map The map to draw.
 * @param entitiesBelowLayer Name of the layer (treetops, roofs...) that covers entities.
 * @param drawEntities Called once, right before that layer, or at the end if it doesn't exist.
 */
void drawMap(const TileMap& map, std::string_view entitiesBelowLayer, const std::function<void()>& drawEntities);
