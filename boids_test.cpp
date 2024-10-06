#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Boid.hpp"
#include "Flock.hpp"
#include "Vec2D.hpp"
#include "doctest.h"

TEST_CASE("Test cases for Vec2D")
{
  SUBCASE("Magnitude e Getters e operatore +=")
  {
    Vec2D v1(3.0, 4.0);
    Vec2D v2(1.0, 2.0);
    CHECK(v1.getX() == 3.0);
    CHECK(v1.getY() == 4.0);
    CHECK(v1.magnitude() == doctest::Approx(5.));
    v1 += v2;
    CHECK(v1.getX() == 4.0);
    CHECK(v1.getY() == 6.0);
    CHECK(v2.getX() == 1.0); // l'op non cambia v2
    CHECK(v2.getY() == 2.0);
    CHECK(v1.magnitude() == doctest::Approx(7.2111025509));
  }
  SUBCASE("Rotation by Non-standard Angles")
  {
    Vec2D v(1.0, 0.0);
    v.rotate(45.0);
    CHECK(doctest::Approx(v.getX()).epsilon(0.001) == 0.7071067812);
    CHECK(doctest::Approx(v.getY()).epsilon(0.001) == 0.7071067812);
  }
  SUBCASE("Exception Messages")
  {
    Vec2D v1(1.0, 1.0);
    Vec2D v2(1.0, 1.0);
    try {
      v1 /= 0.0;
    } catch (const std::runtime_error& e) {
      CHECK(std::string(e.what()) == "Divisione per zero in Vec2D::operator/.");
    }
    v2 *= 0.; // controllo anche *=
    CHECK(v2.getX() == 0.0);
    CHECK(v2.getY() == 0.0);
    try {
      v2.magnitude();
    } catch (const std::runtime_error& e) {
      CHECK(std::string(e.what()) == "Magnitude is zero");
    }
  }
  SUBCASE("Angle Between Vectors and Dot Product")
  {
    Vec2D v1(1.0, 0.0);
    Vec2D v2(0.0, 1.0);
    CHECK(doctest::Approx(v1.angleBetween(v2)) == 90.0);
    CHECK(v1.dotProduct(v2) == 0.);
  }
  SUBCASE("+= operation does not modify second operand")
  {
    Vec2D v1(1.0, 2.0);
    Vec2D v2(3.0, 4.0);
    v2 += v1;
    CHECK(v1.getX() == 1.0);
    CHECK(v1.getY() == 2.0);
  }
}

