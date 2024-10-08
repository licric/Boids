#include "../headers/Flock.hpp"

Flock::Flock(std::vector<Boid> const& flock, double a, double c, double s,
             double radOfVision, double radTooClose)
    : a_{a}
    , c_{c}
    , s_{s}
    , radOfVision_{radOfVision}
    , radTooClose_{radTooClose}
    , flock_{flock}
{}

// computing the 3 corrections (cohesion, alignment, separation)
void Flock::compute(Corrections& corr)
{
  Neighbors neighbors = findNeighbors(flock_, angleOfVision_, radOfVision_);

  for (unsigned int N = 0; N < flock_.size(); ++N) {
    unsigned int start = neighbors.offset[N];
    unsigned int end   = start + neighbors.howMany[N];
    unsigned int countTooClose{0};

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
    std::for_each(
        neighbors.seen.begin() + start, neighbors.seen.begin() + end,
        [&](unsigned int boidSeenIndex) {
          if (flock_[N].isTooClose(flock_[boidSeenIndex], radTooClose_)) {
            corr.separation[N] += flock_[boidSeenIndex].pos;
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
    }
    // summing the 3 corrections in one vector
    corr.sumCorr[N] = corr.separation[N] + corr.alignment[N] + corr.cohesion[N];
  }
}

void Flock::evolve(double delta_t, unsigned int display_width,
                   unsigned int display_height)
{
  Corrections corr(flock_.size());
  compute(corr);

  for (unsigned int i = 0; i != flock_.size(); ++i) {
    // limitating the steering speed if sumCorr!={0,0}
    if (corr.sumCorr[i].getX() != 0 || corr.sumCorr[i].getY() != 0) {
      flock_[i].naturalVeer(corr.sumCorr[i]);
    }
    // updating velocity with correction
    flock_[i].vel += corr.sumCorr[i];
    // limiting velocity to max & min values
    if (flock_[i].vel.magnitude() > maxVel
        || flock_[i].vel.magnitude() < minVel) {
      flock_[i].limitVelMaxMin();
    }
    // updating position
    flock_[i].pos += flock_[i].vel * delta_t;

    /*
    // closed space with some borders
    if (flock_[i].pos.getX() < -50) { //-200
      flock_[i].pos.setX(0);
      flock_[i].vel.invertX();
    }
    if (flock_[i].pos.getX() > display_width + 50) { //+ 200
      flock_[i].pos.setX(display_width + 50);
      flock_[i].vel.invertX(); // punto -1.??
    }
    if (flock_[i].pos.getY() < -50) {
      flock_[i].pos.setY(0);
      flock_[i].vel.invertY();
    }
    if (flock_[i].pos.getY() > display_height + 50) {
      flock_[i].pos.setY(display_height + 50);
      flock_[i].vel.invertY();
    }
    */

    // open space (toroid)
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
