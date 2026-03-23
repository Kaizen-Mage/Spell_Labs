#pragma once
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