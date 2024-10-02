#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Neighbors.hpp"

#include <cmath>

struct Corrections {
    std::vector<Vec2D> separation;
	  std::vector<Vec2D> alignment;
	  std::vector<Vec2D> cohesion;

    // Costruttore della struct Corrections
    Corrections(size_t flockSize) {
        // Riserva la memoria per evitare riallocazioni dinamiche
        separation.resize(flockSize);  // non è maglio reserve??
        alignment.resize(flockSize);  
        cohesion.resize(flockSize);  
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
  
  double getCoeff(char coeff);  //non ha senso

  void compute(Corrections& corr);
  void evolve(double const& delta_t, unsigned int const& display_width, unsigned int const& display_height);

};

#endif