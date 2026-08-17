#pragma once

#include "raylib.h"
#include <iostream>
#include "Math.hpp"

enum class PrimitiveType {SPHERE, CUBE};

struct MeshComponent {
    PrimitiveType meshType;
    float size;
    Color color;

    MeshComponent(PrimitiveType meshType, float size, Color color) : color(color), size(size), meshType(meshType) {}
};
