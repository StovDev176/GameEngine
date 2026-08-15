#pragma once
#include "ECS.hpp"
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
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void update(float dt) {
        for (auto& e : events) {
            e.event(dt);
        }
        events.erase(
            std::remove_if(events.begin(), events.end(), 
                [](const Event& e) { return e.oneShot; }
            ), 
            events.end()
        );
    }
    void stop() {
        isRunning = false;
    }
};