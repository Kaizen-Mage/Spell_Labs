#include "../entities/entityManager.h"
void EntityManager::ClearEntities(){
    for(auto& entity:entities){
        entity->isDrawable=false;
    }
}
void EntityManager::SpawnEntity(Entity* entity) {
    entities.push_back(entity);
}