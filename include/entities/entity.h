#pragma once
#include "raylib.h"
#include "shared.h"
class Entity{  
public:
    Entity();
    Transform2D transform;
    SpriteSheet spriteSheet;
    bool isDrawable=false;
    void SubmitToDraw();
    virtual void Move(float dt){};
    virtual void Follow(Vector2 Target,float dt){};
    
    ~Entity();
};