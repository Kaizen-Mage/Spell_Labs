#pragma once
#include "../shared.h"
class Renderer{

public:
    void Render(Texture tex,Transform2D transform,int flipX,int flipY);
};