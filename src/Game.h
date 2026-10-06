#pragma once

#include <camera/FollowCamera.h>
#include <core/OwnedTexture.h>
#include <core/Window.h>
#include <map/TileMap.h>
#include <player/Player.h>

#include <string>

class Game
{
public:
    explicit Game(const std::string& mapPath);

    void run();

private:
    void update();
    void draw() const;

    Window m_window;
    TileMap m_map;
    OwnedTexture m_playerSprite;
    Player m_player;
    FollowCamera m_camera;
};
