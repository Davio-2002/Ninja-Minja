#include <camera/FollowCamera.h>

#include <raymath.h>

#include <algorithm>

namespace
{

constexpr float kZoomStep = 1.0f;
constexpr float kMinZoom = 2.0f;
constexpr float kMaxZoom = 4.0f;

/**
 * @brief Keeps the view inside the world on one axis.
 * @param center Where the view wants to look.
 * @param halfView Half of the view's size on this axis.
 * @param worldLength Size of the world on this axis.
 * @return The clamped center, or the world's middle if the world is smaller than the view.
 */
float clampToWorld(const float center, const float halfView, const float worldLength)
{
    if (worldLength <= 2 * halfView)
    {
        return worldLength / 2;
    }
    return std::clamp(center, halfView, worldLength - halfView);
}

} // namespace

/**
 * @brief Starts the camera looking at a point.
 * @param target The point to look at.
 *
 * Camera2D draws the world point `target` at the screen point `offset`.
 * With `offset` at the screen center, `target` is the point we look at.
 */
FollowCamera::FollowCamera(const Vector2 target)
    : m_view{.offset = {0, 0}, .target = target, .rotation = 0.0f, .zoom = 1.0f}
{
}

void FollowCamera::update(const Vector2 screenSize, const Vector2 target, const Vector2 worldSize,
                          const float zoomSteps, const std::optional<Vector2> dragDelta)
{
    // The window is resizable, so the screen center is recomputed every frame.
    m_view.offset = screenSize / 2;
    m_view.zoom = std::clamp(m_view.zoom + zoomSteps * kZoomStep, kMinZoom, kMaxZoom);

    // Dragging moves the world with the mouse, i.e. the view the opposite way.
    m_dragOffset = dragDelta ? m_dragOffset - *dragDelta / m_view.zoom : Vector2{0, 0};

    const auto [halfX, halfY] = m_view.offset / m_view.zoom;
    const auto [desiredX, desiredY] = target + m_dragOffset;
    const auto [worldW, worldH] = worldSize;

    m_view.target = {.x = clampToWorld(desiredX, halfX, worldW), .y = clampToWorld(desiredY, halfY, worldH)};

    // Drop the part of the drag the world edge absorbed, so dragging back responds immediately.
    m_dragOffset = m_view.target - target;
}
