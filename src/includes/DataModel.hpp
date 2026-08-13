#pragma once
#include "Math.hpp"

#include <iostream>
#include <vector>

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