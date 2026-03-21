#pragma once
#include "raylib.h"
#include "raymath.h"
#include "../entities/entity.h"
class Enemy:public Entity{
public:
    Enemy();
    float moveSpeed=10.0f;
    void Follow(Vector2 target,float dt);
};