#include "Vec2D.hpp"
#include <math.h>
#include <cmath>

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
  return std::acos(dotProduct(other) / (magnitude() * other.magnitude()))
       * (180 / M_PI);
}
