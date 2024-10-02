#include <random>
#include "Flock.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <algorithm> //NON li abbiamo usati per ora

//const double s_ {1.8}; //1.8 per 200
//const double a_ {1.3}; //1.3 per 200
//const double c_ {1.6}; //1.6 per 200


//copiate da loro, da rifare
/*
auto meandistance(Flock& flock) {
  auto uccelli = flock.state();
  double sum_distance{0};
  int h{0};
  for (unsigned int i = 0; i != uccelli.size() - 1; ++i) {
    for (unsigned int j = i + 1; j != uccelli.size(); ++j) {
      auto dx = uccelli[i].P.x - uccelli[j].P.x;
      auto dy = uccelli[i].P.y - uccelli[j].P.y;
      sum_distance += std::sqrt((dx * dx) + (dy * dy));
      ++h;
    }
  }
  auto mean = sum_distance / h;
  return (mean);
}

auto STD(Flock& flock, double mean) {
  auto uccelli = flock.state();
  double d{0.};
  int h{0};
  for (unsigned int i = 0; i != uccelli.size() - 1; ++i) {
    for (unsigned int j = i + 1; j != uccelli.size(); ++j) {
      auto dx = uccelli[i].P.x - uccelli[j].P.x;
      auto dy = uccelli[i].P.y - uccelli[j].P.y;
      double xy = std::sqrt((dx * dx) + (dy * dy));

      d += std::pow((xy - mean), 2);
      ++h;
    }
  }

  auto STD = std::sqrt(d / (h));
  return (STD);
}
*/


int main()
{
  //display
  auto const display_width = sf::VideoMode::getDesktopMode().width;
  auto const display_height = sf::VideoMode::getDesktopMode().height;

  std::cout << "Display width = " << display_width << " ; "
            << "Display height : " << display_height << '\n';

  //variabili
  auto const delta_t{sf::milliseconds(33)};
  int const fps = 30;


  //creazione di  "Flock f"
  unsigned int numBoids;
  double a;
  double c;
  double s;
  char bruh;
 
  std::cout<<"inserire il numero dei boids: "<<'\n';
  std::cin>>numBoids;

  
  std::cout<<"cambiare i coefficienti? (Y : yes   N : no)"<<'\n';
  std::cin>>bruh;
  if ( bruh == 'Y'){  
    std::cout<<"inserire i coefficienti: "<<'\n';
    std::cin>>a>>s>>c;
  }else{
    a = 1.8;
    c = 1.3;
    s = 4.;
  }

  //creazione flock
  std::vector<Boid> flock(numBoids);  //inizializzazione immediata visto che conosco già grandezza, evita riallocazioni multiple della memoria man mano che riempio 
  
  std::random_device r;
  std::default_random_engine gen{r()};

  //riempimento random
  for (unsigned int i = 0; i != numBoids; i++) {
    std::uniform_real_distribution<double> random_height(200., display_height - 300.);
    std::uniform_real_distribution<double> random_width(200., display_width - 300.);
    std::uniform_real_distribution<double> random_velocity(-50., 50.);

    flock[i].pos.setX(random_width(gen));
    flock[i].pos.setY(random_height(gen));
    flock[i].vel.setX(random_velocity(gen));
    flock[i].vel.setY(random_velocity(gen));

    flock[i].N = i;
  }
  
  
  
 /*
 flock[0].pos.setX(150.);
  flock[0].pos.setY(200.);
  flock[0].vel.setX(100.);
  flock[0].vel.setY(0.);
  flock[0].N = 0;


  flock[1].pos.setX(800.);
  flock[1].pos.setY(200.);
  flock[1].vel.setX(-100.);
  flock[1].vel.setY(0.);
  flock[1].N = 1;

  flock[2].pos.setX(800.);
  flock[2].pos.setY(210.);
  flock[2].vel.setX(-100.);
  flock[2].vel.setY(0.);
  flock[2].N = 2;


  flock[3].pos.setX(810.);
  flock[3].pos.setY(210.);
  flock[3].vel.setX(20.);
  flock[3].vel.setY(0.);
  flock[3].N = 3;

  flock[4].pos.setX(800.);
  flock[4].pos.setY(205.);
  flock[4].vel.setX(20.);
  flock[4].vel.setY(0.);
  flock[4].N = 4;

  flock[5].pos.setX(810.);
  flock[5].pos.setY(200.);
  flock[5].vel.setX(20.);
  flock[5].vel.setY(0.);
  flock[5].N = 5;
  */

  Flock f{flock, a, c, s};

  
  
  sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Boids simulation");
  window.setFramerateLimit(30); // Limita a 30 FPS
  window.setVerticalSyncEnabled(true);


  //caricamento immagine e testi

  sf::Texture texture;
  if (!texture.loadFromFile("../images/Boid.png")) {
    std::cout << "Could not load texture" << std::endl;
    window.close();
    return 0;
  }
  sf::Sprite sprite;
  sprite.setTexture(texture);
  sprite.setScale(0.019f, 0.019f);

  sf::Texture texture1;
  if (!texture1.loadFromFile("../images/cielo.jpg")) {
    std::cout << "Could not load texture" << std::endl;
    window.close();
    return 0;
  }
  sf::Sprite sprite1;
  sprite1.setTexture(texture1);
  sprite1.setScale(1.17f, 1.17f);



  bool wait = false;

  //ciclo per stampa continua dei frame
  
  window.setFramerateLimit(fps);
  while (window.isOpen()) {
    sf::Event event;

   while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) 
      window.close();
    }

    /*
    if (event.key.code == sf::Keyboard::Enter) {
      wait = true;
    }
    

    if (event.key.code == sf::Keyboard::Escape) {
      wait = false;
    }
    */

    if (wait == true) {  // codice quando il gioco è in pausa
    }

    if (wait == false) {

      window.clear(sf::Color::White);
      window.draw(sprite1);

      double const dt{delta_t.asSeconds()};
      f.evolve(dt, display_width, display_height);

      for (auto& u : f.flock_) {
        sprite.setPosition(static_cast<float>(u.pos.getX()), static_cast<float>(u.pos.getY()));
        double angle = std::atan2(u.vel.getY(), u.vel.getX()) * 180 / M_PI;
        sprite.setRotation(static_cast<float>(angle + 90.));
        window.draw(sprite);
      }

      window.display();

    }
  }

}

