#pragma once
#include "raylib.h"
#include "vector"
#include "entity.h"
#include "player/player.h"
#include "enemy/enemy.h"
class EntityManager{
public:
    std::vector<Entity*>entities;
    void ClearEntities();
};
