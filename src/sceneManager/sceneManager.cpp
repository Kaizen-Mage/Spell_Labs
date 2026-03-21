#include "../sceneManager/sceneManager.h"
SceneManager::SceneManager(EngineContext* ctx):context(ctx){
    renderManager=RenderManager(context);
};
void SceneManager::Initialize(float renderWidth,float renderHeight){
   
    player=new Player();
    floor = new Entity();
    player->layer = 1;
       context->entityManager->SpawnEntity(player);
    context->entityManager->SpawnEntity(floor);
    context->resourceManager->LoadTexture("Grass_floor", "src/resources/textures/Grass_Floor.png");
    context->resourceManager->LoadTexture("Player","src/resources/textures/Mage.png");
    context->resourceManager->LoadTexture("Enemy","src/resources/textures/Eye_Monster.png");
    //Player stuff
    player->spriteSheet.texture=context->resourceManager->GetTexture("Player");
    player->transform.position={(float)renderWidth/2,(float)renderHeight/2};
    player->transform.size={(float)player->spriteSheet.texture->width,(float)player->spriteSheet.texture->height};
    //Enemy stuff  
    for (int i = 0; i < 100; i++) {
        Entity* enemy =new Enemy();
        context->entityManager->SpawnEntity(enemy);
        enemies.push_back(enemy);
        enemy->layer = 1;
        enemy->spriteSheet.texture = context->resourceManager->GetTexture("Enemy");
        enemy->transform.position = { (float)GetRandomValue(0,renderWidth),(float)GetRandomValue(0,renderHeight)};
        enemy->transform.size = { (float)enemy->spriteSheet.texture->width,(float)enemy->spriteSheet.texture->height };

    }
 //floor stuff
    floor->spriteSheet.texture = context->resourceManager->GetTexture("Grass_floor");
    floor->transform.position = {240.0f,135.0f };
    floor->transform.size={(float)floor->spriteSheet.texture->width,(float)floor->spriteSheet.texture->height};

}
void SceneManager::Update(float dt){
    floor->SubmitToDraw();
    player->Move(dt);
    player->SubmitToDraw();
    for (auto& enemy : enemies) {
        if (enemy->transform.position.x<player->transform.position.x) {
            enemy->spriteSheet.flipX = -1;
            TraceLog(LOG_INFO, "Flipping");
        }
        else {
            enemy->spriteSheet.flipX = 1;
        }
        enemy->Follow(player->transform.position, dt);
        enemy->SubmitToDraw();
    }
   

}
void SceneManager::Draw(){
    renderManager.DrawEntities();
}
void SceneManager::Clear(){
    context->entityManager->ClearEntities();
}