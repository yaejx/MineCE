#include "TerrainGenerator.h"

#include "PerlinNoise.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

vector<vector<BlockType>> TerrainGenerator::generate(
    int width, int height, double frequency, double amplitude
) const
{
    PerlinNoise perlin;
    // Initialize terrain with Air blocks
    vector<vector<BlockType>> terrain(
        height, vector<BlockType>(width, BlockType::Air)
    );

    // Generate terrain
    for (int x = 0; x < width; ++x) {
        // Generate terrain height using Perlin noise
        double noise = perlin.noise(x * frequency) * amplitude;
        int terrain_height = static_cast<int>(noise) + height / 2;

        // Ensure terrain_height is within bounds (2 blocks from top, 12 blocks from bottom)
        terrain_height = MAX(12, MIN(terrain_height, height - 3));

        // Fill terrain
        for (int y = 0; y < height; ++y) {
            // Invert y-coordinate for correct Minecraft-style orientation
            int inverted_y = height - 1 - y;
            
            if (inverted_y >= terrain_height) {
                // Air is already default, so we don't need to do anything
            } else if (inverted_y == terrain_height - 1) {
                terrain[y][x] = BlockType::Grass;
            } else if (inverted_y >= terrain_height - 3) {
                terrain[y][x] = BlockType::Dirt;
            } else if (inverted_y > 0) {
                terrain[y][x] = BlockType::Stone;
            } else {
                terrain[y][x] = BlockType::Bedrock;
            }
        }
    }

    return terrain;
}