TEST_CASE("Boid.hpp")
{
  SUBCASE(" boidCanSee() & isTooClose() ") {
      Boid b1;
      b1.pos = Vec2D(0., 0.);
      b1.vel = Vec2D(1., 0.); 
      Boid b2;
      b2.pos = Vec2D(100., 100.);
      Boid b3;
      b3.pos = Vec2D(100., -100.);
      // can see b2 within 60 degrees and 200 units
      CHECK(b1.boidCanSee(b2, 60., 200.) == true);
      CHECK(b1.boidCanSee(b3, 60., 200.) == true);
      // cannot see b2 and b3 if angleOfVision < 45.
      CHECK(b1.boidCanSee(b2, 30., 200.) == false);
      CHECK(b1.boidCanSee(b3, 30., 200.) == false); 
      // cannot see b2 if outside radius of vision
      CHECK(b1.boidCanSee(b2, 70., 50.) == false);
      b2.pos = Vec2D(-100., 0.);
      CHECK(b1.boidCanSee(b2, 70., 200.) == false);
      // b2 is too close
      b2.pos = Vec2D(20., 0.);
      CHECK(b1.isTooClose(b2, 45.) == true);
      // check throws
      try {
        b1.isTooClose(b2, -45.);
      } catch (const std::runtime_error& e) {
        CHECK(std::string(e.what()) == "radius tooClose has to be positive");
      }
      try {
        b1.boidCanSee(b2, -70. , -400.);
      } catch (const std::runtime_error& e) {
        CHECK(std::string(e.what()) == "angle & radius of vision have to be positive");
      }
  }
  SUBCASE("Limit Velocity Max Min") {
      Boid b;
      b.vel = Vec2D(1000., 1000.);

      b.limitVelMaxMin();
      CHECK(b.vel.magnitude() == doctest::Approx(maxVel));
      // limit with negative velocities
      b.vel = Vec2D(-300.0, -300.0);
      b.limitVelMaxMin();
      CHECK(b.vel.magnitude() == doctest::Approx(maxVel));
      b.vel = Vec2D(0.5, 0.5);
      b.limitVelMaxMin();
      CHECK(b.vel.magnitude() == doctest::Approx(minVel));
  }
  SUBCASE("Natural Veer") {
      Boid b;
      b.vel = Vec2D(100., 0.); 
      // The angleBetween is 90 degrees
      Vec2D sumCorr(0.0, 1.0); 
      b.naturalVeer(sumCorr);
      double theta = b.vel.angleBetween(sumCorr);
      CHECK(theta >= 3.0);
      CHECK(theta <= 177.0);    
       // The angleBetween is 180 degrees 
      sumCorr = Vec2D(-1.0, 0.0); 
      b.naturalVeer(sumCorr);
      theta = b.vel.angleBetween(sumCorr);
      CHECK(theta >= 3.0);
      CHECK(theta <= 177.0);
      // check throws
      sumCorr = Vec2D(0., 0.); 
      try {
      b.naturalVeer(sumCorr);
      } catch (const std::runtime_error& e) {
        // magnitude throw should come out
        CHECK(std::string(e.what()) == "Magnitude is zero");
      }
    }
}

TEST_CASE("Flock evolution")
{
  SUBCASE("Testing separation") // 45°
  {
    std::vector<Boid> flock;
    Boid b1, b2;

    b1.pos = Vec2D(100.0, 100.0);
    b1.vel = Vec2D(1.0, 0.0);
    b1.N   = 0;
    b2.pos = Vec2D(150.0, 50.0);
    b2.vel = Vec2D(0.0, 1.0);
    b2.N   = 1;

    flock.push_back(b1);
    flock.push_back(b2);

    Flock f(flock, 1.7, 3.0, 6.0);
    f.evolve(1.0, 1920, 1080);

    // Boids should have adjusted their velocities towards each other
    CHECK(f.flock_[0].vel.getY() > 0.0);
    CHECK(f.flock_[1].vel.getX() > 0.0);
  }
  SUBCASE("Testing alignment, with no coesion")
  {
    std::vector<Boid> flock;
    Boid b1, b2, b3;

    b1.pos = Vec2D(100.0, 100.0);
    b1.vel = Vec2D(1.0, 1.0);
    b1.N   = 0;
    b2.pos = Vec2D(100.0, 200.0);
    b2.vel = Vec2D(1.0, 1.0);
    b2.N   = 1;
    b3.pos = Vec2D(0.0, 150.0);
    b3.vel = Vec2D(1.0, 0.0);
    b3.N   = 2;

    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);

    Flock f(flock, 1.7, 3.0, 6.0);
    f.evolve(1.0, 1920, 1080);

    // Boid N=2 should change his Y velocity
    CHECK(f.flock_[2].vel.getY() > 0.0);
  }
  SUBCASE("Testing coesion")
  {
    std::vector<Boid> flock;
    Boid b1, b2, b3;

    b1.pos = Vec2D(100.0, 100.0);
    b1.vel = Vec2D(1.0, 0.0);
    b1.N   = 0;
    b2.pos = Vec2D(200.0, 200.0);
    b2.vel = Vec2D(1.0, 0.0);
    b2.N   = 1;
    b3.pos = Vec2D(200.0, 210.0);
    b3.vel = Vec2D(1.0, 0.0);
    b3.N   = 2;

    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);

    Flock f(flock, 1.7, 3.0, 6.0);
    f.evolve(1.0, 1920, 1080);

    // Boid N=0 should change his Y velocity
    CHECK(f.flock_[0].vel.getY() > 0.0);
  }
}
