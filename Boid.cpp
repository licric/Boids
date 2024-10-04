#include "Boid.hpp"
#include <cmath>

double Boid::distSquared(Boid const& other) const
{
  return (pos.getX() - other.pos.getX()) * (pos.getX() - other.pos.getX())
       + (pos.getY() - other.pos.getY()) * (pos.getY() - other.pos.getY());
}

bool Boid::boidCanSee(Boid const& other, double angleOfVision,
                      double radOfVision) const
{
  if (distSquared(other) < (radOfVision * radOfVision)
      && vel.angleBetween(other.pos - pos) < angleOfVision) {
    return true;
  } else {
    return false;
  }
}

// funzione per determinare se due boid sono troppo vicini
bool Boid::isTooClose(Boid const& other, double threshold) const
{
  return distSquared(other) < threshold * threshold;
}
