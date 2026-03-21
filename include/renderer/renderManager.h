#pragma once
#include "../engine/engineContext.h"
#include "../renderer/renderer.h"
#include <iostream>
class RenderManager{
protected:
    EngineContext *context;
public:
    Renderer renderer;
    RenderManager();
    RenderManager(EngineContext *ctx);
    void DrawEntities();
};