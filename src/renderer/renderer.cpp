#include "renderer/renderer.h"
void Renderer::Render(Texture2D tex,Transform2D transform){
    DrawTexturePro(tex,{0,0,(float)tex.width,(float)tex.height},
                   {transform.position.x,transform.position.y,transform.size.x,transform.size.y},{0,0},0.0,WHITE);
}