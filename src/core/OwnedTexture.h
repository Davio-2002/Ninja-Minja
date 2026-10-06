#pragma once

#include <raylib.h>

#include <stdexcept>
#include <string>

/**
 * @brief A texture that unloads itself.
 *
 * Must be created after the Window and destroyed before it.
 */
class OwnedTexture
{
public:
    explicit OwnedTexture(const std::string& path)
        : m_texture(LoadTexture(path.c_str()))
    {
        if (m_texture.id == 0)
        {
            throw std::runtime_error("Cannot load texture: " + path);
        }
    }

    ~OwnedTexture()
    {
        UnloadTexture(m_texture);
    }

    OwnedTexture(const OwnedTexture&) = delete;
    OwnedTexture& operator=(const OwnedTexture&) = delete;

    [[nodiscard]] const Texture2D& get() const
    {
        return m_texture;
    }

private:
    Texture2D m_texture;
};
