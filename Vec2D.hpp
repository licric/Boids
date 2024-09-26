#ifndef PVECTOR_HPP
#define PVECTOR_HPP

class Vec2D
{
  double x_{};
  double y_{};

 public:
  // Costruttore di default
  Vec2D()
      : x_(0)
      , y_(0)
  {}

  // costruttore
  Vec2D(double x, double y)
      : x_{x}
      , y_{y}
  {}

  //getters
  double getX() const;
  double getY() const;

  // operatori di operazioni composite
  Vec2D& operator+=(Vec2D const& other);
  // operatori di aritmetica
  Vec2D operator-(Vec2D const& other) const;
  Vec2D operator+(Vec2D const& other) const;
  Vec2D operator*(double const& c) const;
  Vec2D operator/(double const& c) const;

  
  //bool operator==(Vec2D const& other) const;
  bool operator!=(Vec2D const& other) const;
  

  double magnitude() const;
  double angleBetween(Vec2D const& other) const;
  double dotProduct(Vec2D other) const;
};


#endif
