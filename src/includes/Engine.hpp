#pragma once
#include "ECS.hpp"
#include <iostream>
#include "Math.hpp"
#include <chrono>
#include <thread>

class Engine {
private:
    bool isRunning = false;
    Registry registry; 

public:
    void start() {
        isRunning = true;
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (isRunning) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> dt = currentTime - lastTime;
            lastTime = currentTime;
            update(dt.count());
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }    
}