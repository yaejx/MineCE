#pragma once

#include <cstddef>

class PerlinNoise
{
    static constexpr size_t HASH_TABLE_SIZE = 256;

public:
    PerlinNoise();

    double noise(double x) const;

private:
    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x) const;

    int p[HASH_TABLE_SIZE * 2];
};