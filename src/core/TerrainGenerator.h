#pragma once

#include "Blocktype.h"

#include "libs/unique_ptr.h"
#include "libs/vector.h"

class TerrainGenerator
{
public:
    vector<vector<BlockType>> generate(
        int width, int height, double frequency, double amplitude
    ) const;
};