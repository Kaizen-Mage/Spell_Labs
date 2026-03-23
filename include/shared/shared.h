#pragma once
#include "raylib.h"
constexpr inline int screenWidth=1920;
constexpr inline int screenHeight=1080;
constexpr inline int downWidth=480;
constexpr inline int downHeight=270;
struct GBuffer{
    unsigned int frameBufferId;
    unsigned int positionTextureId;
    unsigned int normalTextureId;
    unsigned int albedoSpecTextureId;
    unsigned int depthRenderBufferId;
};
struct Pallete{
    Texture* palleteTex;
    int numColors;
    void LoadPallete(std::string palletteName,int size,std::string location);
};