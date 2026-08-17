#pragma once

#include "raylib.h"
#include <iostream>
#include "Math.hpp"

enum class PrimitiveType {SPHERE, CUBE};

struct MeshComponent {
    PrimitiveType meshType;
    Color color;

    MeshComponent(PrimitiveType meshType, Color color) : color(color), meshType(meshType) {}
};
