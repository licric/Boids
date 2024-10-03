#include "Flock.hpp"
#include <numeric>
// da modificare la posizione di queste
const double max_velocity = 160.0;  //160
const double min_velocity = 80.0;  //80


Flock::Flock(std::vector<Boid> const& flock, double a, double c, double s)
    : a_{a} , c_{c} , s_{s}, flock_{flock}{}

/*
//forse serve se vogliamo modificarli una struttura simile
double Flock::getCoeff(char coeff) {
  switch (coeff) {
        case 'a':
            return a_;  // Alignment coefficient
        case 'c':
            return c_;  // Cohesion coefficient
        case 's':
            return s_;  // Separation coefficient
  }
}
*/


void Flock::compute(Corrections& corr)
{
  Neighbors neighbors = findNeighbors(flock_, 90., 300.);
  
  for (unsigned int N = 0; N < flock_.size(); ++N) {  //meglio inizializz con int N{0}
    unsigned int start = neighbors.offset[N];
    unsigned int end = start + neighbors.howMany[N];
    unsigned int countCloseBoids{0};

	    for (unsigned int i = start; i < end; ++i) {       //meglio inizializz con int N{0}??
        if (neighbors.howMany[N] > 1){
          corr.cohesion[N] += flock_[neighbors.seen[i]].pos;
          corr.alignment[N] += flock_[neighbors.seen[i]].vel;
        }      
        if (flock_[N].distSquared(flock_[neighbors.seen[i]]) < 2000.){
          corr.separation[N] += flock_[neighbors.seen[i]].pos;
          countCloseBoids ++;
        }
	    }
        if (countCloseBoids != 0){
		      corr.separation[N] = (corr.separation[N] - flock_[N].pos*static_cast<double>(countCloseBoids)) * -s_ ;
        }
        if (neighbors.howMany[N] > 1){
          corr.alignment[N] = (corr.alignment[N]/(static_cast<double>(neighbors.howMany[N] - 1)) - flock_[N].vel) * a_;
          corr.cohesion[N] = (corr.cohesion[N]/(static_cast<double>(neighbors.howMany[N])) - flock_[N].pos) * c_;   //QUI HO TOLTO IL -1 PERCHè VEDI NOTION
        }
  }
};


void Flock::evolve(double const& delta_t, unsigned int const& display_width, unsigned int const& display_height)
{
  Corrections corr(flock_.size());
  compute(corr);

  for (unsigned int i = 0; i != flock_.size(); ++i) {
    
    Vec2D sumCorr{corr.alignment[i]+corr.cohesion[i]+corr.separation[i]};    //{corr.alignment[i]+corr.cohesion[i]+corr.separation[i]}; 
  
    if(sumCorr.getX() != 0  ||  sumCorr.getY() != 0){
    // limitazione della velocità di sterzo 
    double theta = flock_[i].vel.angleBetween(sumCorr);  
    if (theta > 2. && theta < 175.) {
      sumCorr = sumCorr * (flock_[i].vel.magnitude() / sumCorr.magnitude()) * 0.035/ std::abs(std::sin(theta*M_PI/180.));
    }
    if (theta >= 175. && theta <= 180. ){
      sumCorr.rotate(-15.); // -(theta - 175.)
      double phi = flock_[i].vel.angleBetween(sumCorr);  
      sumCorr = sumCorr * (flock_[i].vel.magnitude() / sumCorr.magnitude()) * (0.0175/ std::abs(std::sin(phi*M_PI/180.)));
    }
    }  

    flock_[i].vel += sumCorr; // aggiornamento velocità con correzioni
   
    // rinormalizzazione vettore velocità con max e min velocities
    if (flock_[i].vel.magnitude() > max_velocity) {
      flock_[i].vel = (flock_[i].vel / flock_[i].vel.magnitude())* max_velocity;   
    }
    if (flock_[i].vel.magnitude() < min_velocity) {
      flock_[i].vel = (flock_[i].vel / flock_[i].vel.magnitude())* min_velocity;
    }

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
    
    //spazio aperto, toroide
    if (flock_[i].pos.getX() < 0) {
      flock_[i].pos.setX(flock_[i].pos.getX() + display_width);
    };
    if (flock_[i].pos.getX() > display_width) {
      flock_[i].pos.setX(flock_[i].pos.getX() - display_width);
    };
    if (flock_[i].pos.getY() < 0) {
      flock_[i].pos.setY(flock_[i].pos.getY() + display_height);
    };
    if (flock_[i].pos.getY() > display_height) {
      flock_[i].pos.setY(flock_[i].pos.getY() - display_height);
    };
    
    
  }
}
