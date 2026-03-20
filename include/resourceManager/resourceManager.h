#pragma once
#include "raylib.h"
#include "string"
#include "unordered_map"
class ResourceManager{
private:
    std::unordered_map<std::string,Texture>textures;
public:
    void LoadTexture(const std::string& key, const std::string& filepath);
    Texture* GetTexture(const std::string& key);
    void UnloadAll();
};