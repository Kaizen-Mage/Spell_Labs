#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "rlights.h"
#include <iostream>
#include "engine/engineContext.h"
#include "sceneManager/sceneManager.h"
int main() {
    EngineContext context;
    ResourceManager resourceManager;
    context.resourceManager=&resourceManager;
    InitWindow(1920, 1080, "3D pixel art");
    SceneManager sceneManager=SceneManager(&context);
    //Shader stuff
    while (!WindowShouldClose()) {
        sceneManager.Update(0.0);
        BeginDrawing();
        sceneManager.Draw();
        EndDrawing();
       
    }
    
    CloseWindow();
}