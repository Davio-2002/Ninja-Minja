#include <Game.h>

#include <input/Input.h>
#include <map/MapRenderer.h>

#include <raymath.h>

#include <string_view>

namespace
{

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr int kTargetFps = 30;

// Conventions for maps made in Tiled.
constexpr std::string_view kPlayerStartType = "start";
constexpr std::string_view kLayerAbovePlayer = "Over";

Vector2 spawnPoint(const TileMap& map)
{
    return map.findObjectPosition(kPlayerStartType).value_or(map.sizeInPixels() / 2);
}

} // namespace

Game::Game(const std::string& mapPath)
    : m_window(kWindowWidth, kWindowHeight, "Davdew Valley")
    , m_map(mapPath)
    , m_playerSprite(ASSETS_DIR "/sprites/player.png")
    , m_player(spawnPoint(m_map), m_playerSprite.get())
    , m_camera(m_player.position())
{
    SetTargetFPS(kTargetFps);
}

void Game::run()
{
    while (!WindowShouldClose())
    {
        update();
        draw();
    }
}

void Game::update()
{
    const Vector2 worldSize = m_map.sizeInPixels();
    m_player.update(input::movementDirection(), GetFrameTime(), worldSize);
    m_camera.update(Window::size(), m_player.position(), worldSize, input::zoomSteps(), input::dragDelta());
}

void Game::draw() const
{
    BeginDrawing();
    BeginMode2D(m_camera.view());
    drawMap(m_map, kLayerAbovePlayer, [this] { m_player.draw(); });
    EndMode2D();
    EndDrawing();
}
