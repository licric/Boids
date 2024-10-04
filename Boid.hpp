#ifndef BOID_HPP
#define BOID_HPP

#include "Vec2D.hpp"

// dobbiamo mettere una classe? forse è meglio
struct Boid
{
  unsigned int N{};
  Vec2D pos;
  Vec2D vel;

  double distSquared(Boid const& other) const;
  bool boidCanSee(Boid const& other, double const& angleOfVision,
                  double const& radOfVision) const;
  bool isTooClose(Boid const& other, double threshold) const;
};

#endif
