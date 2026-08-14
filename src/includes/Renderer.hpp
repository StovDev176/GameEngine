#pragma once
#include <iostream>
#include "Math.hpp"
#include "raylib.h"


class Designer_3D {
public:
    void drawCube(const Vector3& vector3, float size, Color color) {
        DrawCube({vector3.x, vector3.y, vector3.z}, size, size, size, color);
    }  
    void drawSphere(const Vector3& pos, float radius, Color color) {
        DrawSphere({pos.x, pos.y, pos.z}, radius, color);
    }
    void drawTriangle(const vec)   
}