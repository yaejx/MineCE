#include "EngineComponents.h"

namespace EngineComponents
{

void InputHandler::updateInputs(GameEngine::InputManager& keys)
{
    kb_Scan();

    for (auto &key : keys)
    {
        key.second.update(kb_IsDown(key.first));
    }
}

void InputHandler::checkForExit(GameEngine::InputManager& keys, bool& running)
{
    if (keys[kb_KeyClear].isPressed())
    {
        running = false;
    }
}

void MapController::moveMapView(TileManager& tile_data, GameEngine::InputManager& keys)
{
    uint24_t x_offset = tile_data.getXOffset();
    uint24_t y_offset = tile_data.getYOffset();
    uint8_t tile_width = tile_data.getTilemap()->tile_width;
    uint8_t tile_height = tile_data.getTilemap()->tile_height;
    uint8_t map_width = tile_data.getTilemap()->width;
    uint8_t map_height = tile_data.getTilemap()->height;
    uint8_t draw_width = tile_data.getTilemap()->draw_width;
    uint8_t draw_height = tile_data.getTilemap()->draw_height;

    if (keys[kb_KeyLeft].isPressed() && x_offset)
    {
        tile_data.moveMapView(-8, 0);
    }
    if (keys[kb_KeyRight].isPressed() &&
        x_offset / tile_width <= map_width - draw_width)
    {
        tile_data.moveMapView(8, 0);
    }
    if (keys[kb_KeyUp].isPressed() && y_offset)
    {
        tile_data.moveMapView(0, -8);
    }
    if (keys[kb_KeyDown].isPressed() &&
        y_offset / tile_height <= map_height - draw_height)
    {
        tile_data.moveMapView(0, 8);
    }
}

void Renderer::renderTilemap(const TileManager& tile_data)
{
    gfx_Tilemap(
        tile_data.getTilemap(),
        tile_data.getXOffset(),
        tile_data.getYOffset()
    );
}

}; // namespace EngineComponents