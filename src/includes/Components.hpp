#pragma once

#include "raylib.h"
#include <string>

enum class MeshType {
    PRIMITIVE,
    MODEL
};

enum class PrimitiveType {
    SPHERE,
    CUBE,
    CYLINDER,
    PLANE
};

struct MeshComponent {
    MeshType type = MeshType::PRIMITIVE;
    PrimitiveType primitive = PrimitiveType::CUBE;
    std::string assetId = ""; 
    Color color = WHITE;

    MeshComponent(PrimitiveType primitive, Color color = WHITE)
        : type(MeshType::PRIMITIVE), primitive(primitive), color(color) {
            std::cout << "Primitive object created" << std::endl;
        }

    MeshComponent(std::string assetId, Color color = WHITE)
        : type(MeshType::MODEL), assetId(std::move(assetId)), color(color) {
            std::cout << "Model created" << std::endl;
        }
};