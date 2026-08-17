#pragma once
#include "Math.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include <memory>
#include <iostream>
#include <optional>
#include <vector>
#include "raylib.h"

struct Raycast {
  vector3 origin;
  vector3 direction;
  Raycast(const vector3& origin, const vector3& direction) : origin(origin), direction(direction) {}
};

struct Shape {
  virtual ~Shape() = default;
  virtual std::optional<vector3> intersect(const Raycast& ray) const = 0;
};

struct Sphere : Shape {  
  vector3 center;
  float radius;

  Sphere(const vector3& center, float radius) : center(center), radius(radius) {}

  std::optional<vector3> intersect(const Raycast& ray) const override {
    vector3 dir = ray.direction.Unit();
    vector3 L = center - ray.origin;
    float tca = L.Dot(dir);

    if (tca < 0) return std::nullopt;
    
    float d2 = L.Magnitude() * L.Magnitude() - tca * tca;
    if (d2 > radius * radius) return std::nullopt;
    
    float thc = std::sqrt(radius * radius - d2);
    float t0 = tca - thc;
    
    return ray.origin + (dir * t0);
  }
};

struct Quadrilateral : Shape {
  vector3 size;
  vector3 pos;

  Quadrilateral(const vector3& size, const vector3& pos) : pos(pos), size(size) {}
};

class Scene {
private:    
    std::vector<std::unique_ptr<Shape>> shapes;
public:
    void addShape(std::unique_ptr<Shape> shape) {
        shapes.push_back(std::move(shape));
    }
    
    std::optional<vector3> castRay(const Raycast& ray) const {
        for (const auto& s : shapes) {
            if (auto point = s->intersect(ray)) {
                return point;
            }
        }
        return std::nullopt;
    }
};


struct RigidBody {
    vector3 velocity;
    vector3 acceleration;
    float mass;
    float bounciness;
    vector3 netForce;
};

struct TransformComponent {
    vector3 position;
    vector3 rotation;
    vector3 scale;
};

struct AABB {
    vector3 min; 
    vector3 max; 
};

bool checkCollision(AABB a, AABB b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

void applyGravity(RigidBody& rb) {
  vector3 gravityForce = vector3(0, (-9.81f * rb.mass), 0);
  rb.netForce += gravityForce;
}

void applyForce(RigidBody& rb, vector3 force) {
  rb.acceleration += (force/rb.mass);
}

void updatePhysics(RigidBody& rb,TransformComponent& tfC,  float dt) {
  rb.acceleration = vector3(0, 0, 0);

  if (rb.mass > 0.0f) {
    applyForce(rb, rb.netForce);
  }
  rb.velocity += (rb.acceleration * dt);
  tfC.position += (rb.velocity * dt);

  rb.netForce = vector3(0, 0, 0);
}

void collide(RigidBody& rb1, RigidBody& rb2, float restution) {
  vector3 relativeVelocity = rb1.velocity - rb2.velocity;

  if (relativeVelocity.y > 0) {return;}

  float invMassA = 1.0f / rb1.mass;
  float invMassB = 1.0f / rb2.mass;

  float impulseScalar = -(1.0f + restution) * relativeVelocity.y / (invMassA + invMassB);

  rb1.velocity.y += impulseScalar * invMassA;
  rb2.velocity.y -= impulseScalar * invMassB;
}

void applyFriction(RigidBody& rb, float frictionCoefficient) {
    rb.velocity.x *= frictionCoefficient;
    rb.velocity.z *= frictionCoefficient;
}

void posCorrection(AABB a, AABB b, TransformComponent& tfC) {
  float penetration = b.max.y - a.min.y;
  if (penetration > 0) {
    tfC.position.y += penetration;
  }
} 

AABB createAABB(const TransformComponent& tf) {
    AABB box;
    vector3 halfScale = tf.scale * 0.5f; 
    box.min = tf.position - halfScale;
    box.max = tf.position + halfScale;
    return box;
}