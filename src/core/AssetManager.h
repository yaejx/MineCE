#pragma once

#include "TileManager.h"

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void initialize();

    TileManager& getTileData();
    const TileManager& getTileData() const;

private:
    void initializePalette();

    TileManager m_tiles;
};