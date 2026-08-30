#pragma once
#include <iostream>
#include "Math.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Physics.hpp"
#include "ECS.hpp"
#include "ui/imgui_wrapper.hpp"
#include "AssetManager.hpp"
#include "Logger.hpp"
#include "ui/ImGuizmo.h"

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
    void drawUI() {
        DrawFPS(10, 10);
    
        DrawText("Project Maria Engine", 10, 40, 20, RAYWHITE);
    }

    void Draw(Registry& registry, Entity& selectedEntity, ComponentPool<TransformComponent>& tfcPool, ComponentPool<MeshComponent>& meshPool, ComponentPool<RigidBody>& rigidBodyPool, AssetManager& assetManager, Camera3D& camera) {
      beginFrame(camera);
      registry.view<MeshComponent, TransformComponent>([&assetManager](Entity id, MeshComponent& mesh, TransformComponent& tfc) {
        if (mesh.type == MeshType::PRIMITIVE) {
          switch (mesh.primitive) {
            case PrimitiveType::CUBE: {
                Vector3 pos = { tfc.position.x, tfc.position.y, tfc.position.z };
                Vector3 scale = { tfc.scale.x, tfc.scale.y, tfc.scale.z };
                DrawCubeV(pos, scale, mesh.color);
                break;
            }    
            case PrimitiveType::SPHERE: {
                Vector3 pos = { tfc.position.x, tfc.position.y, tfc.position.z };
                Vector3 scale = { tfc.scale.x, tfc.scale.y, tfc.scale.z };
                DrawSphere(pos, scale.x, mesh.color);
                break;
            }    
          }
        } else if (mesh.type == MeshType::MODEL) {
          Model& model = assetManager.GetAsset<Model>(mesh.assetId);
          Vector3 pos = { tfc.position.x, tfc.position.y, tfc.position.z };
          Vector3 scale = { tfc.scale.x, tfc.scale.y, tfc.scale.z };
          Vector3 rotation = {tfc.rotation.x, tfc.rotation.y, tfc.rotation.z};

          std::cout << "Model" << std::endl;

          DrawModelEx(model, pos, rotation, tfc.rotation.y, scale, mesh.color);
        }
      });

      EndMode3D();
      drawUI();
      rlImGuiBegin();
      drawECSInspector(registry, selectedEntity, &tfcPool, &meshPool, &rigidBodyPool);

      if (selectedEntity != UINT32_MAX && tfcPool.hasEntity(selectedEntity)) {
        TransformComponent* tfc = tfcPool.getComponent(selectedEntity);
        Logger::info("Gizmo drawing started");
        drawGizmo(camera, *tfc);
        Logger::info("Gizmo drawn");

      }
      rlImGuiEnd();
      EndDrawing();
    }
 void drawGizmo(Camera3D& camera, TransformComponent& tfc) {
    ImGuizmo::BeginFrame();
    ImGuizmo::Enable(true);
    
    ImGui::PushID(0);

    ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
    ImGuizmo::SetRect(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight());

    Matrix viewMatrix = GetCameraMatrix(camera);
    Matrix projMatrix = MatrixPerspective(
        camera.fovy * (PI / 180.0f), 
        (float)GetScreenWidth() / (float)GetScreenHeight(), 
        0.01f, 1000.0f
    );

    Matrix matScale = MatrixScale(tfc.scale.x, tfc.scale.y, tfc.scale.z);
    Matrix matRot   = MatrixRotateXYZ(Vector3{ 
        tfc.rotation.x * (PI / 180.0f), 
        tfc.rotation.y * (PI / 180.0f), 
        tfc.rotation.z * (PI / 180.0f) 
    });
    Matrix matTrans = MatrixTranslate(tfc.position.x, tfc.position.y, tfc.position.z);

    Matrix worldMatrix = MatrixMultiply(MatrixMultiply(matScale, matRot), matTrans);

    // 5. Transposition pour le format OpenGL (Column-Major)
    viewMatrix  = MatrixTranspose(viewMatrix);
    projMatrix  = MatrixTranspose(projMatrix);
    worldMatrix = MatrixTranspose(worldMatrix);

    // 6. Raccourcis claviers
    static ImGuizmo::OPERATION currentOp = ImGuizmo::TRANSLATE;
    if (IsKeyPressed(KEY_W)) currentOp = ImGuizmo::TRANSLATE;
    if (IsKeyPressed(KEY_E)) currentOp = ImGuizmo::ROTATE;
    if (IsKeyPressed(KEY_R)) currentOp = ImGuizmo::SCALE;

    // 7. Afficher et manipuler le Gizmo
    ImGuizmo::Manipulate(
        &viewMatrix.m0, 
        &projMatrix.m0, 
        currentOp, 
        ImGuizmo::LOCAL, 
        &worldMatrix.m0
    );

    // 8. Récupérer les modifications
    if (ImGuizmo::IsUsing()) {
        worldMatrix = MatrixTranspose(worldMatrix);

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(&worldMatrix.m0, matrixTranslation, matrixRotation, matrixScale);

        tfc.position = vector3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
        tfc.rotation = vector3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
        tfc.scale    = vector3(matrixScale[0], matrixScale[1], matrixScale[2]);
    }

    ImGui::PopID();
}
};