#pragma once
#include "Math.hpp"
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