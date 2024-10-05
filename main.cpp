#include "Flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <algorithm> //NON li abbiamo usati per ora
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>
// Funzione per calcolare la distanza media tra tutte le coppie di boids
double meanDistance(Flock const& flock)
{
  auto const& boids = flock.flock_; // Accede al vettore di boids
  double sum_distance{0};
  int count{0};

  for (unsigned int i = 0; i < boids.size() - 1; ++i) {
    for (unsigned int j = i + 1; j < boids.size(); ++j) {
      double dx       = boids[i].pos.getX() - boids[j].pos.getX();
      double dy       = boids[i].pos.getY() - boids[j].pos.getY();
      double distance = std::sqrt(dx * dx + dy * dy);
      sum_distance += distance;
      ++count;
    }
  }

  if (count == 0) {
    return 0.0;
  }

  else {
    return sum_distance / count;
  }
}

// Funzione per calcolare la deviazione standard delle distanze tra tutte le
// coppie di boids
double stdDevDistance(Flock const& flock, double mean)
{
  auto const& boids = flock.flock_;
  double sum_squared_diff{0};
  int count{0};

  for (unsigned int i = 0; i < boids.size() - 1; ++i) {
    for (unsigned int j = i + 1; j < boids.size(); ++j) {
      double dx       = boids[i].pos.getX() - boids[j].pos.getX();
      double dy       = boids[i].pos.getY() - boids[j].pos.getY();
      double distance = std::sqrt(dx * dx + dy * dy);
      double diff     = distance - mean;
      sum_squared_diff += diff * diff;
      ++count;
    }
  }

  if (count == 0) {
    return 0.0;
  } else {
    return std::sqrt(sum_squared_diff / count);
  }
}

int main()
{
  try {
    // display
    auto const display_width  = sf::VideoMode::getDesktopMode().width;
    auto const display_height = sf::VideoMode::getDesktopMode().height;

    std::cout << "Display width = " << display_width << " ; "
              << "Display height : " << display_height << '\n';

    // variabili
    auto const delta_t{sf::milliseconds(33)};

    // creazione di  "Flock f"
    unsigned int numBoids;
    double a;
    double c;
    double s;
    char bruh;

    std::cout << "inserire il numero dei boids: " << '\n';
    std::cin >> numBoids;

    std::cout << "cambiare i coefficienti? (Y : yes   N : no)" << '\n';
    std::cin >> bruh;
    if (bruh == 'Y') {
      std::cout << "inserire i coefficienti: " << '\n';
      std::cin >> a >> c >> s;
    } else {
      a = 1.7;
      c = 3.;
      s = 6.;
    }

    // creazione flock
    std::vector<Boid> flock(numBoids);
    // inizializzazione immediata visto che conosco già grandezza,
    // evita riallocazioni multiple della memoria man mano che
    // riempio

    std::random_device r;
    std::default_random_engine gen{r()};

    // riempimento random
    for (unsigned int i = 0; i != numBoids; i++) {
      std::uniform_real_distribution<double> random_height(200., display_height
                                                                     - 300.);
      std::uniform_real_distribution<double> random_width(200.,
                                                          display_width - 300.);
      std::uniform_real_distribution<double> random_velocity(-50., 1.);

      flock[i].pos.setX(random_width(gen));
      flock[i].pos.setY(random_height(gen));
      flock[i].vel.setX(random_velocity(gen));
      flock[i].vel.setY(random_velocity(gen));

      flock[i].N = i;
    }

    Flock f{flock, a, c, s};

    // creazione finestra sfml
    sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                            "Boids simulation");
    window.setVerticalSyncEnabled(true);

    // caricamento immagine e testi
    /*sf::Font font;
    if (!font.loadFromFile("../Boids_programmazione_24/Roboto-Regular.ttf")) {
      // Gestisci l'errore se il font non viene caricato
      return -1;
    }

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(24);
    statsText.setFillColor(sf::Color::Black);
    statsText.setPosition(10.f, 10.f); // Posizione del testo nella finestra
*/
    sf::Texture texture;
    if (!texture.loadFromFile("../images/Boid.png")) {
      throw std::runtime_error(
          "Impossibile caricare la texture '../images/Boid.png'");
      window.close();
      return 0;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.019f, 0.019f);

    sf::Texture texture1;
    if (!texture1.loadFromFile("../images/cielo.jpg")) {
      throw std::runtime_error(
          "Impossibile caricare la texture '../images/Boid.png'");
      window.close();
      return 0; // bisogna lasciare anche queste? Bisogna fare una funzione
    }
    sf::Sprite sprite1;
    sprite1.setTexture(texture1);
    sprite1.setScale(1.17f, 1.17f);

    bool wait = false;

    // ciclo per stampa continua dei frame

    window.setFramerateLimit(30);
    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
      }
      if (wait == true) { // codice quando il gioco è in pausa
      }

      if (wait == false) {
        window.clear(sf::Color::White);
        window.draw(sprite1);

        double const dt{delta_t.asSeconds()};
        f.evolve(dt, display_width, display_height);

        for (auto& u : f.flock_) {
          sprite.setPosition(static_cast<float>(u.pos.getX()),
                             static_cast<float>(u.pos.getY()));
          double angle = std::atan2(u.vel.getY(), u.vel.getX()) * 180 / M_PI;
          sprite.setRotation(static_cast<float>(angle + 90.));
          window.draw(sprite);
        }

        /*double mean    = meanDistance(f);
        double std_dev = stdDevDistance(f, mean);

        std::stringstream ss;
        ss << "Distanza media: " << mean
           << "\nDeviazione standard: " << std_dev;
        statsText.setString(ss.str());

        window.draw(statsText);*/
        window.display();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Errore irreversibile: " << e.what() << std::endl;
    return EXIT_FAILURE; // posizione giusta di catch?
  } catch (...) {
    std::cerr << "Errore non previsto." << std::endl;
    return EXIT_FAILURE;
  }
}
