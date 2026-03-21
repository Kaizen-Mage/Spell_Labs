#include "../renderer/renderer.h"
void Renderer::Render(Texture2D tex,Transform2D transform,float rotation,int flipX,int flipY){
    DrawTexturePro(tex,{0,0,(float)tex.width * flipX,(float)tex.height * flipY},
        { transform.position.x,transform.position.y,transform.size.x  ,transform.size.y}, {transform.size.x / 2,transform.size.y / 2}, rotation, WHITE);
}