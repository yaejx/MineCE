#pragma once

#include "GameEngine.h"
#include "TileManager.h"

namespace EngineComponents
{

class InputHandler
{
public:
    static void updateInputs(GameEngine::InputManager& keys);  
    static void checkForExit(GameEngine::InputManager& keys, bool& running);
};

class MapController
{
public:
    static void moveMapView(TileManager& tile_data, GameEngine::InputManager& keys);
};

class Renderer
{
public:
    static void renderTilemap(const TileManager& tile_data);
};

}; // namespace EngineComponents
