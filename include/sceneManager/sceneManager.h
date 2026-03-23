#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "rlights.h"
#include "raymath.h"
#include "iostream"
#include "shared/shared.h"
#include "engine/engineContext.h"
class SceneManager{
protected:
    EngineContext* context;
public:
    int mode=0;
    int texUnitPosition = 0;
    int texUnitNormal = 1;
    int texUnitAlbedoSpec = 2;
    int texUnitDepth= 3;
    Camera3D cam;
    Model cube;
    Model gun;
    GBuffer gBuffer;
    Light lights[MAX_LIGHTS] = { 0 };
    SceneManager(EngineContext* ctx);
    void Update(float dt);
    void Draw();
};