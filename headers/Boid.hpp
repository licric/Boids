#ifndef BOID_HPP
#define BOID_HPP

#include "Vec2D.hpp"
// global variables
constexpr double maxVel{100.};
constexpr double minVel{70.};

struct Boid
{
  unsigned int N{};
  Vec2D pos;
  Vec2D vel;

  double distSquared(Boid const& other) const;
  bool boidCanSee(Boid const& other, double angleOfVision,
                  double radOfVision) const;
  bool isTooClose(Boid const& other, double radTooClose) const;
  void limitVelMaxMin();
  void naturalVeer(Vec2D& sumCorr);
};

#endif
