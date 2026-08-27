#include "includes/ECS.hpp"
#include "includes/Math.hpp"
#include "includes/DataModel.hpp"
#include "includes/Physics.hpp"
#include "includes/TaskScheduler.hpp"
#include "includes/Renderer.hpp"
#include "includes/Components.hpp"
#include "includes/ui/imgui_wrapper.hpp"
#include "includes/CameraService.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <optional>
#include <chrono>



int main() {
    Registry registry;
    Entity player = registry.create();
    Entity enemy = registry.create();
    Entity mesh1 = registry.create();
    Entity baseplate = registry.create();
    ComponentPool<RigidBody>* rigidBodyPool = registry.getComponentPool<RigidBody>();
    ComponentPool<TransformComponent>* tfcPool = registry.getComponentPool<TransformComponent>();
    ComponentPool<MeshComponent>* meshPool = registry.getComponentPool<MeshComponent>();
    ComponentPool<AABB>* AABB_Pool = registry.getComponentPool<AABB>();

    rigidBodyPool->addData(
      RigidBody{
        vector3(1.0f, 2.0f, 0.0f),
        vector3(0.0f, 0.0f, 0.0f),
        5.0f,
        0.5f,
        vector3(0.0f, 0.0f, 0.0f),}, mesh1);
    rigidBodyPool->addData(
      RigidBody{
        vector3(0.0f, 0.0f, 0.0f), 
        vector3(0.0f, 0.0f, 0.0f),
        0.0f,
        0.5f,
        vector3(0.0f, 0.0f, 0.0f),}, baseplate);
    tfcPool->addData(TransformComponent{
      vector3(0.0f, 100.0f, 0.0f), // position
      vector3(90.0f, 20.0f, 0.0f), // rotation
      vector3(1.0f, 1.0f, 1.0f), // scale
    }, mesh1); 
    tfcPool->addData(TransformComponent{
      vector3(0.0f, -0.5f, 0.0f),
      vector3(0.0f, 0.0f, 0.0f),
      vector3(100.0f, 1.0f, 100.0f),
    }, baseplate);   


    meshPool->addData(MeshComponent(PrimitiveType::SPHERE, GREEN), mesh1);
    meshPool->addData(MeshComponent(PrimitiveType::CUBE, BLUE), baseplate);
    TaskScheduler taskScheduler;
    Camera3D camera = {0};
    camera.position = (Vector3){ 20.0f, 20.0f, 20.0f }; 
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };     
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          
    camera.fovy = 45.0f; 
    camera.projection = CAMERA_PERSPECTIVE;  
    Renderer renderer;
    taskScheduler.insertEvent(Event([rigidBodyPool, tfcPool](float dt){
      for (size_t i = 0; i < rigidBodyPool->dense.size(); i++) {
        uint32_t id = rigidBodyPool->denseIds[i];
        RigidBody* rb = rigidBodyPool->getComponent(id);
        TransformComponent* tfc = tfcPool->getComponent(id);

        if (rb != nullptr && tfc != nullptr) {
            applyGravity(*rb);
            updatePhysics(*rb, *tfc, dt); 
        }
      }  
    }, 1, false));

    taskScheduler.insertEvent(Event([tfcPool, meshPool, &camera](float dt){
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Ray raylibRay = GetMouseRay(mousePos, camera);

        Raycast raycast(
          vector3(raylibRay.position.x, raylibRay.position.y, raylibRay.position.z), 
          vector3(raylibRay.direction.x, raylibRay.direction.y, raylibRay.direction.z) 
        );
        
        for (size_t i = 0; i < meshPool->dense.size(); i++) {
          uint32_t id = meshPool->denseIds[i];
          MeshComponent* meshPtr = &meshPool->dense[i];
          TransformComponent* tfCPtr = tfcPool->getComponent(id);

          if (tfCPtr != nullptr && meshPtr != nullptr) {
            Sphere tempSphere(tfCPtr->position, tfCPtr->scale.x);
            auto hit = tempSphere.intersect(raycast);

            if (hit != std::nullopt) {
              meshPtr->color = RED; 
              break; 
            }
          }
        }
      }
    }, 4, false)); 


    taskScheduler.insertEvent(Event([&registry, tfcPool, rigidBodyPool](float dt){
      for (size_t i = 0; i < tfcPool->dense.size(); i++) {
        
        Entity idA = tfcPool->denseIds[i];
        TransformComponent* tfcA = &tfcPool->dense[i];
        RigidBody* rb1 = rigidBodyPool->getComponent(idA);
        if (rb1 == nullptr) {continue;}
        AABB boxA = createAABB(*tfcA); 
        for (size_t j = i + 1; j < tfcPool->dense.size(); j++) {
            
            Entity idB = tfcPool->denseIds[j];
            TransformComponent* tfcB = &tfcPool->dense[j];
            AABB boxB = createAABB(*tfcB);
            RigidBody* rb2 = rigidBodyPool->getComponent(idB);
            if (rb2 == nullptr) {continue;}
            Manifold m = computeManifold(boxA, boxB);
            if (m.colliding) {
              float res = std::min(rb1->bounciness, rb2->bounciness);
              collide(*rb1, *rb2, m.normal, res);
              posCorrection(m, *rb1, *rb2, *tfcA, *tfcB);
            }
        }
      }
    }, 2, false));
    Entity selectedEntity = mesh1;

    ImGuiIO& io = ImGui::GetIO();
    while (!WindowShouldClose()) {
      taskScheduler.update(GetFrameTime());
      if (!io.WantCaptureMouse) {
        UpdateCameraTarget(0.003, camera);
        UpdateFreeflyCamera(camera, 20, GetFrameTime());
      }

      renderer.beginFrame(camera);
      for (int i=0; i<meshPool->dense.size();i++) {
        uint32_t id = meshPool->denseIds[i];
        MeshComponent* meshPtr = &meshPool->dense[i];
        TransformComponent* tfCPointer = tfcPool->getComponent(id);

        if (tfCPointer != nullptr && meshPtr != nullptr) {
          switch(meshPtr->meshType) {
            case PrimitiveType::SPHERE:
              renderer.drawSphere({tfCPointer->position.x, tfCPointer->position.y, tfCPointer->position.z}, tfCPointer->scale.x, meshPtr->color);
              break;
            case PrimitiveType::CUBE:
              renderer.drawCube({tfCPointer->position.x, tfCPointer->position.y, tfCPointer->position.z}, tfCPointer->scale.x, tfCPointer->scale.y, tfCPointer->scale.z, meshPtr->color);
              break;
          }
        }

      }

      renderer.end3D();
      renderer.drawUI();
      rlImGuiBegin();
      drawECSInspector(registry, selectedEntity, tfcPool, meshPool, rigidBodyPool);
      rlImGuiEnd();
      renderer.endFrame();
    }
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}