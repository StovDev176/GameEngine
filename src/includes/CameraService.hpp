#pragma once
#include "raylib.h"
#include "Math.hpp"
#include "Physics.hpp"
#include <vector>
#include "raymath.h" 

void UpdateFreeflyCamera(Camera3D& camera, float speed, float dt) {
    Vector3 moveDirection = { 0.0f, 0.0f, 0.0f };

    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    moveDirection = Vector3Add(moveDirection, forward);
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  moveDirection = Vector3Subtract(moveDirection, forward);
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDirection = Vector3Add(moveDirection, right);
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  moveDirection = Vector3Subtract(moveDirection, right);

    if (Vector3Length(moveDirection) > 0.0f) {
        moveDirection = Vector3Normalize(moveDirection);
        
        Vector3 displacement = Vector3Scale(moveDirection, speed * dt);
        camera.position = Vector3Add(camera.position, displacement);
        camera.target   = Vector3Add(camera.target, displacement); 
    }
}