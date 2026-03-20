#include "resourceManager/resourceManager.h"
void ResourceManager::LoadTexture(const std::string& key, const std::string& filepath) {
        auto it = textures.find(key);
        if (it != textures.end()); // already loaded

        Texture tex = ::LoadTexture(filepath.c_str()); // raylib
        textures[key] = tex;
}

Texture* ResourceManager::GetTexture(const std::string& key) {
    auto it = textures.find(key);
    if (it == textures.end()) return nullptr;
    return &it->second;
}

void ResourceManager::UnloadAll() {
    for (auto& [key, tex] : textures) {
        ::UnloadTexture(tex);
    }
    textures.clear();
}