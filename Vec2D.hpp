#ifndef PVECTOR_HPP
#define PVECTOR_HPP

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

// limitazione della velocità di sterzo
inline void naturalVeer(Vec2D& sumCorr, Vec2D const& vel)
{
  double theta = vel.angleBetween(sumCorr);
  if (theta > 2. && theta < 175.) {
    sumCorr = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(2. * M_PI / 180.) / std::abs(std::sin(theta * M_PI / 180.));
  } else if (theta >= 175. && theta <= 180.) {
    sumCorr.rotate(-10.);
    double phi = vel.angleBetween(sumCorr);
    sumCorr = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(2. * M_PI / 180.) / std::abs(std::sin(phi * M_PI / 180.));
  }
}

#endif
