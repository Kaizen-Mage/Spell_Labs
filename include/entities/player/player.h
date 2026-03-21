#pragma once
#include "raylib.h"
#include "../entities/entity.h"
class Player:public Entity{
public:
    Player();
    float moveSpeed=30.0f;
    void Move(float dt);
};