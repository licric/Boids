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

// Funzione per calcolare la velocità media di tutti i boids
double meanSpeed(Flock const& flock)
{
  auto const& boids = flock.flock_; // Accede al vettore di boids
  double sum_speed{0};
  long unsigned int count = boids.size();

  for (const auto& boid : boids) {
    sum_speed +=
        boid.vel.magnitude(); // Somma le velocità (modulo del vettore velocità)
  }

  if (count == 0) {
    return 0.0;
  } else {
    return sum_speed / static_cast<double>(count);
  }
}

// Funzione per calcolare la deviazione standard delle velocità
double stdDevSpeed(Flock const& flock, double mean)
{
  auto const& boids = flock.flock_;
  double sum_squared_diff{0};
  long unsigned int count = boids.size();

  for (const auto& boid : boids) {
    double speed = boid.vel.magnitude();
    double diff  = speed - mean;
    sum_squared_diff += diff * diff;
  }

  if (count == 0) {
    return 0.0;
  } else {
    return std::sqrt(sum_squared_diff / static_cast<double>(count));
  }
}

int main()
{
  try {
    // display
    auto const display_width  = 1152;
    auto const display_height = 720;

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

    std::cout << "Inserire il numero dei boids: ";
    while (true) {
      std::cin >> numBoids;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Input non valido. Per favore, inserisci un numero intero "
                     "positivo: ";
      } else {
        break;
      }
    }
    std::cout << '\n' << "Cambiare i coefficienti? (y : yes   n : no)" << '\n';
    std::cin >> bruh;
    if (bruh == 'y') {
      std::cout << "Inserire i coefficienti..." << '\n' << "a: ";
      std::cin >> a;
      std::cout << '\n' << "c: ";
      std::cin >> c;
      std::cout << '\n' << "s: ";
      std::cin >> s;
    } else {
      a = 3.; // 1.8
      c = 3.; // 1.
      s = 7.; // 2.5
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
      if (i < numBoids / 2) {
        std::uniform_real_distribution<double> random_height(200., 600);
        std::uniform_real_distribution<double> random_width(100., 500.);
        std::uniform_real_distribution<double> random_velocity(-50., 50.);
        flock[i].pos.setX(random_width(gen));
        flock[i].pos.setY(random_height(gen));
        flock[i].vel.setX(random_velocity(gen));
        flock[i].vel.setY(random_velocity(gen));
        flock[i].N = i;
      } else {
        std::uniform_real_distribution<double> random_height(200., 600.);
        std::uniform_real_distribution<double> random_width(700., 1100.);
        std::uniform_real_distribution<double> random_velocity(-50., 50.);
        flock[i].pos.setX(random_width(gen));
        flock[i].pos.setY(random_height(gen));
        flock[i].vel.setX(random_velocity(gen));
        flock[i].vel.setY(random_velocity(gen));
        flock[i].N = i;
      }
    }

    Flock f{flock, a, c, s};

    // creazione finestra sfml
    sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                            "Boids simulation");
    window.setVerticalSyncEnabled(true);

    // caricamento immagine e testi
    sf::Font font;

    if (!font.loadFromFile("../others/Roboto-Bold.ttf")) { //
      // Gestisci l'errore se il font non viene caricato
      std::cout << "Could not load texture" << '\n';
      return -1;
    }

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(10.f, 10.f); // Posizione del testo nella finestra

    sf::Texture texture;
    if (!texture.loadFromFile("../images/Boid.png")) {
      std::cerr << "Impossibile caricare la texture '../images/Boid.png'"
                << std::endl;
      window.close();
      return EXIT_FAILURE;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.017f, 0.017f);

    sf::Texture texture1;
    if (!texture1.loadFromFile("../images/cielo.jpg")) {
      throw std::runtime_error(
          "Impossibile caricare la texture '../images/Boid.png'");
      window.close();
      return 0; // bisogna lasciare anche queste? Bisogna fare una funzione
    }
    sf::Sprite sprite1;
    sprite1.setTexture(texture1);
    float scaleX = static_cast<float>(display_width) / texture1.getSize().x;
    float scaleY = static_cast<float>(display_height) / texture1.getSize().y;
    sprite1.setScale(scaleX, scaleY);
    sprite1.setScale(1.17f, 1.17f);

    bool wait = false;

    // ciclo per stampa continua dei frame

    window.setFramerateLimit(60);
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

        double mean          = meanDistance(f);
        double std_dev       = stdDevDistance(f, mean);
        double mean_speed    = meanSpeed(f);
        double std_dev_speed = stdDevSpeed(f, mean_speed);

        std::stringstream ss;
        ss << "Distanza media: " << mean << "\nDeviazione standard: " << std_dev
           << "\nVelocita' media: " << mean_speed << " px/s"
           << "\nDeviazione standard: " << std_dev_speed << " px/s";

        statsText.setString(ss.str());

        window.draw(statsText);
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
  return EXIT_SUCCESS;
}