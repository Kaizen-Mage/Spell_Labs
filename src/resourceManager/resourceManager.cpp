#include "../resourceManager/resourceManager.h"
void ResourceManager::LoadTexture(const std::string& key, const std::string& filepath) {
        auto it = textures.find(key);
        if (it != textures.end()); // already loaded

        Texture tex = ::LoadTexture(filepath.c_str()); // raylib
        textures[key] = tex;
}

Texture* ResourceManager::GetTexture(const std::string& key) {
    auto it = textures.find(key);
    if (it == textures.end()) {
        
        TraceLog(LOG_WARNING,TextFormat("Failed to find texure with key: %s",key.c_str()));
        return nullptr;
    }
    TraceLog(LOG_INFO, TextFormat("Loaded Texture with key: %s",key.c_str()));
    return &it->second;
}

void ResourceManager::UnloadAll() {
    for (auto& [key, tex] : textures) {
        ::UnloadTexture(tex);
    }
    textures.clear();
}