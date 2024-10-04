#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Neighbors.hpp"

struct Corrections
{
  std::vector<Vec2D> separation;
  std::vector<Vec2D> alignment;
  std::vector<Vec2D> cohesion;
  std::vector<Vec2D> sumCorr;

  // Costruttore della struct Corrections
  Corrections(size_t flockSize)
  {
    separation.resize(flockSize); // non è maglio reserve??
    alignment.resize(flockSize);
    cohesion.resize(flockSize);
    sumCorr.resize(flockSize);
  }
};

class Flock
{
  double a_{};
  double c_{};
  double s_{};

 public:
  std::vector<Boid> flock_; // questo lo devo poter modificare

  Flock(std::vector<Boid> const& flock, double a, double c, double s);

  void compute(Corrections& corr);
  void evolve(double delta_t, unsigned int display_width,
              unsigned int display_height);
};

#endif