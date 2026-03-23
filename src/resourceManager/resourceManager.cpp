#include "resourceManager/resourceManager.h"
void ResourceManager::LoadTexture(const std::string& key, const std::string& filepath) {
        auto it = textures.find(key);
        if (it != textures.end()); // already loaded

        Texture tex = ::LoadTexture(filepath.c_str()); // raylib
        textures[key] = tex;
}
void ResourceManager::LoadShader(const std::string& key,const std::string& vertexPath,const std::string& fragmentPath) {
        auto it = shaders.find(key);
        if (it != shaders.end()); // already loaded

        Shader shader = ::LoadShader(vertexPath.c_str(),fragmentPath.c_str()); // raylib
        shaders[key] = shader;
}
Texture* ResourceManager::GetTexture(const std::string& key) {
    auto it = textures.find(key);
    if (it == textures.end()) return nullptr;
    return &it->second;
}
Shader* ResourceManager::GetShader(const std::string& key) {
    auto it = shaders.find(key);
    if (it == shaders.end()) return nullptr;
    return &it->second;
}

void ResourceManager::UnloadAll() {
    for (auto& [key, tex] : textures) {
        ::UnloadTexture(tex);
    }
    for (auto& [key, shader] : shaders) {
        ::UnloadShader(shader);
    }
    shaders.clear();
    textures.clear();
}