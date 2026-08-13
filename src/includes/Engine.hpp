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

template<typename T>
class Event {
private:    
    std::function<void(T)> event;
    int priority;
    float maxTime;
    std::string eventType;
public:
    event(std::function<void(T)> event, int priority, float maxTime) : event(event), priority(priority), maxTime(maxTime) {}  
    void fireEvent(T args) {
        event(args)
    }
}

class taskScheduler {
private:    
    std::priority_queue<Event> events;
    bool isRunning = true;
public:
    void pushBack(const Event& event) {
        events.push(event.priority, )
    }
}