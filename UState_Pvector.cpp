#include <UState_Pvector.hpp>
#include <cmath>

Vec2D& Vec2D::operator+=(Vec2D const& v2)
{
  x_ += v2.x_;
  y_ += v2.y_;
  return *this;
};

Vec2D Vec2D::operator-(Vec2D const& v2)
{
  return Vec2D{x_ - v2.x_, y_ - v2.y_};
}

// double Vec2D::angle() const{
return std::atan2(y_, x_);
}

double Vec2D::dotProduct(Vec2D v2) const
{
  float dot = x * v.x + y * v.y;
  return dot;
}

double Vec2D::magnitude(Vec2D v) const
{
  return std::sqrt(v.x_*v.x_ + v.y_*v.y_);
}

double Vec2D::angleBetween(Vec2D v1, Vec2D v2) const
{
  return std::acos(dotProduct(v2) / (magnitude(v1) * magnitude(v2)));
}
