#pragma once

#include "raylib.h"
#include <iostream>
#include "Math.hpp"

enum class PrimitiveType {SPHERE, CUBE};

struct MeshComponent {
    Model* modelPtr = nullptr;
    Color color;

    MeshComponent(Color color, Model* modelPtr) : color(color), meshType(meshType) modelPtr(modelPtr) {}
};

struct PrimitiveObjectComponent {
    PrimitiveType objectType;
    Color color;

    PrimitiveObjectComponent(Color color, PrimitiveType objectType) : color(color), objectType(objectType) {}
}