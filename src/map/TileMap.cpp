#include <map/TileMap.h>

#include <map/TmxTextures.h>

#include <stdexcept>
#include <string>

namespace
{

const tmx_object* findObject(const tmx_layer* layer, const std::string_view type)
{
    for (; layer != nullptr; layer = layer->next)
    {
        if (layer->type == L_GROUP)
        {
            if (const tmx_object* found = findObject(layer->content.group_head, type))
            {
                return found;
            }
        }
        else if (layer->type == L_OBJGR)
        {
            for (const tmx_object* object = layer->content.objgr->head; object != nullptr; object = object->next)
            {
                if (object->type != nullptr && object->type == type)
                {
                    return object;
                }
            }
        }
    }
    return nullptr;
}

} // anonymous namespace

TileMap::TileMap(const std::string& path)
{
    tmx_textures::install();
    m_map.reset(tmx_load(path.c_str()));
    if (!m_map)
    {
        throw std::runtime_error("Cannot load map: " + std::string(tmx_strerr()));
    }
}

Vector2 TileMap::sizeInPixels() const
{
    return {static_cast<float>(m_map->width * m_map->tile_width),
            static_cast<float>(m_map->height * m_map->tile_height)};
}

std::optional<Vector2> TileMap::findObjectPosition(const std::string_view type) const
{
    const tmx_object* const object = findObject(m_map->ly_head, type);
    if (object == nullptr)
    {
        return std::nullopt;
    }
    return Vector2{static_cast<float>(object->x), static_cast<float>(object->y)};
}
