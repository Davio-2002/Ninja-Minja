#pragma once

#include <raylib.h>

#include <stdexcept>

class Window
{
public:
    Window(const int width, const int height, const char* const title)
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(width, height, title);
        if (!IsWindowReady())
        {
            throw std::runtime_error("Cannot create a window");
        }
        MaximizeWindow();
    }

    ~Window()
    {
        CloseWindow();
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    [[nodiscard]] static Vector2 size()
    {
        return {.x = static_cast<float>(GetScreenWidth()), .y = static_cast<float>(GetScreenHeight())};
    }
};
