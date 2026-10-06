# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

Top-down 2D game in C++20: loads Tiled `.tmx` maps with libTMX and renders them with raylib. Started from the libTMX tutorial "renderer from scratch" (https://libtmx.readthedocs.io/en/latest/renderer-from-scratch.html). Talk to the user in Russian; code, identifiers and comments stay in English.

## Commands

```bash
cmake -S . -B build                 # configure (FetchContent downloads raylib 6.0 and libTMX 1.10.1)
cmake --build build                 # build
./build/Davdew_Valley               # run the default map (assets/maps/island/island.tmx)
./build/Davdew_Valley <path.tmx>    # run any other map
find src -name '*.cpp' -o -name '*.h' | xargs clang-format -i   # format (always, after editing C++)
```

- `build/` is configured with the system CMake (`/usr/bin/cmake`); use it for command-line builds.
- `cmake-build-debug/` belongs to CLion, which uses its own bundled CMake. Never configure or build it from the command line: mixing CMake versions there forces a re-download of dependencies and collides with CLion's own reloads.
- There are no tests and no linter besides clang-format. To verify a change, build, then launch the binary for a couple of seconds and grep its raylib log for `WARNING`/`ERROR` (every texture load logs a `TEXTURE: [ID n]` line).
- New `.cpp` files must be added to the explicit source list in `CMakeLists.txt`.

## Architecture

`Game` (`src/Game.cpp`) is the only module that knows about the others; each folder under `src/` can be read on its own.

- **Lifetime is member order.** `Game` members are declared Window → TileMap → OwnedTexture → Player → FollowCamera. Textures (the map's and the sprite's) need an open window to load and must be freed before `CloseWindow`; reverse destruction order guarantees this. Anything owning GPU resources must be declared after `m_window`.
- **Input is isolated.** Only `src/input/` calls raylib key/mouse functions. Entities and the camera receive intent (direction, zoom steps, drag delta) as arguments from `Game::update`.
- **libTMX texture bridge.** libTMX doesn't load images; `tmx_textures::install()` (called in the `TileMap` constructor, before `tmx_load`) sets the global `tmx_img_load_func`/`tmx_img_free_func` hooks to allocate/free `Texture2D`. Renderer code gets a texture back from `resource_image` via `tmx_textures::fromResource`.
- **Rendering and entity draw order.** `drawMap(map, entitiesBelowLayer, drawEntities)` walks layers first-to-last (recursing into groups) and calls the `drawEntities` callback exactly once, right before the layer with that name, or at the end if it doesn't exist. `Game` passes `"Over"` and a lambda that draws the player. New entities are drawn inside that same lambda.
- **Tile layers.** Gids are masked with `TMX_FLIP_BITS_REMOVAL` (flip flags are ignored, not implemented). Animated tiles are resolved per cell by `currentFrame(tile, GetTime())` using one global clock, so identical tiles animate in sync.
- **Camera.** `Camera2D.offset` stays at screen center, so `target` is the look-at point. It follows the player; the left-mouse drag offset resets on release; each axis is clamped to the map (or centered when the map is smaller than the view).

## Tiled conventions the code depends on

- Player spawns at the first object whose type/class is `start` (`kPlayerStartType` in `Game.cpp`), else at the map center.
- The layer named `Over` is drawn above the player (`kLayerAbovePlayer`).
- Player sprite sheet `assets/sprites/player.png`: 16×16 frames, columns = facing (Down, Up, Left, Right, matching the `Player::Facing` enum order), rows 0–3 = walk cycle.

## Assets

- `ASSETS_DIR` is a compile definition with the absolute path to `assets/`, so the working directory doesn't matter.
- raylib's default build cannot load JPG. Convert images to PNG and update the references in `.tmx`/`.tsx` files.
- `assets/maps/island/` holds Tiled's `rpg/island.tmx` example; `assets/maps/libtmx-samples/` holds libTMX's test maps, which are useful for checking object and image layers.

## C++ style (user requirements)

`.clang-format` enforces most of this; follow the rest by hand:
- Allman braces, and every `if`/`for`/`while` body in braces.
- `const` wherever possible, including locals and by-value parameters in definitions (not in declarations).
- All includes use `<>`, including project headers (`<map/TileMap.h>`; `src/` is on the include path).
- `[[nodiscard]]` on getters and queries; `m_` prefix for private members; `k` prefix for constants in anonymous namespaces.
- No comments that restate the code; keep only ones that explain something non-obvious.
