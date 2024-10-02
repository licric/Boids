#include "Flock.hpp"
#include <numeric>
// da modificare la posizione di queste
const double max_velocity = 130.0;
const double min_velocity = 80.0;


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
  Neighbors neighbors = findNeighbors(flock_, 70., 300.);
  
  for (unsigned int N = 0; N < flock_.size(); ++N) {  //meglio inizializz con int N{0}
    unsigned int start = neighbors.offset[N];
    unsigned int end = start + neighbors.howMany[N];
    unsigned int countCloseBoids{0};

    //std::cout << "Vicini al numero " << flock_[N].N << ": ";
    
    //if (neighbors.howMany[N] > 1){
	    for (unsigned int i = start; i < end; ++i) {       //meglio inizializz con int N{0}??
        //std::cout<<neighbors.seen[i]<<", ";
        if (neighbors.howMany[N] > 1){
          corr.cohesion[N] += flock_[neighbors.seen[i]].pos;
          corr.alignment[N] += flock_[neighbors.seen[i]].vel;
        }
        //std::cout << "dist : "<< flock_[N].distSquared(flock_[neighbors.seen[i]])<<'\n';
      
        if (flock_[N].distSquared(flock_[neighbors.seen[i]]) < 3000.){
          corr.separation[N] += flock_[neighbors.seen[i]].pos;
          countCloseBoids ++;
        }
	    }
        //std::cout<<'\n';
        //std::cout<<"sep prima boid compute() "<< N <<": "<< corr.separation[N].getX()<<", "<<corr.separation[N].getY()<<'\n';
        if (countCloseBoids != 0){
		      corr.separation[N] = (corr.separation[N] - flock_[N].pos*static_cast<double>(countCloseBoids)) * -s_ ;
        }
        //std::cout<<"sep dopo boid "<< N <<": "<< corr.separation[N].getX()<<", "<<corr.separation[N].getY()<<'\n';
        if (neighbors.howMany[N] > 1){
          corr.alignment[N] = (corr.alignment[N]/(static_cast<double>(neighbors.howMany[N] - 1)) - flock_[N].vel) * a_;
          corr.cohesion[N] = (corr.cohesion[N]/(static_cast<double>(neighbors.howMany[N] - 1)) - flock_[N].pos) * c_; 
        }
    //}
    //std::cout<<'\n';
  }
};


void Flock::evolve(double const& delta_t, unsigned int const& display_width, unsigned int const& display_height)
{
  Corrections corr(flock_.size());
  compute(corr);

  for (unsigned int i = 0; i != flock_.size(); ++i) {

    Vec2D sumCorr{corr.alignment[i]+corr.cohesion[i]+corr.separation[i]}; 
    //std::cout<<"sumCorr "<< sumCorr.getX()<<", "<<sumCorr.getY()<<'\n';

    
    if(sumCorr.getX() != 0  &&  sumCorr.getY() != 0){
    //std::cout<<"sep boid evolve() "<< corr.separation[i].getX()<<", "<<corr.separation[i].getY()<<'\n';

    // limitazione della velocità angolare 
    
    double theta = flock_[i].vel.angleBetween(sumCorr);  
    if (theta > 1. && theta < 175.) {
      sumCorr = sumCorr * (flock_[i].vel.magnitude() / sumCorr.magnitude()) * 0.0175/ std::abs(std::sin(theta*M_PI/180.));
    }else{
      sumCorr.rotate(-theta + 175.); // -(theta - 177.)
      sumCorr = sumCorr * (flock_[i].vel.magnitude() / sumCorr.magnitude()) * 0.0175/ std::abs(std::sin(theta*M_PI/180.));
    }
    }
    
    //std::cout<<"sumCorr "<< sumCorr.getX()<<", "<<sumCorr.getY()<<'\n';
    //std::cout<<"vel prima boid "<<flock_[i].N<<": "<<flock_[i].vel.getX()<<", "<<flock_[i].vel.getY()<<'\n';
    //std::cout<< "correzione: " << sumCorr.getX() << ", " << sumCorr.getY() <<'\n';
    //std::cout<< "alignment: " << corr.alignment[i].getX() << ", " << corr.alignment[i].getY()  <<'\n';
    //std::cout<< "cohesion: " << corr.cohesion[i].getX() << ", " << corr.cohesion[i].getY()  <<'\n';
    //std::cout<< "separation: " << corr.separation[i].getX() << ", " << corr.separation[i].getY()  <<'\n';
    
    //std::cout<<"vel prima boid "<<flock_[i].N<<": "<<flock_[i].vel.getX()<<", "<<flock_[i].vel.getY()<<'\n';
    flock_[i].vel += sumCorr; // aggiornamento velocità con correzioni
    //std::cout<<"vel dopo boid "<<flock_[i].N<<": "<<flock_[i].vel.getX()<<", "<<flock_[i].vel.getY()<<'\n';

    // rinormalizzazione vettore velocità con max e min velocities

    if (flock_[i].vel.magnitude() > max_velocity) {
      flock_[i].vel = (flock_[i].vel / flock_[i].vel.magnitude())* max_velocity;   
    }
    if (flock_[i].vel.magnitude() < min_velocity) {
      flock_[i].vel = (flock_[i].vel / flock_[i].vel.magnitude())* min_velocity;
    }

    //std::cout<<"pos prima boid "<<flock_[i].N<<": "<<flock_[i].pos.getX()<<", "<<flock_[i].pos.getY()<<'\n';
    flock_[i].pos += flock_[i].vel * delta_t; // aggiornamento posizioni
    //std::cout<<"pos dopo boid "<<flock_[i].N<<": "<<flock_[i].pos.getX()<<", "<<flock_[i].pos.getY()<<'\n'<<'\n';

    
    // spazio chiuso con cornici
    if (flock_[i].pos.getX() < 0) {  //-200
      flock_[i].pos.setX(0);
      flock_[i].vel.invertX(); 
    }
    if (flock_[i].pos.getX() > display_width -100) { //+ 200
      flock_[i].pos.setX(display_width -100);
      flock_[i].vel.invertX(); // punto -1.??
    }
    if (flock_[i].pos.getY() < 0) {
      flock_[i].pos.setY(0);
      flock_[i].vel.invertY();
    }
    if (flock_[i].pos.getY() > display_height -100) {
      flock_[i].pos.setY(display_height -100);
      flock_[i].vel.invertY();
    }
    
  }
}

/* 
if (flock_[i].pos.getX() < -200) {
      flock_[i].vel.invertX() = std::abs(flock_[i].vel.getX());
    };
    if (flock_[i].pos.getX() > display_width +200) {
      flock_[i].vel.getX() = (-1) * std::abs(flock_[i].vel.getX()); //punto -1.??
    };
    if (flock_[i].pos.getY() < -200) {
      flock_[i].vel.getY() = std::abs(flock_[i].vel.getY());
    };
    if (flock_[i].pos.getY() > display_height -20) {
      flock_[i].vel.getY() = (-1) * std::abs(flock_[i].vel.getY());
    };

*/