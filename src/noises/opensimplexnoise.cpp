#include "opensimplexnoise.h"

OpenSimplexNoise::OpenSimplexNoise(int seed, float frequency)
    : _seed(seed), _frequency(frequency)
{
    _noise.SetSeed(seed);
    _noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    _noise.SetFrequency(frequency);
}

OpenSimplexNoise::~OpenSimplexNoise()
{
}

double OpenSimplexNoise::noise2D(double x, double y) const
{
    return _noise.GetNoise(static_cast<float>(x), static_cast<float>(y));
}
