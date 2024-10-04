#include "Vec2D.hpp"
#include <math.h>
#include <cmath>
#include <stdexcept>

double Vec2D::getX() const
{
  return x_;
}

double Vec2D::getY() const
{
  return y_;
}

void Vec2D::setX(double x)
{
  x_ = x;
}

void Vec2D::setY(double y)
{
  y_ = y;
}

Vec2D& Vec2D::operator+=(Vec2D const& other)
{
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

Vec2D& Vec2D::operator-=(Vec2D const& other)
{
  x_ -= other.x_;
  y_ -= other.y_;
  return *this;
}

Vec2D& Vec2D::operator*=(double c)
{
  x_ *= c;
  y_ *= c;
  return *this;
}

Vec2D& Vec2D::operator/=(double c)
{
  x_ /= c;
  y_ /= c;
  return *this;
}

Vec2D Vec2D::operator+(Vec2D const& other) const
{
  return Vec2D{x_ + other.x_, y_ + other.y_};
}

Vec2D Vec2D::operator-(Vec2D const& other) const
{
  return Vec2D{x_ - other.x_, y_ - other.y_};
}

Vec2D Vec2D::operator*(double c) const
{
  return Vec2D{x_ * c, y_ * c};
}

Vec2D Vec2D::operator/(double c) const
{
  if (c == 0) {
    throw std::runtime_error("Divisione per zero in Vec2D::operator/.");
  }
  return Vec2D{x_ / c, y_ / c};
}

void Vec2D::rotate(double angle)
{
  x_ = x_ * std::cos(angle * M_PI / 180.) - y_ * std::sin(angle * M_PI / 180.);
  y_ = x_ * std::sin(angle * M_PI / 180.) + y_ * std::cos(angle * M_PI / 180.);
}

void Vec2D::invertX()
{
  x_ = -x_;
}

void Vec2D::invertY()
{
  y_ = -y_;
}

double Vec2D::dotProduct(Vec2D const& other) const
{
  return x_ * other.x_ + y_ * other.y_;
}

double Vec2D::magnitude() const
{
  return std::sqrt(x_ * x_ + y_ * y_);
}

double Vec2D::angleBetween(Vec2D const& other) const
{
  if (magnitude() == 0 || other.magnitude() == 0) {
    throw std::runtime_error(
        "Impossibile calcolare l'angolo tra vettori con magnitudine zero.");
  }
  double cosTheta = dotProduct(other) / (magnitude() * other.magnitude());
  // Corregge possibili errori numerici che portano cosTheta fuori
  // dall'intervallo [-1, 1]
  cosTheta = std::max(-1.0, std::min(1.0, cosTheta));
  return std::acos(cosTheta) * (180 / M_PI);
}
