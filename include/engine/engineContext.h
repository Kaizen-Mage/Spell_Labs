#pragma once
#include "entities/entityManager.h"
#include "resourceManager/resourceManager.h"
struct EngineContext{
    EntityManager* entityManager;
    ResourceManager* resourceManager;
};