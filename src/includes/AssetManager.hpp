#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <format>
#include "raylib.h"

class AssetManager {
private:    
    std::unordered_map<std::string, Texture2D> textures;

public:
    ~AssetManager() {
        UnloadAll();
    }

    bool LoadTexture(const std::string& filePath, const std::string& id) {
        if (textures.find(id) != textures.end()) {
            std::cout << std::format("[AssetManager] Error: ID '{}' already exists in the assets map.\n", id);
            return false;
        }

        Texture2D tex = ::LoadTexture(filePath.c_str());

        if (tex.id == 0) {
            std::cout << std::format("[AssetManager] Error: Failed to load texture from path '{}'\n", filePath);
            return false;
        }

        textures[id] = tex;
        return true;
    }

    Texture2D& GetTexture(const std::string& id) {
        auto it = textures.find(id);
        if (it == textures.end()) {
            std::cout << std::format("[AssetManager] Warning: Texture ID '{}' not found!\n", id);
            static Texture2D emptyTexture = { 0 };
            return emptyTexture;
        }
        return it->second;
    }

    void UnloadAll() {
        for (auto& [id, texture] : textures) {
            ::UnloadTexture(texture);
        }
        
        textures.clear();
        std::cout << "[AssetManager] All textures successfully unloaded from VRAM.\n";
    }
};