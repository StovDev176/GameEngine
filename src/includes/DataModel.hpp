#pragma once
#include "Math.hpp"

#include <iostream>
#include <vector>

class SceneNode : public std::enable_shared_from_this<SceneNode> {
private:
  vector3 pos;
  std::vector<std::shared_ptr<SceneNode>> children;
  std::weak_ptr<SceneNode> parent;
public: 
  SceneNode(const vector3& pos) : pos(pos) {}

  void move(const vector3& newPos) { pos = newPos; }  
  vector3 getPos() const { return pos; }

  void addChild(std::shared_ptr<SceneNode> child) {
    child->parent = shared_from_this();
    children.push_back(std::move(child));
  }

  vector3 getWorldPos() const {
    auto lockedParent = parent.lock();
    if (lockedParent != nullptr) {
        return pos + lockedParent->getWorldPos(); 
    } else {
        return pos;
    }
  }  
};