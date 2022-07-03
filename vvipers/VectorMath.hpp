#ifndef VVIPERS_VECTORMATH_HPP
#define VVIPERS_VECTORMATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

const float pi = 3.14159265358979323846;
const float radPerDeg = pi / 180.f;
const float degPerRad = 180.f / pi;
inline float degToRad(float deg) { return deg * radPerDeg; }
inline float radToDeg(float rad) { return rad * degPerRad; }

template <typename T>
class Vec2 : public sf::Vector2<T> {
  public:
    Vec2(){};
    Vec2(T x, T y) : sf::Vector2<T>(x, y){};
    Vec2(const sf::Vector2<T>& v) : sf::Vector2<T>(v) {}
    template <typename U> Vec2(const sf::Vector2<U>& v) : sf::Vector2<T>(v) {}
    float abs() const { return std::sqrt(this->x * this->x + this->y * this->y); }
    Vec2<T>& normalize(T norm = 1) {
        *this *= norm / this->abs();
        return (*this);
    };
    Vec2<T> perpVec() const { return Vec2<T>(-this->y, this->x); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int32_t> Vec2i;
typedef Vec2<uint32_t> Vec2u;

template <typename T>
inline Vec2<T> operator+(const Vec2<T>& left, const Vec2<T>& right) {
    return Vec2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline Vec2<T> operator-(const Vec2<T>& left, const Vec2<T>& right) {
    return Vec2<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline Vec2<T> operator*(const Vec2<T>& left, const Vec2<T>& right) {
    return Vec2<T>(left.x * right.x, left.y * right.y);
}

template <typename T, typename U>
inline Vec2<T> operator*(const Vec2<T>& left, U right) {
    return Vec2<T>(left.x * right, left.y * right);
}

template <typename T, typename U>
inline Vec2<T> operator*(U left, const Vec2<T>& right) {
    return Vec2<T>(left * right.x, left * right.y);
}

template <typename T>
inline Vec2<T> operator/(const Vec2<T>& left, const Vec2<T>& right) {
    return Vec2<T>(left.x / right.x, left.y / right.y);
}

template <typename T, typename U>
inline Vec2<T> operator/(const Vec2<T>& left, U right) {
    return Vec2<T>(left.x / right, left.y / right);
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    return os << "(x: " << v.x << ", y: " << v.y << ")";
}

template <typename T>
inline float abs(const Vec2<T>& v) {
    return v.abs();
}

template <typename T>
inline Vec2<T> normVec(const Vec2<T>& v) {
    return v / abs(v);
}

// Rotates vector 90 degrees
template <typename T>
inline Vec2<T> perpVec(const Vec2<T>& v) {
    return v.perpVec();
}

template <typename T>
inline float distance(const Vec2<T>& a, const Vec2<T>& b) {
    return abs(b - a);
}

#endif