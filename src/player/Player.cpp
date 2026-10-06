#include <player/Player.h>

#include <raymath.h>

#include <algorithm>
#include <cmath>

namespace
{

constexpr float kSpeed = 80.0f; // map pixels per second
constexpr float kFrameSize = 16.0f;
constexpr int kWalkFrameCount = 4; // rows 0-3 of the sprite sheet
constexpr float kWalkFramesPerSecond = 8.0f;

} // namespace

Player::Player(const Vector2 position, const Texture2D& spriteSheet)
    : m_spriteSheet(spriteSheet)
    , m_position(position)
{
}

void Player::update(const Vector2 direction, const float dt, const Vector2 worldSize)
{
    m_walking = direction.x != 0 || direction.y != 0;
    if (!m_walking)
    {
        m_walkTime = 0;
        return;
    }

    // Diagonals show the sideways sprite.
    if (direction.x != 0)
    {
        m_facing = direction.x > 0 ? Facing::Right : Facing::Left;
    }
    else
    {
        m_facing = direction.y > 0 ? Facing::Down : Facing::Up;
    }

    m_position += Vector2Normalize(direction) * (kSpeed * dt);
    m_walkTime += dt;

    const float half = kFrameSize / 2;
    m_position.x = std::clamp(m_position.x, half, worldSize.x - half);
    m_position.y = std::clamp(m_position.y, half, worldSize.y - half);
}

void Player::draw() const
{
    const int column = static_cast<int>(m_facing);
    const int row = m_walking ? static_cast<int>(m_walkTime * kWalkFramesPerSecond) % kWalkFrameCount : 0;
    const Rectangle source{column * kFrameSize, row * kFrameSize, kFrameSize, kFrameSize};

    // Whole pixels, otherwise the sprite shimmers when zoomed in.
    const Vector2 topLeft{std::round(m_position.x - kFrameSize / 2), std::round(m_position.y - kFrameSize / 2)};
    DrawTextureRec(m_spriteSheet, source, topLeft, WHITE);
}
