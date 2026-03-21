#include "raylib.h"
#include "../engine/engineContext.h"
#include "../sceneManager/sceneManager.h"
int renderWidth=1920/4;
int renderHeight=1080/4;
int screenWidth=1920;
int screenHeight=1080;
float moveSpeed=30.0f;

int main(){
    EngineContext context;
    EntityManager entityManager=EntityManager();
    ResourceManager resourceManager=ResourceManager();
    context.entityManager=&entityManager;
    context.resourceManager=&resourceManager;
    SceneManager sceneManager=(&context);
    InitWindow(1920,1080,"SpellLabs");
    RenderTexture2D tex=LoadRenderTexture(renderWidth,renderHeight);
    sceneManager.Initialize((float)renderWidth,(float)renderHeight);
    float dt;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        dt=GetFrameTime();

       sceneManager.Update(dt);
        BeginDrawing();
        ClearBackground(BLACK);
        SetTextureFilter(tex.texture,TEXTURE_FILTER_POINT);
        BeginTextureMode(tex);
            ClearBackground(BLACK);
            sceneManager.Draw();
        EndTextureMode();
        DrawTexturePro(tex.texture,{0,0,(float)tex.texture.width,(float)-tex.texture.height},{0,0,(float)screenWidth,(float)screenHeight},{0,0},0.0,WHITE);
        EndDrawing();
        //Clear all entities
        sceneManager.Clear();
    }
    CloseWindow();
}
