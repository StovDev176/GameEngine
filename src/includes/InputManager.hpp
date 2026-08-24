#pragma once
#include <iostream>
#include "raylib.h"
#include <functional>
#include

enum class InputType {MOVE, JUMP, IDLE};

struct Action {
    InputType action;

}

class InputManager {
private:
    InputType previousAction;
public:
    InputManager() : previousAction(InputType::IDLE) {}

    void pressed(InputType action, std::function<void(void)> callback) {
        
    }
}