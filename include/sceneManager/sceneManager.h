#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "rlights.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "iostream"
#include "vector"
#include "algorithm"
#include "shared/shared.h"
#include "engine/engineContext.h"
class SceneManager{
protected:
    EngineContext* context;
    std::vector<std::string>palleteNames;
    std::unordered_map<std::string, Pallete> palletes;
public:
    int mode=0;
    int curPallete=0;
    bool useOutline=true;
    bool colorSpace=false;
    bool showEdges =false;
    bool colorMap =false;
    int texUnitPosition = 0;
    int texUnitNormal = 1;
    int texUnitAlbedoSpec = 2;
    int texUnitDepth= 3;
    int texUnitPalette = 4; // pick an unused slot
    Camera3D cam;
    Model cube;
    Model gun;
    Model man;
    GBuffer gBuffer;
    Light lights[MAX_LIGHTS] = { 0 };
    SceneManager(EngineContext* ctx);
    void LoadPallete(std::string palletteName,int size,std::string location);
    void Update(float dt);
    void Draw();
    void TakeScreenShot();
};