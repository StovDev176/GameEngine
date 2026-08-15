#include "includes/ECS.hpp"
#include "includes/Math.hpp"
#include "includes/DataModel.hpp"
#include "includes/Physics.hpp"
#include "includes/TaskScheduler.hpp"
#include "includes/Renderer.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <optional>
#include <chrono>

struct Position {
    vector3 pos;
};

struct Velocity {
  vector3 vel;
};

void movementSystem(Registry& registry, float dt) {
  ComponentPool<Velocity>* velocityPool = registry.getComponentPool<Velocity>();
  ComponentPool<Position>* positionPool = registry.getComponentPool<Position>();

  for (size_t i=0; i<velocityPool->dense.size(); i++) {
    Entity entityId = velocityPool->denseIds[i];
    Velocity vel = velocityPool->dense[i];

    Position* posPtr = positionPool->getComponent(entityId);
    if (posPtr != nullptr) {
      posPtr->pos = posPtr->pos + (vel.vel * dt);
      std::cout << "Successfully updated the position, this is new position :  " << posPtr->pos << std::endl;
    }

  }

}

int main() {
    auto coffre = std::make_shared<SceneNode>(vector3(10, 0, 0));
    auto piece = std::make_shared<SceneNode>(vector3(1, 2, 0));
    
    coffre->addChild(piece);
    std::cout << "Global position of the gold piece : " << piece->getWorldPos() << std::endl;
    coffre->move(vector3(100, 0, 0));
    std::cout << "Global position after moving : " << piece->getWorldPos() << std::endl;

    std::cout << "\n=== RAYCAST TEST ===" << std::endl;
    Scene scene;
    scene.addShape(std::make_unique<Sphere>(vector3(0, 0, 5), 1.0f));
    
    Raycast laser(vector3(0, 0, 0), vector3(0, 0, 1));
    
    auto impact = scene.castRay(laser);
    
    if (impact) {
        std::cout << "The raycast collided with something  " << *impact << std::endl;
    } else {
        std::cout << "The raycast hasn't collided with something" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    Registry registry;
    Entity player = registry.create();
    Entity enemy = registry.create();
    ComponentPool<Position>* posPool = registry.getComponentPool<Position>();
    ComponentPool<Velocity>* velocityPool = registry.getComponentPool<Velocity>();

    posPool->addData(Position{{10.0f, 0.0f, 0.0f}}, player);
    velocityPool->addData(Velocity{{1.5f, 0.0f, 0.0f}}, player);
    posPool->addData(Position{{25.0f, 5.0f, 0.0f}}, enemy);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Execution time : " << duration.count() << " ms" << std::endl;
    std::vector<int> nums = {1, 7, 2};
    TaskScheduler taskScheduler;

    std::string eventName = "Animation";    
    taskScheduler.insertEvent(Event([&registry](float dt){movementSystem(registry, dt);}, 1, true));
    taskScheduler.insertEvent(Event([eventName](float dt){std::cout << eventName <<std::endl;}, 2, true));
    
    Camera3D camera = {0};
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; 
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };     
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          
    camera.fovy = 45.0f; 
    camera.projection = CAMERA_PERSPECTIVE;  

    Renderer renderer;
    taskScheduler.insertEvent(Event([&taskScheduler](float dt) {
    if (WindowShouldClose()) {
        taskScheduler.stop();
    }
    }, 0, false));

    taskScheduler.insertEvent(Event([&renderer, posPool, &camera](float dt){
      renderer.beginFrame(camera);
      for (auto pos : posPool->dense) {
        renderer.drawGrid(20, 1.0f);  

        renderer.drawCube(pos.pos, 5.5f, 1.2f, 3.0f, BLUE);
      }
      renderer.endFrame();
    }, 10, false));
    taskScheduler.insertEvent(Event([&camera](float dt){
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }, 5, false));
    
    taskScheduler.start();
    CloseWindow();
    return 0;
}