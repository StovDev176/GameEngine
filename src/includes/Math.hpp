#pragma once
#include <iostream>
#include <cmath>

struct vector3 {
  float x, y, z;

  vector3() : x(0.0f), y(0.0f), z(0.0f) {} 

  vector3(float x, float y, float z) : x(x), y(y), z(z) {}
  
  float Magnitude() const {
    return std::sqrt(x*x + y*y + z*z);
  }
  
  vector3 Unit() const {
    float mag = Magnitude();
    return vector3(x/mag, y/mag, z/mag);
  }

  vector3 operator+(const vector3& other) const {
    return vector3(x + other.x, y + other.y, z + other.z);
  }
  
  vector3 operator-(const vector3& other) const {
    return vector3(x - other.x, y - other.y, z - other.z);
  }
  
  vector3 operator*(float scalar) const {
    return vector3(x * scalar, y * scalar, z * scalar);
  }

  vector3 operator/(float scalar) const {
    return vector3(x / scalar, y / scalar, z / scalar);
  }

  float Dot(const vector3& other) const {
    return ((x * other.x) + (y * other.y) + (z * other.z));
  }

  vector3 Lerp(const vector3& other, float t) const {
    return vector3(x + (other.x - x) * t, y + (other.y - y) * t, z + (other.z - z) * t);
  }

  void operator+=(const vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }
}

void operator-=(const vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
};

std::ostream& operator<<(std::ostream& os, const vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

float cubicLerp(float t) {
  return t * t * (3.0f - 2.0f * t);
}

float sineLerp(float t) {
  return -(std::cos(3.14159f * t) - 1.0f) / 2.0f;
}