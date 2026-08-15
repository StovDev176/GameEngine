#pragma once
#include <iostream>
#include "Math.hpp"
#include "raylib.h"
#include "Physics.hpp"

class Renderer {
private:
    Camera3D camera;
public:
    Renderer(const Camera3D& camera) : camera(camera) {
        InitWindow(800, 450, "My 3D game engine!");
        SetTargetFPS(60);
    }   
    void beginFrame() {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
    } 
    void endFrame() {
        EndMode3D();
        EndDrawing();
    }

    void drawSphere(const Sphere& sphere, Color color) {
        DrawSphere({sphere.center.x, sphere.center.y, sphere.center.z}, sphere.radius, color);
    } 
    void drawCube(const Vector3& pos, float w, float h, float l, Color color) {
        DrawCube({pos.x, pos.y, pos.z}, w, h, l, color);
    }
};