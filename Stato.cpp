#include <Stato.hpp>
#include <cmath>

double Boid::dist(Boid const& other) const
{
  double d = std::sqrt((pos.x_ - other.pos.x_) * (pos.x_ - other.pos.x_)
                       + (pos.y_ - other.pos.y_) * (pos.y_ - other.pos.y_));
  return d;
}

bool Boid::boidCanSee(Boid const& other, double const& angleOfVision) const
{
  if (angleBetween(vel, other.pos - pos) < angleOfVision) {  //come faccio a dire la posizione del boid attuale
    return true;
  } else {
    return false;
  }
}
