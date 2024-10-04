#include "Flock.hpp"
// da modificare la posizione di queste
const double maxVel = 160.0; // 160
const double minVel = 80.0;  // 80

Flock::Flock(std::vector<Boid> const& flock, double a, double c, double s)
    : a_{a}
    , c_{c}
    , s_{s}
    , flock_{flock}
{}

void Flock::compute(Corrections& corr)
{
  Neighbors neighbors = findNeighbors(flock_, 90., 300.);

  for (unsigned int N = 0; N < flock_.size(); ++N) {
    unsigned int start = neighbors.offset[N];
    unsigned int end   = start + neighbors.howMany[N];
    unsigned int countTooClose{0};

    corr.cohesion[N] = std::accumulate(
        neighbors.seen.begin() + start, neighbors.seen.begin() + end, Vec2D(),
        [&](Vec2D const& acc, unsigned int seenIndex) {
          return acc + flock_[seenIndex].pos;
        });
    corr.alignment[N] = std::accumulate(
        neighbors.seen.begin() + start, neighbors.seen.begin() + end, Vec2D(),
        [&](Vec2D const& acc, unsigned int seenIndex) {
          return acc + flock_[seenIndex].vel;
        });
    std::for_each(neighbors.seen.begin() + start, neighbors.seen.begin() + end,
                  [&](unsigned int neighborIndex) {
                    if (flock_[N].isTooClose(flock_[neighborIndex], 45.)) {
                      corr.separation[N] += flock_[neighborIndex].pos;
                      ++countTooClose;
                    }
                  });

    if (countTooClose > 0) {
      corr.separation[N] -= flock_[N].pos * static_cast<double>(countTooClose);
      corr.separation[N] *= -s_;
    }
    if (neighbors.howMany[N] > 1) {
      corr.alignment[N] /= static_cast<double>(neighbors.howMany[N] - 1);
      corr.alignment[N] -= flock_[N].vel;
      corr.alignment[N] *= a_;

      corr.cohesion[N] /= static_cast<double>(neighbors.howMany[N]);
      corr.cohesion[N] -= flock_[N].pos;
      corr.cohesion[N] *= c_;
      // QUI HO TOLTO IL -1 PERCHè VEDI NOTION
    }

    corr.sumCorr[N] = corr.separation[N] + corr.alignment[N] + corr.cohesion[N];
  }
}

// parte sostituita con algoritmi
/*
for (unsigned int i = start; i < end; ++i) {
      if (neighbors.howMany[N] > 1) {
        corr.cohesion[N] += flock_[neighbors.seen[i]].pos;
        corr.alignment[N] += flock_[neighbors.seen[i]].vel;
      }
      if (flock_[N].isTooClose(flock_[neighbors.seen[i]], 45.)) {
        corr.separation[N] += flock_[neighbors.seen[i]].pos;
        countTooClose++;
      }
    }
*/

void Flock::evolve(double delta_t, unsigned int display_width,
                   unsigned int display_height)
{
  Corrections corr(flock_.size());
  compute(corr);

  for (unsigned int i = 0; i != flock_.size(); ++i) {
    // limitazione della velocità di sterzo se il vettore correzione non è nullo
    if (corr.sumCorr[i].getX() != 0 || corr.sumCorr[i].getY() != 0) {
      naturalVeer(corr.sumCorr[i], flock_[i].vel);
    }
    // aggiornamento velocità con correzioni
    flock_[i].vel += corr.sumCorr[i];
    // rinormalizzazione vettore velocità con max e min velocities
    
    // aggiornamento posizioni
    flock_[i].pos += flock_[i].vel * delta_t; // aggiornamento posizioni

    /*
    // spazio chiuso con cornici
    if (flock_[i].pos.getX() < -100 ) {  //-200
      flock_[i].pos.setX(0);
      flock_[i].vel.invertX();
    }
    if (flock_[i].pos.getX() > display_width + 100) { //+ 200
      flock_[i].pos.setX(display_width +100);
      flock_[i].vel.invertX(); // punto -1.??
    }
    if (flock_[i].pos.getY() < -100) {
      flock_[i].pos.setY(0);
      flock_[i].vel.invertY();
    }
    if (flock_[i].pos.getY() > display_height + 100) {
      flock_[i].pos.setY(display_height +100);
      flock_[i].vel.invertY();
    }
    */

    // spazio aperto, toroide
    if (flock_[i].pos.getX() < 0) {
      flock_[i].pos.setX(flock_[i].pos.getX() + display_width);
    }
    if (flock_[i].pos.getX() > display_width) {
      flock_[i].pos.setX(flock_[i].pos.getX() - display_width);
    }
    if (flock_[i].pos.getY() < 0) {
      flock_[i].pos.setY(flock_[i].pos.getY() + display_height);
    }
    if (flock_[i].pos.getY() > display_height) {
      flock_[i].pos.setY(flock_[i].pos.getY() - display_height);
    }
  }
}
