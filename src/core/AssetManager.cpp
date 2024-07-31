#include "AssetManager.h"

#include <graphx.h>

#include "gfx/tiles.h"

AssetManager::AssetManager() = default;
AssetManager::~AssetManager() = default;

void AssetManager::initialize()
{
    initializePalette();
    m_tiles.initializeTilemap();
}

TileManager& AssetManager::getTileData()
{
    return m_tiles;
}

const TileManager& AssetManager::getTileData() const
{
    return m_tiles;
}

void AssetManager::initializePalette()
{
    gfx_SetPalette(palette, sizeof_palette, 0);
}
