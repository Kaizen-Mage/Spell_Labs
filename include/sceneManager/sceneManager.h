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
    int useOutline=true;
    int colorSpace=false;
    int showEdges =false;
    int colorMap =false;
    int texUnitPosition = 0;
    int texUnitNormal = 1;
    int texUnitAlbedoSpec = 2;
    int texUnitDepth= 3;
    int texUnitShadows=4;
    int texUnitPalette = 5; // pick an unused slot
    Camera3D cam;
    Camera3D lightCam;
    Model cube;
    Model gun;
    Model man;
    GBuffer shadowBuffer;
    GBuffer gBuffer;
    Light lights[MAX_LIGHTS] = { 0 };
    Matrix lightView;
    Matrix  lightProj;
    Matrix lightViewProj;
    SceneManager(EngineContext* ctx);
    void LoadPallete(std::string palletteName,int size,std::string location);
    void Update(float dt);
    void Draw();
    void TakeScreenShot();
};