#pragma once

#include <raylib.h>

class Player
{
public:
    Player(Vector2 position, const Texture2D& spriteSheet);

    void update(Vector2 direction, float dt, Vector2 worldSize);
    void draw() const;

    [[nodiscard]] Vector2 position() const
    {
        return m_position;
    }

private:
    /// @brief Where the player looks. Order matches the columns of the sprite sheet.
    enum class Facing
    {
        Down,
        Up,
        Left,
        Right
    };

    const Texture2D& m_spriteSheet;
    Vector2 m_position;
    Facing m_facing = Facing::Down;
    bool m_walking = false;
    float m_walkTime = 0;
};
