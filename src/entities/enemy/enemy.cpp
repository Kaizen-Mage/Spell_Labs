#include "entities/enemy/enemy.h"
Enemy::Enemy(){

}
void Enemy::Follow(Vector2 target,float dt){
    Vector2 distance=Vector2Subtract(target,transform.position);
    float len=Vector2Length(distance);
    if(len>0.001f){
        Vector2 dir=Vector2Scale(distance,(1.0f/len));
        transform.position=Vector2Add(transform.position,Vector2Scale(dir,moveSpeed*dt));

    }
    }