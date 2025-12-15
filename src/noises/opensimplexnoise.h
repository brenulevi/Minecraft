#pragma once

#include <fastnoiselite.h>

class OpenSimplexNoise
{
public:
    OpenSimplexNoise(int seed, float frequency);
    ~OpenSimplexNoise();

    double noise2D(double x, double y) const;

private:
    FastNoiseLite _noise;
    int _seed;
    float _frequency;
};