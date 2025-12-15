#pragma once

#include "texture.h"

struct Crosshair
{
    Crosshair(Texture* tex, float s, float rot)
        : texture(tex), size(s), rotation(rot) {}

    Texture* texture;
    float size;
    float rotation;
};