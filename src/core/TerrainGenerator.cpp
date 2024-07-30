#include "TerrainGenerator.h"

#include "PerlinNoise.h"

vector<vector<BlockType>> TerrainGenerator::generate(
    int width, int height, double frequency, double amplitude
) const
{
    PerlinNoise perlin;
    
    // Initialize terrain with Air blocks
    vector<vector<BlockType>> terrain(
        height, vector<BlockType>(width, BlockType::Air)
    );

    for (int x = 0; x < width; ++x)
    {
        double noise = perlin.noise(x * frequency) * amplitude;

        int terrain_height = static_cast<int>(
            // Normalize the noise to the range [0, height / 2] and center it
            (noise + 1.0) * height / 4.0 + height / 2.0
        );

        // Clamp terrain_height to valid range [0, height - 1]
        terrain_height = (terrain_height < 0) ? 0
            : ((terrain_height > height - 1) ? height - 1: terrain_height);

        int terrain_line = height - terrain_height;

        for (int y = 0; y < height; ++y)
        {
            if (y == height - 1)
            {
                // Guarantee there is bedrock at the bottom
                terrain[y][x] = BlockType::Bedrock;
            }
            else if (y > terrain_line + 2)
            {
                // Everything below 2 layers of dirt is stone
                terrain[y][x] = BlockType::Stone;
            }
            else if (y > terrain_line)
            {
                // 2 layers of dirt below grass
                terrain[y][x] = BlockType::Dirt;
            }
            else if (y == terrain_line)
            {
                // Top layer of terrain is grass
                terrain[y][x] = BlockType::Grass;
            }
            // else: Above terrain_line remains Air (already initialized)
        }
    }
    return terrain;
}