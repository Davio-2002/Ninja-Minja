#include <map/MapRenderer.h>

#include <map/TileAnimation.h>
#include <map/TileMap.h>
#include <map/TmxTextures.h>

#include <raylib.h>
#include <tmx.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <span>
#include <string_view>

namespace
{

constexpr float kObjectLineThickness = 2.5f;

Color toColor(const std::uint32_t argb)
{
    const tmx_col_bytes c = tmx_col_to_bytes(argb);
    return {.r = c.r, .g = c.g, .b = c.b, .a = c.a};
}

/**
 * @brief Turns a polygon/polyline point into a world position.
 * @param origin The object's position; points are stored relative to it.
 * @param point The point as an {x, y} pair.
 */
Vector2 pointAt(const Vector2 origin, const double* const point)
{
    return {.x = origin.x + static_cast<float>(point[0]), .y = origin.y + static_cast<float>(point[1])};
}

std::span<double* const> pointsOf(const tmx_object& object)
{
    return {object.content.shape->points, static_cast<std::size_t>(object.content.shape->points_len)};
}

void drawPolyline(const Vector2 origin, const std::span<double* const> points, const Color color)
{
    for (std::size_t i = 1; i < points.size(); ++i)
    {
        DrawLineEx(pointAt(origin, points[i - 1]), pointAt(origin, points[i]), kObjectLineThickness, color);
    }
}

void drawPolygon(const Vector2 origin, const std::span<double* const> points, const Color color)
{
    drawPolyline(origin, points, color);
    if (points.size() > 2)
    {
        DrawLineEx(pointAt(origin, points.front()), pointAt(origin, points.back()), kObjectLineThickness, color);
    }
}

void drawObject(const tmx_object& object, const Color color)
{
    const Vector2 position{.x = static_cast<float>(object.x), .y = static_cast<float>(object.y)};
    const auto width = static_cast<float>(object.width);
    const auto height = static_cast<float>(object.height);

    switch (object.obj_type)
    {
        case OT_SQUARE:
            DrawRectangleLinesEx({position.x, position.y, width, height}, kObjectLineThickness, color);
            break;
        case OT_POLYGON:
            drawPolygon(position, pointsOf(object), color);
            break;
        case OT_POLYLINE:
            drawPolyline(position, pointsOf(object), color);
            break;
        case OT_ELLIPSE:
            DrawEllipseLinesV({position.x + width / 2, position.y + height / 2}, width / 2, height / 2, color);
            break;
        default:
            break;
    }
}

void drawObjectLayer(const tmx_object_group& group)
{
    const Color color = toColor(group.color);
    for (const tmx_object* object = group.head; object != nullptr; object = object->next)
    {
        if (object->visible)
        {
            drawObject(*object, color);
        }
    }
}

void drawImageLayer(const tmx_image& image)
{
    DrawTexture(tmx_textures::fromResource(image.resource_image), 0, 0, WHITE);
}

void drawTileLayer(const tmx_map& map, const tmx_layer& layer)
{
    const auto alpha = static_cast<unsigned char>(0xFF * layer.opacity);
    const Color tint{.r = alpha, .g = alpha, .b = alpha, .a = alpha};
    const double time = GetTime();

    for (unsigned int row = 0; row < map.height; ++row)
    {
        for (unsigned int column = 0; column < map.width; ++column)
        {
            // The top 3 bits of a cell are flip flags (not supported yet), the rest is the tile id.
            const std::uint32_t gid = layer.content.gids[row * map.width + column] & TMX_FLIP_BITS_REMOVAL;
            if (map.tiles[gid] == nullptr)
            {
                continue;
            }

            const tmx_tile& tile = currentFrame(*map.tiles[gid], time);
            const tmx_tileset& tileset = *tile.tileset;
            // Tiles have their own image in "collection of images" tilesets,
            // otherwise they are a region of the tileset's sprite sheet.
            const tmx_image& image = tile.image != nullptr ? *tile.image : *tileset.image;

            const auto width = static_cast<float>(tileset.tile_width);
            const auto height = static_cast<float>(tileset.tile_height);
            const Rectangle source{.x = static_cast<float>(tile.ul_x),
                                   .y = static_cast<float>(tile.ul_y),
                                   .width = width,
                                   .height = height};
            const Vector2 destination{.x = column * width, .y = row * height};
            DrawTextureRec(tmx_textures::fromResource(image.resource_image), source, destination, tint);
        }
    }
}

class LayerPass
{
public:
    LayerPass(const tmx_map& map, const std::string_view entitiesBelowLayer, const std::function<void()>& drawEntities)
        : m_map(map)
        , m_entitiesBelowLayer(entitiesBelowLayer)
        , m_drawEntities(drawEntities)
    {
    }

    void run()
    {
        drawLayers(m_map.ly_head);
        drawEntitiesOnce();
    }

private:
    void drawLayers(const tmx_layer* layer)
    {
        for (; layer != nullptr; layer = layer->next)
        {
            if (layer->name != nullptr && layer->name == m_entitiesBelowLayer)
            {
                drawEntitiesOnce();
            }
            if (!layer->visible)
            {
                continue;
            }
            switch (layer->type)
            {
                case L_GROUP:
                    drawLayers(layer->content.group_head);
                    break;
                case L_OBJGR:
                    drawObjectLayer(*layer->content.objgr);
                    break;
                case L_IMAGE:
                    drawImageLayer(*layer->content.image);
                    break;
                case L_LAYER:
                    drawTileLayer(m_map, *layer);
                    break;
                default:
                    break;
            }
        }
    }

    void drawEntitiesOnce()
    {
        if (!m_entitiesDrawn)
        {
            m_drawEntities();
            m_entitiesDrawn = true;
        }
    }

    const tmx_map& m_map;
    const std::string_view m_entitiesBelowLayer;
    const std::function<void()>& m_drawEntities;
    bool m_entitiesDrawn = false;
};

} // namespace

void drawMap(const TileMap& map, const std::string_view entitiesBelowLayer, const std::function<void()>& drawEntities)
{
    ClearBackground(toColor(map.data().backgroundcolor));
    LayerPass(map.data(), entitiesBelowLayer, drawEntities).run();
}
