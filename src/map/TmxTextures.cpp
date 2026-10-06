#include <map/TmxTextures.h>

#include <tmx.h>

namespace tmx_textures
{

namespace
{

void* load(const char* const path)
{
    return new Texture2D(LoadTexture(path));
}

void unload(void* const resourceImage)
{
    auto* const texture = static_cast<Texture2D*>(resourceImage);
    UnloadTexture(*texture);
    delete texture;
}

} // namespace

void install()
{
    tmx_img_load_func = load;
    tmx_img_free_func = unload;
}

const Texture2D& fromResource(const void* const resourceImage)
{
    return *static_cast<const Texture2D*>(resourceImage);
}

} // namespace tmx_textures
