#include "Flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

// Functions to calculate the stats about positions of boids
std::pair<double, double> meanAndStdDevDistance(Flock const& f)
{
  auto const& boids = f.flock_;
  double sum_distance{0};
  double sum_squared_diff{0};
  int count{0};

  for (unsigned int i = 0; i < boids.size() - 1; ++i) {
    for (unsigned int j = i + 1; j < boids.size(); ++j) {
      double dx       = boids[i].pos.getX() - boids[j].pos.getX();
      double dy       = boids[i].pos.getY() - boids[j].pos.getY();
      double distance = std::sqrt(dx * dx + dy * dy);
      // Sommiamo le distanze totali
      sum_distance += distance;
      ++count;
      // Incrementiamo la somma delle differenze quadrate rispetto alla med...
      double mean_so_far = sum_distance / count;
      double diff        = distance - mean_so_far;
      sum_squared_diff += diff * diff;
    }
  }
  if (count == 0) {
    return {0.0, 0.0}; // Caso in cui ci siano zero boid
  }
  double mean     = sum_distance / count;     // Calcoliamo la media finale
  double variance = sum_squared_diff / count; // La varianza

  return {mean, std::sqrt(variance)};
}

// Functions to calculate the stats about speeds of boids
std::pair<double, double> meanAndStdDevSpeed(Flock const& flock)
{
  auto const& boids = flock.flock_;
  double sum_speed{0};
  double sum_squared_diff{0};
  long unsigned int count = boids.size();

  for (const auto& boid : boids) {
    double speed = boid.vel.magnitude();
    sum_speed += speed;
    // Calcoliamo la media parziale e aggiorniamo le differenze quadrate
    double mean_so_far = sum_speed / (boid.N + 1);
    double diff        = speed - mean_so_far;
    sum_squared_diff += diff * diff;
  }
  if (count == 0) {
    return {0.0, 0.0};
  }
  double mean     = sum_speed / static_cast<double>(count); // Media finale
  double variance = sum_squared_diff / static_cast<double>(count); // Varianza

  return {mean, std::sqrt(variance)};
}

