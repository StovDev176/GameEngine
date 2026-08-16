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
    void end3D() {
        EndMode3D();
    }
    void endFrame() {
        EndDrawing();
    }

    void drawSphere(const vector3& pos, float radius, Color color) {
        DrawSphere({pos.x,pos.y, pos.z}, radius, color);
    } 
    void drawCube(const vector3& pos, float w, float h, float l, Color color) {
        DrawCube({pos.x, pos.y, pos.z}, w, h, l, color);
    }
    void drawGrid(int slices, float spacing) {
        DrawGrid(slices, spacing);
    }
    void drawUI() {
        DrawFPS(10, 10);
    
        DrawText("Project Maria Engine", 10, 40, 20, RAYWHITE);
    }
};