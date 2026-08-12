#include "includes/ecs.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <optional>

struct Position {
    float x, y, z;
};

struct Vector3 {
  float x, y, z;

  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
  
  float Magnitude() const {
    return std::sqrt(x*x + y*y + z*z);
  }
  
  Vector3 Unit() const {
    float mag = Magnitude();
    return Vector3(x/mag, y/mag, z/mag);
  }

  Vector3 operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }
  
  Vector3 operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }
  
  Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  float Dot(const Vector3& other) const {
    return ((x * other.x) + (y * other.y) + (z * other.z));
  }

  Vector3 Lerp(const Vector3& other, float t) const {
    return Vector3(x + (other.x - x) * t, y + (other.y - y) * t, z + (other.z - z) * t);
  }
};

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

float cubicLerp(float t) {
  return t * t * (3.0f - 2.0f * t);
}

float sineLerp(float t) {
  return -(std::cos(3.14159f * t) - 1.0f) / 2.0f;
}

class Transform : public std::enable_shared_from_this<Transform> {
private:
  Vector3 pos;
  std::vector<std::shared_ptr<Transform>> children;
  std::weak_ptr<Transform> parent;
public: 
  Transform(const Vector3& pos) : pos(pos) {}

  void move(const Vector3& newPos) { pos = newPos; }  
  Vector3 getPos() const { return pos; }

  void addChild(std::shared_ptr<Transform> child) {
    child->parent = shared_from_this();
    children.push_back(std::move(child));
  }

  Vector3 getWorldPos() const {
    auto lockedParent = parent.lock();
    if (lockedParent != nullptr) {
        return pos + lockedParent->getWorldPos(); 
    } else {
        return pos;
    }
  }  
};

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

    Registry registry;
    Entity player = registry.create();
    Entity enemy = registry.create();
    registry.addComponent<Position>();
    ComponentPool<Position>* posPool = registry.getComponentPool<Position>();

    posPool->addData(Position{10.0f, 0.0f, 0.0f}, player);
    posPool->addData(Position{25.0f, 5.0f, 0.0f}, enemy);
    return 0;
}