#pragma once

#include <iostream>
#include <cmath>

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