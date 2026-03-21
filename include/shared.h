#pragma once
#include "raylib.h"
struct Transform2D{
    Vector2 position;
    Vector2 size;
    float rotation=0.0f;
};
struct SpriteSheet
{
    Texture* texture;
    int flipX=1;
    int flipY=1;
};
constexpr inline int MAX_LAYERS = 2;