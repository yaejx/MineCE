#include "PerlinNoise.h"

#include <cstdlib> // rand()

PerlinNoise::PerlinNoise()
{
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
    {
        p[i] = i;
    }

    // Fisher-Yates shuffle
    for (size_t i = HASH_TABLE_SIZE - 1; i > 0; i--)
    {
        size_t j = rand() % (i + 1);
        
        // std::swap() is breaking intellisense for some reason
        size_t temp = p[i];
        p[i] = p[j];
        p[j] = temp;
    }
}

double PerlinNoise::noise(double x) const
{
    // Hash the integer part of x to get an index into the permutation table
    int X = static_cast<int>(x) & (HASH_TABLE_SIZE - 1);
    // Calculate the fractional part of x
    x -= static_cast<int>(x);
    // Apply the fade function to smoothen the output
    double u = fade(x);
    // Return interploation between the gradients at X and X + 1
    return lerp(u, grad(p[X], x), grad(p[X + 1], x - 1)) * 2.0 - 1.0;
}

double PerlinNoise::fade(double t) const
{
    // Smoothly interpolates between 0 and 1, with zero 1st and 2nd order
    // derivatives at t=0 and t=1
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) const
{
    // Linearly interpolates between a and b
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x) const
{
    // Extract the low 4 bits of the hash
    int h = hash & 15;
    // Create a gradient value based on the lower 3 bits
    // This will be either 1.0, 2.0, ..., or 8.0
    double grad = 1.0 + (h & 7);
    // If the 4th bit is set, negate the gradient
    if (h & 8) grad = -grad;
    // Return the dot product of the gradient and the input coordinate
    return grad * x;
}
