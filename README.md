# Specifications about the key functions of the project

---
- [Specifications about the key functions of the project](#specifications-about-the-key-functions-of-the-project)
  - [Flock::compute(Corrections& corr)](#flockcomputecorrections-corr)
  - [Boid::naturalVeer(Vec2D& sumCorr)](#boidnaturalveervec2d-sumcorr)
  - [Come abbiamo gestito i Neighbors](#come-abbiamo-gestito-i-neighbors)
---

## Flock::compute(Corrections& corr)

Le regole per le correzioni sono: 

$$
\vec{v}_{sep} = -s\sum_{j\ne i}(\vec{x}_{b_j}-\vec{x}_{b_i})=-s(\sum_{j\ne i}\vec{x}_{b_j}-N_{neigh}\cdot \vec{x}_{bi})
$$

$$
\vec{v}_{all} = a(\frac{1}{n-1}\sum_{j\ne i}\vec{v}_{b_j} - \vec{v}_{b_i})
$$

$$
\vec{v}_{coe} = c(\vec{x}_{cm}-\vec{x}_{b_i})
$$

Nella funzione in esame il ciclo for scorre i Boids del flock e le variabili sottostanti servono per leggere i vicini per ogni Boid dal vettore `neighbors.seen` . I due algoritmi seguenti:

```cpp
corr.cohesion[N] = std::accumulate(
        neighbors.seen.begin() + start, neighbors.seen.begin() + end, Vec2D(),
        [&](Vec2D const& acc, unsigned int boidSeenIndex) {
          return acc + flock_[boidSeenIndex].pos;
        });
corr.alignment[N] = std::accumulate(
        neighbors.seen.begin() + start, neighbors.seen.begin() + end, Vec2D(),
        [&](Vec2D const& acc, unsigned int boidSeenIndex) {
          return acc + flock_[boidSeenIndex].vel;
        });
std::for_each(neighbors.seen.begin() + start, neighbors.seen.begin() + end,
                  [&](unsigned int boidSeenIndex) {
                    if (flock_[N].isTooClose(flock_[boidSeenIndex], 45.)) {
                      corr.separation[N] += flock_[boidSeenIndex].pos;
                      ++countTooClose;
                    }
                  });
```

sono l’anagolo delle sommatorie nelle regole di correzione, in particolare nel `for_each` si tiene anche conto sei boids che sono troppo vicini contandoli con `countTooClose` . Le restanti operazioni per `corr.separation` vengono fatte dopo aver verificato che ci siano effettivamente dei boids troppo vicini:

```cpp
if (countTooClose > 0) {
      corr.separation[N] -= flock_[N].pos * static_cast<double>(countTooClose);
      corr.separation[N] *= -s_;
}
```

Per gli altri due coefficienti si controlla che ci siano dei vicini: 

```cpp
if (neighbors.howMany[N] > 1) {
      corr.alignment[N] /= static_cast<double>(neighbors.howMany[N] - 1);
      corr.alignment[N] -= flock_[N].vel;
      corr.alignment[N] *= a_;

      corr.cohesion[N] /= static_cast<double>(neighbors.howMany[N]);
      corr.cohesion[N] -= flock_[N].pos;
      corr.cohesion[N] *= c_;
}
```

Come si può notare nella correzione cohesion non è stato sottratto 1 al numero **n** di vicini, perchè questa condizione scaturiva comportamenti anomali nella simulazione.

## Boid::naturalVeer(Vec2D& sumCorr)

Questa funzione membro della classe Boid, chiamata dalla funzione `evolve()` in Flock.cpp, serve per garantire che la velocità di sterzo/virata del Boid non risulti repentina. La dinamica di volo degli uccelli con questa regola migliora notevolmente e la simulazione risulta più realistica. 

Si tratta di una rinormalizzazione del vettore 2D `sumCorr[N]` in modo tale che la somma con `flock_[N].vel` non formi con angolo maggiore di 3/5° con la velocità stessa. In questo modo sappiamo che ad ogni chiamata della funzione evolve il Boid può virare al massimo di un angolo di 3/5°. Nella directory del progetto è presente un file che è apribile da geogebra e che permette di graficare interattivamente le relazione tra i vettori in gioco. 

Come si nota dalla simulazione se si oltrepassano i 175° tra la velocità e la correzione, la somma tra la correzione rinormalizzata e la valocità diventa negativa. Per ovviare a questo problema abbiamo messo un `if` per controllare l’angolo tra i due e nell’else abbiamo ruotato `sumCorr[N]` prima di rinormalizzare. 

```cpp
void Boid::naturalVeer(Vec2D& sumCorr)
{
  double theta = vel.angleBetween(sumCorr);
  if (theta > 3. && theta < 175.) {
    sumCorr = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(3. * M_PI / 180.) / std::abs(std::sin(theta * M_PI / 180.));
  } else if (theta >= 175. && theta <= 180.) {
    sumCorr.rotate(-10.);
    double phi = vel.angleBetween(sumCorr);
    sumCorr    = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(3. * M_PI / 180.) / std::abs(std::sin(phi * M_PI / 180.));
  }
}
```

Per rinormalizzare la correzione abbiamo utilizzato il teorema dei seni, arrivando alla seguente espressione:

$$
\vec u_{corr}=\vec u_{corr}\cdot\frac{|\vec u_{vel}|}{|\vec u_{corr}|}\cdot\frac{\sin(3°)}{\sin(\theta)}
$$

## Come abbiamo gestito i Neighbors

Al posto di utilizzare una struttura del tipo `std::vector<std::vector<unsigned int>>` come avevamo inizialmente fatto, questa struttura dati permette di gestire in modo più efficiente gli spazi in memoria. 

Tutti i vicini vengono salvati in un vettore la quale dimensione possiamo stimare, che è `seen` , mentre gli altri due servono per leggere `seen` ed estrapolare l’informazione. 

```cpp
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
```

Nella funzione `compute()` viene infatti mostrato come viene letto il vettore `seen` utilizzando le variabili `start` ed `end` che spostano gli iteratori degli algoritmi nei punti giusti.