#pragma once
#include "../shared.h"
class Renderer{

public:
    void Render(Texture tex,Transform2D transform,float rotatiopn,int flipX,int flipY);
};