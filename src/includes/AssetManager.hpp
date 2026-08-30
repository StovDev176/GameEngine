#pragma once
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <format>
#include <variant>
#include <type_traits>
#include "raylib.h"

class AssetManager {
private:    
    std::unordered_map<std::string, std::variant<Texture2D, Model>> assets;

public:
    ~AssetManager() {
        UnloadAll();
    }

    template<typename T>
    bool LoadAsset(const std::filesystem::path& filePath, const std::string& id) {
        static_assert(std::is_same_v<T, Model> || std::is_same_v<T, Texture2D>, 
                      "[AssetManager] : Wrong type, please submit Model or Texture2D dawg.");

        if (assets.find(id) != assets.end()) {
            std::cout << std::format("[AssetManager] Error: ID '{}' already exists in the assets map.\n", id);
            return false;
        }

        T asset;

        if constexpr (std::is_same_v<T, Model>) {
            asset = ::LoadModel(filePath.string().c_str());
            if (asset.meshCount <= 0) {
                std::cout << std::format("[AssetManager] Error: Failed to load Model from path '{}'\n", filePath);
                return false;
            }
        } else if constexpr (std::is_same_v<T, Texture2D>) {
            asset = ::LoadTexture(filePath.string().c_str());
            if (asset.id == 0) {
                std::cout << std::format("[AssetManager] Error: Failed to load Texture2D from path '{}'\n", filePath);
                return false;
            }
        }

        assets[id] = asset;
        return true;
    }

    template<typename T>
    T& GetAsset(const std::string& id) {
        static_assert(std::is_same_v<T, Model> || std::is_same_v<T, Texture2D>, 
                      "[AssetManager] : Wrong type requested.");

        auto it = assets.find(id);
        if (it == assets.end()) {
            std::cout << std::format("[AssetManager] Warning: Asset ID '{}' not found!\n", id);
            if constexpr (std::is_same_v<T, Texture2D>) {
                static Texture2D emptyTexture = { 0 };
                return emptyTexture;
            } else if constexpr (std::is_same_v<T, Model>) {
                static Model emptyModel = { 0 };
                return emptyModel;
            }
        }
        return std::get<T>(it->second);
    }    

    void UnloadAll() {
        for (auto& [id, asset] : assets) {
            std::visit([](auto&& arg) {
                using Type = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Type, Model>) {
                    ::UnloadModel(arg);
                } else if constexpr (std::is_same_v<Type, Texture2D>) {
                    ::UnloadTexture(arg);
                }
            }, asset);
        }
        assets.clear();
    }
};