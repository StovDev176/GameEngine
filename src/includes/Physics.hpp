#pragma once
#include "Math.hpp"
#include <memory>
#include <iostream>
#include <optional>
#include <vector>

struct Ray {
  Vector3 origin;
  Vector3 direction;
  Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction) {}
};

struct Shape {
  virtual ~Shape() = default;
  virtual std::optional<Vector3> intersect(const Ray& ray) const = 0;
};

struct Sphere : Shape {  
  Vector3 center;
  float radius;

  Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}

  std::optional<Vector3> intersect(const Ray& ray) const override {
    Vector3 dir = ray.direction.Unit();
    Vector3 L = center - ray.origin;
    float tca = L.Dot(dir);

    if (tca < 0) return std::nullopt;
    
    float d2 = L.Magnitude() * L.Magnitude() - tca * tca;
    if (d2 > radius * radius) return std::nullopt;
    
    float thc = std::sqrt(radius * radius - d2);
    float t0 = tca - thc;
    
    return ray.origin + (dir * t0);
  }
};

class Scene {
private:    
    std::vector<std::unique_ptr<Shape>> shapes;
public:
    void addShape(std::unique_ptr<Shape> shape) {
        shapes.push_back(std::move(shape));
    }
    
    std::optional<Vector3> castRay(const Ray& ray) const {
        for (const auto& s : shapes) {
            if (auto point = s->intersect(ray)) {
                return point;
            }
        }
        return std::nullopt;
    }
};