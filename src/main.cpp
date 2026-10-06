#include <Game.h>

#include <exception>
#include <iostream>
#include <string>

int main(const int argc, char** const argv)
{
    const std::string mapPath = argc > 1 ? argv[1] : ASSETS_DIR "/maps/ninja/ninja.tmx";

    try
    {
        Game game(mapPath);
        game.run();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
