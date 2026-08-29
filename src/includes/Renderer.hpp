#pragma once
#include <iostream>
#include "Math.hpp"
#include "raylib.h"
#include "Physics.hpp"
#include "ECS.hpp"
#include "imgui_wrapper.hpp"

void ApplyModernTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding    = 6.0f;
    style.ChildRounding     = 4.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding        = 4.0f;

    style.WindowPadding     = ImVec2(10, 10);
    style.FramePadding      = ImVec2(6, 4);
    style.ItemSpacing       = ImVec2(8, 6);
    style.WindowBorderSize  = 1.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]           = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_Header]             = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_HeaderHovered]      = ImVec4(0.28f, 0.30f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderActive]       = ImVec4(0.35f, 0.38f, 0.48f, 1.00f);
    colors[ImGuiCol_Button]             = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered]      = ImVec4(0.28f, 0.30f, 0.38f, 1.00f);
    colors[ImGuiCol_ButtonActive]       = ImVec4(0.35f, 0.38f, 0.48f, 1.00f);
    colors[ImGuiCol_FrameBg]            = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.22f, 0.24f, 0.30f, 1.00f);
    colors[ImGuiCol_FrameBgActive]      = ImVec4(0.28f, 0.30f, 0.38f, 1.00f);
    colors[ImGuiCol_TitleBg]            = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.14f, 0.14f, 0.17f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]   = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_CheckMark]          = ImVec4(0.40f, 0.65f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab]         = ImVec4(0.40f, 0.65f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.50f, 0.75f, 1.00f, 1.00f);
}


class Renderer {
public:
    Renderer() {
        InitWindow(1200, 650, "Project Maria");
        SetTargetFPS(30);
        rlImGuiSetup(true);
        ApplyModernTheme();
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
    void drawUI() {
        DrawFPS(10, 10);
    
        DrawText("Project Maria Engine", 10, 40, 20, RAYWHITE);
    }
};