#include "renderer/renderManager.h"
RenderManager::RenderManager(){}
RenderManager::RenderManager(EngineContext *ctx):context(ctx){}
void RenderManager::DrawEntities(){
    for(auto &entity:context->entityManager->entities){
        if(entity->isDrawable){
            renderer.Render(*entity->spriteSheet.texture,entity->transform);
        }
    }
}