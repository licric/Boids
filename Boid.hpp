#ifndef BOID_HPP
#define BOID_HPP

#include "Vec2D.hpp"
#include <vector>


//dobbiamo mettere una classe? forse è meglio
struct Boid
{ 
  unsigned int N{};
  Vec2D pos;
  Vec2D vel;

  double distSquared(Boid const& other) const;
  bool boidCanSee(Boid const& other, double const& angleOfVision, double const& radOfVision)const;

  //bool operator==(Boid const& other) const;
  //bool operator!=(Boid const& other) const;

};


#endif
