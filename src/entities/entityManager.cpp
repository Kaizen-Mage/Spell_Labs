#include "entities/entityManager.h"
void EntityManager::ClearEntities(){
    for(auto& entity:entities){
        entity->isDrawable=false;
    }
}