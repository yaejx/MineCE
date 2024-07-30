#pragma once

#include <graphx.h>

#include "gfx/tiles.h"
#include "libs/sprite_raii.h"
#include "libs/unique_ptr.h"
#include "libs/vector.h"

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void initialize();

    gfx_tilemap_t* getTilemap() const;

    uint24_t getTilemapXOffset() const;
    uint24_t getTilemapYOffset() const;

    void moveMapView(int24_t x_offset, int24_t y_offset);

private:
    void initializePalette();
    void initializeTilemap();

    void loadTiles();
    void loadMap();

    unique_ptr<gfx_tilemap_t> m_tilemap;
    uint24_t m_tilemapXOffset;
    uint24_t m_tilemapYOffset;
    vector<sprite_raii> m_tiles;
    unique_ptr<uint8_t[]> m_map;
};