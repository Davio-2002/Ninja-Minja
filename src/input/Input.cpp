#include <input/Input.h>

namespace input
{

Vector2 movementDirection()
{
    Vector2 direction{0, 0};
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        direction.x += 1;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        direction.x -= 1;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        direction.y += 1;
    }
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        direction.y -= 1;
    }
    return direction;
}

float zoomSteps()
{
    return GetMouseWheelMove();
}

std::optional<Vector2> dragDelta()
{
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        return std::nullopt;
    }
    return GetMouseDelta();
}

} // namespace input
