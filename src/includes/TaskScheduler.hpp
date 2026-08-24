#pragma once
#include "ECS.hpp"
#include "raylib.h"
#include <iostream>
#include "Math.hpp"
#include <chrono>
#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

struct Event {  
    std::function<void(float)> event;
    int priority;
    bool oneShot;
    Event(std::function<void(float)> event, int priority, bool oneShot) : event(event), priority(priority), oneShot(oneShot) {}  
    ~Event() {}
};

class TaskScheduler {
private:    
    std::vector<Event> events;
    bool isRunning = true;
    float accumulator = 0.0f;
public:
    void insertEvent(const Event& event) {
        events.push_back(event);
        std::sort(events.begin(), events.end(),
         [](const Event& e1, const Event& e2) {
            return e1.priority < e2.priority;
        });
    }

    void start() {
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (isRunning) {
            auto currentTime = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float> dt = currentTime - lastTime;
            lastTime = currentTime;
            update(dt.count());
        }
    }

    void update(float frameTime) {    
        const float FIXED_DT = 1/60.0f;         
        if (frameTime > 0.25f) frameTime = 0.25f;  
        accumulator += frameTime;

        while (accumulator >= FIXED_DT) { 
            for (auto& e : events)
                if (!e.oneShot) e.event(FIXED_DT);  
            accumulator -= FIXED_DT;
        }

        for (auto& e : events)                
            if (e.oneShot) e.event(frameTime);

        events.erase(
            std::remove_if(events.begin(), events.end(),
                [](const Event& e) { return e.oneShot; }),
            events.end());
    }
    void stop() {
        isRunning = false;
    }
};