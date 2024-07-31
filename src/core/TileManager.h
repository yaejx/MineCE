#pragma once

#include <graphx.h>

#include "libs/SpriteRaii.h"
#include "libs/unique_ptr.h"
#include "libs/vector.h"

class TileManager
{
public:
    void initializeTilemap();

    void moveMapView(int24_t x_offset, int24_t y_offset);

    gfx_tilemap_t* getTilemap() const;
    uint24_t getXOffset() const;
    uint24_t getYOffset() const;
    
private:
    void loadTiles();
    void loadMap();

    unique_ptr<gfx_tilemap_t> m_tilemap;
    uint24_t m_xOffset;
    uint24_t m_yOffset;
    vector<SpriteRaii> m_tiles;
    unique_ptr<uint8_t[]> m_map; 
};