#pragma once
#include "raylib.h"
#include "vector"
#include "string"
#include "entity.h"
#include "player/player.h"
#include "enemy/enemy.h"
class EntityManager{
public:
    std::vector<Entity*>entities;
    void SpawnEntity(Entity* entity);
    void ClearEntities();
};
