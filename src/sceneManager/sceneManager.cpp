#include "sceneManager/sceneManager.h"
SceneManager::SceneManager(EngineContext* ctx):context(ctx){
    renderManager=RenderManager(context);
};
void SceneManager::Initialize(float renderWidth,float renderHeight){
   
    player=new Player();
    enemy=new Enemy();
    context->entityManager->entities.push_back(player);
    context->entityManager->entities.push_back(enemy);
    context->resourceManager->LoadTexture("Player","src/resources/textures/Mage.png");
    context->resourceManager->LoadTexture("Enemy","src/resources/textures/Monster1.png");
    player->spriteSheet.texture=context->resourceManager->GetTexture("Player");
    player->transform.position={(float)renderWidth/2,(float)renderHeight/2};
    player->transform.size={(float)player->spriteSheet.texture->width,(float)player->spriteSheet.texture->height};
    
    enemy->spriteSheet.texture=context->resourceManager->GetTexture("Enemy");
    enemy->transform.position={(float)renderWidth/2,(float)renderHeight/2};
    enemy->transform.size={(float)enemy->spriteSheet.texture->width,(float)enemy->spriteSheet.texture->height};
    

}
void SceneManager::Update(float dt){
    player->Move(dt);
    player->SubmitToDraw();
    enemy->Follow(player->transform.position,dt);
    enemy->SubmitToDraw();
}
void SceneManager::Draw(){
    renderManager.DrawEntities();
}
void SceneManager::Clear(){
    context->entityManager->ClearEntities();
}