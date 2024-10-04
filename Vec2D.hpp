#ifndef PVECTOR_HPP
#define PVECTOR_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

class Vec2D
{
  double x_;
  double y_;

 public:
  // Costruttore di default
  Vec2D() = default;

  // costruttore
  Vec2D(double x, double y)
      : x_{x}
      , y_{y}
  {}

  // getters
  double getX() const;
  double getY() const;
  void setX(double x);
  void setY(double y);

  // operatori di operazioni composite
  Vec2D& operator+=(Vec2D const& other);
  Vec2D& operator-=(Vec2D const& other);
  Vec2D& operator*=(double c);
  Vec2D& operator/=(double c);

  // operatori di aritmetica
  Vec2D operator-(Vec2D const& other) const;
  Vec2D operator+(Vec2D const& other) const;
  Vec2D operator*(double c) const;
  Vec2D operator/(double c) const;

  void rotate(double angle);
  void invertX();
  void invertY();

  double magnitude() const;
  double angleBetween(Vec2D const& other) const;
  double dotProduct(Vec2D const& other) const;
};


#endif
