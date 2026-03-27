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
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "3D pixel art");
    SceneManager sceneManager=SceneManager(&context);
    SetTraceLogLevel(LOG_WARNING);
    //Shader stuff
    //ToggleFullscreen();
    while (!WindowShouldClose()) {
        sceneManager.Update(0.0);
        BeginDrawing();
        sceneManager.Draw();
        EndDrawing();
       
    }
    context.resourceManager->UnloadAll();
    rlImGuiShutdown();
    CloseWindow();
}