#pragma once
#include "raylib.h"
#include "string"
#include "unordered_map"
class ResourceLoader{
private:
    std::unordered_map<std::string,Texture>textures;
    std::unordered_map<std::string,Shader>shaders;
public:
    void LoadShader(const std::string& key,const std::string& vertexPath, const std::string& fragmentPath);
    void LoadTexture(const std::string& key, const std::string& filepath);
    Texture* GetTexture(const std::string& key);
    Shader* GetShader(const std::string& key);
    void UnloadAll();
};