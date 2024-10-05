#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Boid.hpp"
#include "Flock.hpp"
#include "Vec2D.hpp"
#include "doctest.h"

// Test cases for Vec2D
TEST_CASE("Vec2D operations")
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
      CHECK(std::string(e.what()) == "Magnitude è 0");
    }
  }

  SUBCASE("Angle Between Vectors and Dot Product")
  {
    Vec2D v1(1.0, 0.0);
    Vec2D v2(0.0, 1.0);
    CHECK(doctest::Approx(v1.angleBetween(v2)) == 90.0);
    CHECK(v1.dotProduct(v2) == 0.);
  }

  SUBCASE("Arithmetic Operations Do Not Modify Operands")
  {
    Vec2D v1(1.0, 2.0);
    Vec2D v2(3.0, 4.0);
    Vec2D v3 = v1 + v2 += v1;
    CHECK(v2.getX() == 3.0);
    CHECK(v2.getY() == 4.0);
    CHECK(v3.getX() == 5.0);
    CHECK(v3.getY() == 8.0);
  }
}

// Test cases for Boid
TEST_CASE("Boid visibility and proximity")
{
  Boid b1;
  b1.pos = Vec2D(0.0, 0.0);
  b1.vel = Vec2D(1.0, 0.0);

  Boid b2;
  b2.pos = Vec2D(100.0, 0.0);

  CHECK(b1.boidCanSee(b2, 90.0, 200.0) == true);
  CHECK(b1.isTooClose(b2, 50.0) == false);
}

// Test cases for Flock
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
