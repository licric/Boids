# page

### knefoienv

# kjenvle

---

---

```cpp
ct Neighbors {
    std::vector<int> seen // vicini che ogni boid può vedere
    std::vector<int> tooClose;  // vicini a rischio di schianto
    std::vector<int> offset;  // contiene l'offset (l'indice di partenza) per ogni boid
    std::vector<int> howMany;    // contiene il numero di vicini per ogni boid

    // Costruttore della struct Neighbors
    Neighbors(size_t flockSize) {
        // Riserva la memoria per evitare riallocazioni dinamiche
        seenNeighbors.reserve(flockSize * 30);  // Stima empirica: 30 vicini per boid
        tooCloseNeighbors.reserve(flockSize * 10);  // Stima empirica: 10 vicini troppo vicini per boid
        neighborOffset.resize(flockSize);  // Inizializza con flockSize elementi
        numNeighbors.resize(flockSize);    // Inizializza con flockSize elementi
    }
};

//in FLock.cpp

Neighbors findNeighbors(std::vector<Boid> const& flock, double const& angleOfVision, double const& radOfVision) 
{
    Neighbors neighbors(flock.size());  // Crea l'oggetto Neighbors con dimensione corretta
    int currentOffset = 0;
```