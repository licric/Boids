#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP
#include <vector>
#include "Boid.hpp"
#include <iostream>

struct Neighbors {
    std::vector<unsigned int> seen; // vicini che ogni boid può vedere
    std::vector<unsigned int> tooClose;  // vicini a rischio di schianto
    std::vector<unsigned int> offset;  // contiene l'offset (l'indice di partenza) per ogni boid
    std::vector<unsigned int> howMany;    // contiene il numero di vicini per ogni boid

    // Costruttore della struct Neighbors
    Neighbors(std::size_t flockSize) {
        // Riserva la memoria per evitare riallocazioni dinamiche
        seen.reserve(flockSize * 30);  // Stima empirica: 30 vicini per boid
        tooClose.reserve(flockSize * 10);  // Stima empirica: 10 vicini troppo vicini per boid
        offset.resize(flockSize);  // Inizializza con flockSize elementi
        howMany.resize(flockSize);    // Inizializza con flockSize elementi
    }
};

inline Neighbors findNeighbors(std::vector<Boid> const& flock, double const& angleOfVision, double const& radOfVision) 
{
    Neighbors neighbors(flock.size());  // Crea l'oggetto Neighbors con dimensione corretta
    unsigned int currentOffset = 0;

    for (unsigned int i = 0; i < flock.size(); ++i) {
        // Memorizza l'inizio dei vicini del boid i
        neighbors.offset[i] = currentOffset;
        unsigned int countNeighbors = 0;

        for (unsigned int j = 0; j < flock.size(); ++j) {
            if (j != i && flock[i].boidCanSee(flock[j], angleOfVision, radOfVision)) {
                neighbors.seen.push_back(flock[j].N); // Aggiungi il boid j come vicino visibile
                ++countNeighbors;
            }
        }
        // Aggiorna il numero di vicini per il boid i
        neighbors.howMany[i] = countNeighbors;
        currentOffset += countNeighbors;
    }
    return neighbors;  // Restituisce l'oggetto Neighbors
};


#endif