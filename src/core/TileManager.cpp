#include "TileManager.h"

#include <compression.h>

#include "gfx/tiles.h"
#include "TerrainGenerator.h"

#define MAP_WIDTH 96
#define MAP_HEIGHT 64

void TileManager::initializeTilemap()
{
    loadTiles();
    loadMap();
    
    // establish raii
    m_tilemap = make_unique<gfx_tilemap_t>();
    m_tilemap->map = m_map.get();

    // allocate an array of raw pointers to the unique_ptr's
    // (since gfx_tilemap_t expects raw pointers)
    m_tilemap->tiles = new gfx_sprite_t*[m_tiles.size()];
    for (size_t i = 0; i < m_tiles.size(); ++i)
    {        
        m_tilemap->tiles[i] = m_tiles[i].get();
    }

    // set the rest of the tilemap's properties
    m_tilemap->type_width = gfx_tile_32_pixel;
    m_tilemap->type_height = gfx_tile_32_pixel;
    m_tilemap->tile_width = 32;
    m_tilemap->tile_height = 32;
    m_tilemap->width = MAP_WIDTH;
    m_tilemap->height = MAP_HEIGHT;
    m_tilemap->x_loc = 0;
    m_tilemap->y_loc = 0;
    m_xOffset = 0;
    m_yOffset = 0;

    // ensure that the draw box is as large as the screen when the maximum
    // numbers of tiles are exposed, even if it means tiles are only partially
    // visible or otherwise offscreen the majority of the time
    m_tilemap->draw_width = GFX_LCD_WIDTH / m_tilemap->tile_width + 1;
    m_tilemap->draw_height = GFX_LCD_HEIGHT / m_tilemap->tile_height + 1;
    if (GFX_LCD_WIDTH % m_tilemap->tile_width)
    {
        m_tilemap->draw_width++;
    }
    if (GFX_LCD_HEIGHT % m_tilemap->tile_height)
    {
        m_tilemap->draw_height++;
    }
}

void TileManager::moveMapView(int24_t x_offset, int24_t y_offset)
{
    m_xOffset += x_offset;
    m_yOffset += y_offset;
}

gfx_tilemap_t* TileManager::getTilemap() const
{
    return m_tilemap.get();
}

uint24_t TileManager::getXOffset() const
{
    return m_xOffset;
}

uint24_t TileManager::getYOffset() const
{
    return m_yOffset;
}

void TileManager::loadTiles()
{
    m_tiles.reserve(tileset_num_tiles);
    SpriteRaii decomp_spr(16, 16);

    // decompress each tile and scale it to 32x32
    for (size_t i = 0; i < tileset_num_tiles; ++i)
    {
        m_tiles.emplace_back(32, 32);
        zx0_Decompress(decomp_spr.get(), tileset_tiles_compressed[i]);
        gfx_ScaleSprite(decomp_spr.get(), m_tiles[i].get());
    }
}

void TileManager::loadMap()
{
    TerrainGenerator terrain;
    auto terrain_2d = terrain.generate(MAP_WIDTH, MAP_HEIGHT, 0.1, 0.5);

    // Flatten the 2D terrain vector into a 1D array for tilemap compatibility
    m_map = make_unique<uint8_t[]>(MAP_WIDTH * MAP_HEIGHT);
    size_t i = 0;
    for (const auto& row: terrain_2d)
    {
        for (const auto& block: row)
        {
            m_map[i++] = static_cast<uint8_t>(block);
        }
    }
}