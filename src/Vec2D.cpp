#include </home/rlicata/Progetto_stormi/Prove_efficienza_codice/Vec2D.hpp>
#include <cmath>
#include <math.h>


double Vec2D::getX() const {
    return x_;
  }

double Vec2D::getY() const {
    return y_;
  }

Vec2D& Vec2D::operator+=(Vec2D const& other)
{
  x_ += other.x_;
  y_ += other.y_;
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

Vec2D Vec2D::operator*(double const& c) const 
{
    return Vec2D{x_ * c, y_ * c};
}

Vec2D Vec2D::operator/(double const& c) const 
{
    return Vec2D{x_ / c, y_ / c};
}

bool Vec2D::operator!=(Vec2D const& other) const
{
  return (x_ != other.x_) && (y_ != other.y_);
}

double Vec2D::dotProduct(Vec2D other) const
{
  return x_ * other.x_ + y_ * other.y_;
}

double Vec2D::magnitude() const
{
  return std::sqrt(x_ * x_ + y_ * y_);
}

double Vec2D::angleBetween(Vec2D const& other) const
{
  return std::acos(dotProduct(other) / (magnitude() * other.magnitude()))*(180/M_PI);
}
