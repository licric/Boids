#include "Boid.hpp"
#include <cmath>

double Boid::distSquared(Boid const& other) const
{
  return (pos.getX() - other.pos.getX()) * (pos.getX() - other.pos.getX())
                       + (pos.getY() - other.pos.getY()) * (pos.getY() - other.pos.getY());
}

/*
//bool operator!=(Boid const& other) const;
bool Boid::operator!=(Boid const& other) const
{
    return !(*this == other);
}


bool Boid::operator==(Boid const& other) const
{
    return (pos.getX() == other.pos.getX()) &&
           (pos.getY() == other.pos.getY()) &&
           (vel.getX() == other.vel.getX()) &&
           (vel.getY() == other.vel.getY());
}
*/
  
bool Boid::boidCanSee(Boid const& other, double const& angleOfVision, double const& radOfVision) const
{
    if (distSquared(other) < (radOfVision*radOfVision) && vel.angleBetween(other.pos - pos) < angleOfVision) {
      return true;
    }else {
      return false;
    }
}




