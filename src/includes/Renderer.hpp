#pragma once
#include <iostream>
#include "Math.hpp"
#include "raylib.h"
#include "Physics.hpp"

class Renderer {
public:
    Renderer() {
        InitWindow(800, 450, "Project Maria");
        SetTargetFPS(60);
    }   
    void beginFrame(const Camera3D& cam) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(cam);
    } 
    void endFrame() {
        EndMode3D();
        EndDrawing();
    }

    void drawSphere(const Sphere& sphere, Color color) {
        DrawSphere({sphere.center.x, sphere.center.y, sphere.center.z}, sphere.radius, color);
    } 
    void drawCube(const vector3& pos, float w, float h, float l, Color color) {
        DrawCube({pos.x, pos.y, pos.z}, w, h, l, color);
    }
    void drawGrid(int slices, float spacing) {
        DrawGrid(slices, spacing);
    }
};