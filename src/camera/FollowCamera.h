#pragma once

#include <raylib.h>

#include <optional>

/**
 * @brief Keeps a target centered on screen without showing anything outside the world.
 *
 * Dragging looks away from the target; releasing snaps back.
 */
class FollowCamera
{
public:
    explicit FollowCamera(Vector2 target);

    void update(Vector2 screenSize, Vector2 target, Vector2 worldSize, float zoomSteps,
                std::optional<Vector2> dragDelta);

    [[nodiscard]] const Camera2D& view() const
    {
        return m_view;
    }

private:
    Camera2D m_view{};
    Vector2 m_dragOffset{0, 0};
};
