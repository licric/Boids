#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP
#include "Boid.hpp"
#include <iostream>
#include <vector>

struct Neighbors
{
  // vicini che ogni boid può vedere
  std::vector<unsigned int> seen;
  // contiene l'offset (l'indice di partenza) per ogni boid
  std::vector<unsigned int> offset;
  // contiene il numero di vicini per ogni boid
  std::vector<unsigned int> howMany;

  Neighbors(std::size_t flockSize)
  {
    // Riserva la memoria per evitare riallocazioni dinamiche
    seen.reserve(flockSize * 30); // Stima empirica: 30 vicini per boid
    offset.resize(flockSize);
    howMany.resize(flockSize);
  }
};

// funzione per trovare i vicini
inline Neighbors findNeighbors(std::vector<Boid> const& flock,
                               double angleOfVision, double radOfVision)
{
  Neighbors neighbors(flock.size());
  unsigned int currentOffset = 0;

  for (unsigned int i = 0; i < flock.size(); ++i) {
    // Memorizza l'inizio dei vicini del boid i
    neighbors.offset[i]         = currentOffset;
    unsigned int countNeighbors = 0;

    for (unsigned int j = 0; j < flock.size(); ++j) {
      if (j != i && flock[i].boidCanSee(flock[j], angleOfVision, radOfVision)) {
        neighbors.seen.push_back(flock[j].N);
        ++countNeighbors;
      }
    }
    // Aggiorna il numero di vicini per il boid "i"
    neighbors.howMany[i] = countNeighbors;
    currentOffset += countNeighbors;
  }
  return neighbors;
};

#endif