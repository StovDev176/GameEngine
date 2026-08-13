#include "includes/ECS.hpp"
#include "includes/Math.hpp"
#include "includes/DataModel.hpp"
#include "includes/Physics.hpp"
#include "includes/TaskScheduler.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <optional>
#include <chrono>

struct Position {
    Vector3 pos;
};

struct Velocity {
  Vector3 vel;
};

void movementSystem(Registry& registry) {
  ComponentPool<Velocity>* velocityPool = registry.getComponentPool<Velocity>();
  ComponentPool<Position>* positionPool = registry.getComponentPool<Position>();

  for (size_t i=0; i<velocityPool->dense.size(); i++) {
    Entity entityId = velocityPool->denseIds[i];
    Velocity vel = velocityPool->dense[i];

    Position* posPtr = positionPool->getComponent(entityId);
    if (posPtr != nullptr) {
      posPtr->pos = posPtr->pos + vel.vel;
      std::cout << "Successfully updated the position, this is new position :  " << posPtr->pos << std::endl;
    }

  }

}

int main() {
    auto coffre = std::make_shared<Transform>(Vector3(10, 0, 0));
    auto piece = std::make_shared<Transform>(Vector3(1, 2, 0));
    
    coffre->addChild(piece);
    std::cout << "Global position of the gold piece : " << piece->getWorldPos() << std::endl;
    coffre->move(Vector3(100, 0, 0));
    std::cout << "Global position after moving : " << piece->getWorldPos() << std::endl;

    std::cout << "\n=== RAYCAST TEST ===" << std::endl;
    Scene scene;
    scene.addShape(std::make_unique<Sphere>(Vector3(0, 0, 5), 1.0f));
    
    Ray laser(Vector3(0, 0, 0), Vector3(0, 0, 1));
    
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

    for (int i=0; i<5; i++) {
      movementSystem(registry);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Execution time : " << duration.count() << " ms" << std::endl;
    std::vector<int> nums = {1, 7, 2};
    TaskScheduler taskScheduler;
    taskScheduler.insertEvent(Event([nums](float dt){std::cout<<"It works"<<std::endl;}, 1, false));

    taskScheduler.start();
    return 0;
}