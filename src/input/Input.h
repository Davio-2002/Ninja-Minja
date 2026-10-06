#pragma once

#include <raylib.h>

#include <optional>

/// @brief All key and mouse bindings live here.
namespace input
{

/// @brief Movement keys held this frame: -1, 0 or 1 on each axis. Not normalized.
[[nodiscard]] Vector2 movementDirection();

/// @brief Mouse wheel notches this frame (positive = zoom in).
[[nodiscard]] float zoomSteps();

/// @brief How far the mouse moved this frame, or nothing if the drag button isn't held.
[[nodiscard]] std::optional<Vector2> dragDelta();

} // namespace input
