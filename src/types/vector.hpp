#pragma once 

#include <cmath>

#include "base_types.hpp"

template<typename T>
struct Vec2 { 
  T x, y;

  Vec2(T x = 0, T y = 0) : x(x), y(y) {}

  Vec2<T> operator -(const Vec2<T> &other) const {
    return Vec2<T>(this->x - other.x, this->y - other.y);
  }

  Vec2<T> operator +(const Vec2<T> &other) const {
    return Vec2<T>(this->x + other.x, this->y + other.y);
  }

  Vec2<T> operator +=(const Vec2<T> &other) {
    return *this = *this + other;
  }

  Vec2<T> operator *(T x) const {
    return Vec2<T>(this->x * x, this->y * x);
  }

  bool operator ==(const Vec2<T> &other) const {
    return this->x == other.x && this->y == other.y;
  }

  f32 distanceTo(const Vec2<T> &other) const {
    return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
  }

  T magnitude() const {
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
  }

  Vec2<T> normalized() const {
    T magnitude = this->magnitude();
    return Vec2<T>(this->x / magnitude, this->y / magnitude);
  }

  Vec2<T> rotate(T degrees) const {
    const T theta = degrees * M_PI / 180;

    const T cs = cos(theta);
    const T sn = sin(theta);

    return { 
      x * cs - y * sn, 
      x * sn + y * cs 
    };
  }
};

template<typename T>
struct Vec3 : Vec2<T> { 
  T z;
  Vec3(T x = 0, T y = 0, T z = 0) : Vec2<T>(x, y), z(z) {}

  Vec3<T> operator -(const Vec3<T> &other) const {
    return Vec3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
  }

  Vec3<T> operator +(const Vec3<T> &other) const {
    return Vec3<T>(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Vec3<T> &operator +=(const Vec3<T> &other) {
    return *this = *this + other;
  }

  Vec3<T> operator *(T x) const {
    return Vec3<T>(this->x * x, this->y * x, this->z * x);
  }

  f32 distanceTo(const Vec3<T> &other) const {
    return sqrt(
      pow(this->x - other.x, 2) + 
      pow(this->y - other.y, 2) + 
      pow(this->z - other.z, 2) 
    );
  }

  T magnitude() const {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
  }

  Vec3<T> normalized() const {
    T magnitude = this->magnitude();
    return Vec3<T>(this->x / magnitude, this->y / magnitude, this->z / magnitude);
  }
};

struct Rgb {
  f32 r, g, b;
  Rgb(f32 r = 0, f32 g = 0, f32 b = 0) : r(r), g(g), b(b) {}
};

struct Rgba : Rgb {
  f32 a;
  Rgba(f32 r = 0, f32 g = 0, f32 b = 0, f32 a = 0) : Rgb(r, g, b), a(a) {}

  Rgba operator +(const Rgba &other) const {
    return Rgba(
      this->r + other.r, 
      this->g + other.g, 
      this->b + other.b, 
      this->a + other.a
    );
  }

  Rgba operator -(const Rgba &other) const {
    return Rgba(
      this->r - other.r, 
      this->g - other.g, 
      this->b - other.b, 
      this->a - other.a
    );
  }

  Rgba &operator +=(const Rgba &other) {
    return *this = *this + other;
  }

  Rgba &operator -=(const Rgba &other) {
    return *this = *this - other;
  }
};