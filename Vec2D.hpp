#ifndef PVECTOR_HPP
#define PVECTOR_HPP

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

  //getters
  double getX() const;
  double getY() const;
  void setX(double x);
  void setY(double y);

  // operatori di operazioni composite
  Vec2D& operator+=(Vec2D const& other);
  // operatori di aritmetica
  Vec2D operator-(Vec2D const& other) const;
  Vec2D operator+(Vec2D const& other) const;
  Vec2D operator*(double const& c) const;
  Vec2D operator/(double const& c) const;
  Vec2D rotate(double angle) const;

  void invertX();
  void invertY();


  double magnitude() const;
  double angleBetween(Vec2D const& other) const;
  double dotProduct(Vec2D const& other) const;
};


#endif
