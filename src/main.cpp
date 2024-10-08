#include "../headers/Flock.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

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
      sum_distance += distance;
      ++count;

      double mean_so_far = sum_distance / count;
      double diff        = distance - mean_so_far;
      sum_squared_diff += diff * diff;
    }
  }
  if (count == 0) {
    return {0.0, 0.0};
  }
  double mean     = sum_distance / count;
  double variance = sum_squared_diff / count;

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

    double mean_so_far = sum_speed / (boid.N + 1);
    double diff        = speed - mean_so_far;
    sum_squared_diff += diff * diff;
  }
  if (count == 0) {
    return {0.0, 0.0};
  }
  double mean     = sum_speed / static_cast<double>(count);
  double variance = sum_squared_diff / static_cast<double>(count);

  return {mean, std::sqrt(variance)};
}

int main()
{
  try {
    // fixed display size since otherwise it gives rendering problems
    auto const dispWidth  = 1152;
    auto const dispHeight = 720;
    std::cout << "Display width = " << dispWidth << " ; "
              << "Display height : " << dispHeight << '\n';

    // parameters
    unsigned int numBoids;
    double a;
    double c;
    double s;
    double radOfVision;
    double radTooClose;

    char choice;

    const double minCoeffAC = 0.1;
    const double minCoeffS  = 7.;

    std::cout << "This is a program that simulates the behavior of boids "
                 "according to Craig Reynolds' model\n"
              << "You can later choose the fundamental parameters of the "
                 "simulation or \n"
              << "run the program with default parameters (by choosing not to "
                 "change them)\n\n\n";
    std::cout << "Enter the number of boids (positive integer<400): ";
    while (true) {
      std::cin >> numBoids;
      if (std::cin.fail() || numBoids <= 0 || numBoids > 400) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a positive integer<400 : ";
      } else {
        break;
      }
    }
    while (true) {
      std::cout << "\nChange the coefficients? (y : yes   n : no): ";
      std::cin >> choice;
      if (choice == 'y') {
        // Input coefficient a
        std::cout << "Enter coefficient a (positive double grater than: "
                  << minCoeffAC << "): ";
        while (true) {
          std::cin >> a;
          if (std::cin.fail() || a < minCoeffAC) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout
                << "Invalid input. Enter a positive double value grater than "
                << minCoeffAC << ":";
          } else {
            break;
          }
        }
        std::cout << "Enter coefficient a (positive double grater than: "
                  << minCoeffAC << "): ";
        while (true) {
          std::cin >> c;
          if (std::cin.fail() || c < minCoeffAC) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout
                << "Invalid input. Enter a positive double value grater than "
                << minCoeffAC << ":";
          } else {
            break;
          }
        }
        std::cout << "Enter coefficient a (positive double grater than: "
                  << minCoeffS << ".): ";
        while (true) {
          std::cin >> s;
          if (std::cin.fail() || s < minCoeffS) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout
                << "Invalid input. Enter a positive double value grater than "
                << minCoeffS << ".:";
          } else {
            break;
          }
        }
        std::cout << "Enter the separation radius:";
        while (true) {
          std::cin >> radTooClose;
          if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a double value:";
          } else {
            break;
          }
        }
        std::cout << "Enter the vision radius:";
        while (true) {
          std::cin >> radOfVision;
          if (std::cin.fail() || radOfVision < radTooClose) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a double value:";
          } else {
            break;
          }
        }
        break;
      } else if (choice == 'n') {
        // Default values if the coefficients are not changed
        a           = 1.;
        c           = 2.;
        s           = 7.;
        radOfVision = 300.;
        radTooClose = 30.;
        break;
      } else {
        std::cout << "Invalid choice. Please enter 'y' or 'n'.\n";
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

    Flock f{flock, a, c, s, radOfVision, radTooClose};

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
      std::cerr << "Unable to load texture '../images/Boid.png'" << std::endl;
      window.close();
      return EXIT_FAILURE;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.017f, 0.017f);

    sf::Texture texture1;
    if (!texture1.loadFromFile("../images/cielo.jpg")) {
      throw std::runtime_error("Unable to load texture '../images/Boid.png'");
      window.close();
      return 0;
    }
    sf::Sprite sprite1;
    sprite1.setTexture(texture1);
    sprite1.setScale(1.17f, 1.17f);

    // loop for evolving the flock and frame printing process
    window.setFramerateLimit(30);
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
      ss << "Average distance: " << statDist.first
         << "\nStandard deviation: " << statDist.second
         << "\nAverage speed: " << statSpeed.first << " px/s"
         << "\nStandard deviation: " << statSpeed.second << " px/s";
      statsText.setString(ss.str());

      window.draw(statsText);
      window.display();
    }

  } catch (const std::exception& e) {
    std::cerr << "Unrecoverable error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unexpected error." << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
