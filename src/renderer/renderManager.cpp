#include "../renderer/renderManager.h"
RenderManager::RenderManager(){}
RenderManager::RenderManager(EngineContext *ctx):context(ctx){}
void RenderManager::DrawEntities(){
    for (int layer = 0; layer < MAX_LAYERS;layer++)
    {
        for (auto& entity : context->entityManager->entities) {
            if (entity->isDrawable && layer == entity->layer) {
                renderer.Render(*entity->spriteSheet.texture, entity->transform,entity->transform.rotation,entity->spriteSheet.flipX,entity->spriteSheet.flipY);
            }
        }
    }
   
}