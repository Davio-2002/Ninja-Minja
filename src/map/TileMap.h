#pragma once

#include <raylib.h>
#include <tmx.h>

#include <memory>
#include <optional>
#include <string>
#include <string_view>

/**
 * @brief A Tiled map loaded from a .tmx file.
 *
 * Owns the map data and its textures, so it must be created after the Window
 * and destroyed before it.
 */
class TileMap
{
public:
    explicit TileMap(const std::string& path);

    [[nodiscard]] Vector2 sizeInPixels() const;

    /**
     * @brief Finds where an object of the given kind is placed.
     * @param type Tiled "type"/"class" to look for.
     * @return Position of the first matching object, or nothing if there is none.
     */
    [[nodiscard]] std::optional<Vector2> findObjectPosition(std::string_view type) const;

    [[nodiscard]] const tmx_map& data() const
    {
        return *m_map;
    }

private:
    struct Deleter
    {
        void operator()(tmx_map* const map) const
        {
            tmx_map_free(map);
        }
    };

    std::unique_ptr<tmx_map, Deleter> m_map;
};
