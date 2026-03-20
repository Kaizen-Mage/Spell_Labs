#pragma once
#include "raylib.h"
#include "renderer/renderManager.h"
#include "engine/engineContext.h"
class SceneManager{
private:
    EngineContext* context;
public:
    Entity* player;
    Entity* enemy;
    RenderManager renderManager;
    SceneManager(EngineContext* ctx);
    void Initialize(float renderWidth,float renderHeight);
    void Update(float dt);
    void Draw();
    void Clear();
};