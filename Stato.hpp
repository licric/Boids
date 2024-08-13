#ifndef STATO_HPP
#define STATO_HPP

#include <UState_Pvector.hpp>

struct Boid
{ // stato dei singoli uccelli. Numero progressivo, posizione e
  // velocità in x,y
  int N{};
  Vec2D pos;
  Vec2D vel;
  double dist(Boid const& other) const;
  bool boidCanSee(Boid const& other, double const& angleOfVision) const;
};

// double dist(Stato const &u1, Stato const &u2);

#endif