int main()
{
  try {
    // fixed display size
    auto const dispWidth  = 1152;
    auto const dispHeight = 720;
    std::cout << "Display width = " << dispWidth << " ; "
              << "Display height : " << dispHeight << '\n';

    // gathering the parameters
    unsigned int numBoids;
    double a;
    double c;
    double s;
    double radOfVision;
    double radTooClose;

    char choice;

    const double MIN_COEFF = 0.1;  // Valore minimo accettabile
    const double MAX_COEFF = 10.0; // Valore massimo accettabile

    // Input del numero di boid
    std::cout << "Questo è un programma che simula l'andamento dei boids "
                 "secondo il modello di Craig Reynolds\n"
              << "E' possibile successivamente scegliere i parametri "
                 "fondamentali della simulazione oppure \n"
              << "eseguire il programma con i parametri di default (scegliendo "
                 "di non cambiarli)\n\n\n";
    std::cout << "Inserire il numero dei boids (intero positivo): ";
    while (true) {
      std::cin >> numBoids;
      if (std::cin.fail() || numBoids <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Input non valido. Per favore, inserisci un numero intero "
                     "positivo: ";
      } else {
        break;
      }
    }
    // Chiedi se l'utente vuole cambiare i coefficienti
    while (true) {
      std::cout << "\nCambiare i coefficienti? (y : yes   n : no): ";
      std::cin >> choice;
      if (choice == 'y') {
        // Input del coefficiente a
        std::cout << "Inserire il coefficiente a (double positivo tra "
                  << MIN_COEFF << " e " << MAX_COEFF << "): ";
        while (true) {
          std::cin >> a;
          if (std::cin.fail() || a < MIN_COEFF || a > MAX_COEFF) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input non valido. Inserisci un valore double "
                         "positivo per a tra "
                      << MIN_COEFF << " e " << MAX_COEFF << ": ";
          } else {
            break;
          }
        }
        // Input del coefficiente c
        std::cout << "Inserire il coefficiente c (double positivo tra "
                  << MIN_COEFF << " e " << MAX_COEFF << "): ";
        while (true) {
          std::cin >> c;
          if (std::cin.fail() || c < MIN_COEFF || c > MAX_COEFF) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input non valido. Inserisci un valore double "
                         "positivo per c tra "
                      << MIN_COEFF << " e " << MAX_COEFF << ": ";
          } else {
            break;
          }
        }
        // Input del coefficiente s
        std::cout << "Inserire il coefficiente s (double positivo tra "
                  << MIN_COEFF << " e " << MAX_COEFF << "): ";
        while (true) {
          std::cin >> s;
          if (std::cin.fail() || s < MIN_COEFF || s > MAX_COEFF) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input non valido. Inserisci un valore double "
                         "positivo per s tra "
                      << MIN_COEFF << " e " << MAX_COEFF << ": ";
          } else {
            break;
          }
        }
        std::cout << "Inserire il raggio di separazione:";
        while (true) {
          std::cin >> radTooClose;
          if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input non valido. Inserisci un valore double:";
          } else {
            break;
          }
        }
        std::cout << "Inserire il raggio di visione:";
        while (true) {
          std::cin >> radOfVision;
          if (std::cin.fail() || radOfVision > radTooClose) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input non valido. Inserisci un valore double:";
          } else {
            break;
          }
        }
        break; // Esci dal ciclo dopo aver impostato i coefficienti
      } else if (choice == 'n') {
        // Valori di default se non si cambiano i coefficienti
        a = 1.8;
        c = 1.0;
        s = 2.5;
        break; // Esci dal ciclo
      } else {
        std::cout << "Scelta non valida. Per favore, inserisci 'y' o 'n'.\n";
        // Il ciclo ripete la domanda
      }
    }

    // creation of the Flock (random initial pos & vel)
    std::vector<Boid> flock(numBoids);
    std::random_device r;
    std::default_random_engine gen{r()};

    for (unsigned int i = 0; i != numBoids; i++) {
      if (i < numBoids / 2) {
        std::uniform_real_distribution<double> randomHeight(200., 600);
        std::uniform_real_distribution<double> randomWidth(100., 500.);
        std::uniform_real_distribution<double> randomVel(-80., 8.);
        flock[i].pos.setX(randomWidth(gen));
        flock[i].pos.setY(randomHeight(gen));
        flock[i].vel.setX(randomVel(gen));
        flock[i].vel.setY(randomVel(gen));
        flock[i].N = i;
      } else {
        std::uniform_real_distribution<double> randomHeight(200., 600.);
        std::uniform_real_distribution<double> randomWidth(700., 1100.);
        std::uniform_real_distribution<double> randomVel(-8., 80.);
        flock[i].pos.setX(randomWidth(gen));
        flock[i].pos.setY(randomHeight(gen));
        flock[i].vel.setX(randomVel(gen));
        flock[i].vel.setY(randomVel(gen));
        flock[i].N = i;
      }
    }

    Flock f{flock, a, c, s, radOfVision, radOfVision};

    // rendering the sfml window
    sf::RenderWindow window(sf::VideoMode(dispWidth, dispHeight),
                            "Boids simulation");
    window.setVerticalSyncEnabled(true);

    // loading images and fonts
    sf::Font font;
    if (!font.loadFromFile("../others/Roboto-Bold.ttf")) {
      std::cout << "Could not load texture" << '\n';
      return -1;
    }
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(10.f, 10.f);

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
      return 0;
    }
    sf::Sprite sprite1;
    sprite1.setTexture(texture1);
    sprite1.setScale(1.17f, 1.17f);

    // loop for evolving and frame printing process
    window.setFramerateLimit(60);
    auto const delta_t{sf::milliseconds(33)};

    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
      }

      window.clear(sf::Color::White);
      window.draw(sprite1);

      double const dt{delta_t.asSeconds()};
      f.evolve(dt, dispWidth, dispHeight);

      for (auto& u : f.flock_) {
        sprite.setPosition(static_cast<float>(u.pos.getX()),
                           static_cast<float>(u.pos.getY()));
        double angle = std::atan2(u.vel.getY(), u.vel.getX()) * 180 / M_PI;
        sprite.setRotation(static_cast<float>(angle + 90.));
        window.draw(sprite);
      }

      // print the stats
      std::pair<double, double> statDist  = meanAndStdDevDistance(f);
      std::pair<double, double> statSpeed = meanAndStdDevSpeed(f);
      std::stringstream ss;
      ss << "Distanza media: " << statDist.first
         << "\nDeviazione standard: " << statDist.second
         << "\nVelocita' media: " << statSpeed.first << " px/s"
         << "\nDeviazione standard: " << statSpeed.second << " px/s";
      statsText.setString(ss.str());

      window.draw(statsText);
      window.display();
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