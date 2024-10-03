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
    std::cin>>a>>c>>s;
  }else{
    a = 1.7;
    c = 3.;
    s = 6.;
  }

  //creazione flock
  std::vector<Boid> flock(numBoids);  //inizializzazione immediata visto che conosco già grandezza, evita riallocazioni multiple della memoria man mano che riempio 
  
  std::random_device r;
  std::default_random_engine gen{r()};

  //riempimento random
  for (unsigned int i = 0; i != numBoids; i++) {
    std::uniform_real_distribution<double> random_height(200., display_height - 300.);
    std::uniform_real_distribution<double> random_width(200., display_width - 300.);
    std::uniform_real_distribution<double> random_velocity(-50., 1.);

    flock[i].pos.setX(random_width(gen));
    flock[i].pos.setY(random_height(gen));
    flock[i].vel.setX(random_velocity(gen));
    flock[i].vel.setY(random_velocity(gen));

    flock[i].N = i;
  }

  Flock f{flock, a, c, s};
  
  //creazione finestra sfml
  sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Boids simulation");
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
  
  window.setFramerateLimit(30);
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

