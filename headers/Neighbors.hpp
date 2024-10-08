#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP
#include "Boid.hpp"

struct Neighbors
{
  // neighbors that all the boids see
  std::vector<unsigned int> seen;
  // offset (starting point in seen vector)
  std::vector<unsigned int> offset;
  // number of neighbors for each boid
  std::vector<unsigned int> howMany;

  Neighbors(std::size_t flockSize)
  {
    seen.reserve(flockSize * 150);
    offset.resize(flockSize);
    howMany.resize(flockSize);
  }
};

// funzione per trovare i vicini
inline Neighbors findNeighbors(std::vector<Boid> const& flock,
                               double angleOfVision, double radOfVision)
{
  Neighbors neighbors(flock.size());
  unsigned int currentOffset{0};

  for (unsigned int i = 0; i < flock.size(); ++i) {
    // memorizes the starting point in seen vector for each boid
    neighbors.offset[i] = currentOffset;
    unsigned int countNeighbors{0};

    for (unsigned int j = 0; j < flock.size(); ++j) {
      if (j != i && flock[i].boidCanSee(flock[j], angleOfVision, radOfVision)) {
        neighbors.seen.push_back(flock[j].N);
        ++countNeighbors;
      }
    }
    // saves the number of naighbors
    neighbors.howMany[i] = countNeighbors;
    currentOffset += countNeighbors;
  }
  return neighbors;
};

#endif