#include "entities/player/player.h"
Player::Player(){}
void Player::Move(float dt){
    if(IsKeyDown(KEY_A)){
            transform.position.x-=moveSpeed*dt;
    }
    if(IsKeyDown(KEY_D)){
        transform.position.x+=moveSpeed*dt;
    }
    if(IsKeyDown(KEY_W)){
        transform.position.y-=moveSpeed*dt;
    }
    if(IsKeyDown(KEY_S)){
        transform.position.y+=moveSpeed*dt;
    }
}