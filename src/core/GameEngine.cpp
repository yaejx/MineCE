#include "GameEngine.h"

GameEngine::GameEngine()
    : m_running(true)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::run()
{
    init();

    while (m_running)
    {
        input();
        update();
        render();
    }
}

void GameEngine::init()
{
    m_assets.initialize();
}

void GameEngine::input()
{
    kb_Scan();

    for (auto &key : m_keys)
    {
        key.second.update(kb_IsDown(key.first));
    }
}

void GameEngine::update()
{
    if (m_keys[kb_KeyClear].isPressed())
    {
        m_running = false;
    }

    uint24_t x_offset = m_assets.getTilemapXOffset();
    uint24_t y_offset = m_assets.getTilemapYOffset();
    uint8_t tile_width = m_assets.getTilemap()->tile_width;
    uint8_t tile_height = m_assets.getTilemap()->tile_height;
    uint8_t map_width = m_assets.getTilemap()->width;
    uint8_t map_height = m_assets.getTilemap()->height;
    uint8_t draw_width = m_assets.getTilemap()->draw_width;
    uint8_t draw_height = m_assets.getTilemap()->draw_height;

    if (m_keys[kb_KeyLeft].isPressed() && x_offset)
    {
        m_assets.moveMapView(-8, 0);
    }
    if (m_keys[kb_KeyRight].isPressed() &&
        x_offset / tile_width <= map_width - draw_width)
    {
        m_assets.moveMapView(8, 0);
    }
    if (m_keys[kb_KeyUp].isPressed() && y_offset)
    {
        m_assets.moveMapView(0, -8);
    }
    if (m_keys[kb_KeyDown].isPressed() &&
        y_offset / tile_height <= map_height - draw_height)
    {
        m_assets.moveMapView(0, 8);
    }
}

void GameEngine::render() const
{
    gfx_Tilemap(
        m_assets.getTilemap(),
        m_assets.getTilemapXOffset(),
        m_assets.getTilemapYOffset()
    );

    gfx_SwapDraw();
}
