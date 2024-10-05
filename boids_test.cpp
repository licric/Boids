// test.cpp
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
    CHECK(v1.magnitude() == doctest::Approx(7.211));
  }

  SUBCASE("Rotation by Non-standard Angles")
  {
    Vec2D v(1.0, 0.0);
    v.rotate(45.0);
    CHECK(doctest::Approx(v.getX()).epsilon(0.001) == 0.7071);
    CHECK(doctest::Approx(v.getY()).epsilon(0.001) == 0.7071);
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
        b2.pos = Vec2D(100., -100.);
        // can see b2 within 60 degrees and 200 units
        CHECK(b1.boidCanSee(b2, 60., 200.) == true);
        CHECK(b1.boidCanSee(b3, 60., 200.) == true);
        // cannot see b2 and b3 if angleOfVision < 45.
        CHECK(b1.boidCanSee(b2, 30., 200.) == false);
        CHECK(b1.boidCanSee(b3, 30., 200.) == false); 
        // cannot see b2 if outside radius of vision
        CHECK(b1.boidCanSee(b2, 70., 400.) == false);
        b2.pos = Vec2D(-100., 0.);
        CHECK(b1.boidCanSee(b2, 70., 200.) == false);
        // b2 is too close
        b2.pos = Vec2D(20., 0.);
        CHECK(b1.isTooClose(b2, 45.) == true);
    }

    SUBCASE("Limit Velocity Max Min") {
        Boid b;
        b.vel = Vec2D(1000., 1000.);

        b.limitVelMaxMin(5.0, 2.0);
        CHECK(b.vel.magnitude() == doctest::Approx(maxVel));
        // limit with negative velocities
        b.vel = Vec2D(-300.0, -300.0);
        CHECK(b.vel.magnitude() == doctest::Approx(minVel));
        b.vel = Vec2D(0.5, 0.5);
        CHECK(b.vel.magnitude() == doctest::Approx(minVel));
    }
    SUBCASE("Natural Veer") {
        Boid b;
        b.vel = Vec2D(1.0, 0.0); // Moving along positive X-axis
        Vec2D sumCorr(0.0, 1.0); // Correction vector pointing up

        // The angle between vel and sumCorr is 90 degrees
        b.naturalVeer(sumCorr);
        double theta = b.vel.angleBetween(sumCorr);

        // After naturalVeer, the angle between vel and sumCorr should be limited
        CHECK(theta <= 3.0 || theta >= 177.0);

        // Test with sumCorr opposite to vel
        sumCorr = Vec2D(-1.0, 0.0); // Opposite direction
        b.naturalVeer(sumCorr);
        theta = b.vel.angleBetween(sumCorr);
        CHECK(theta <= 3.0 || theta >= 177.0);
    }
    SUBCASE("Check throws Boid.cpp") {
        Boid b;
        b.vel = Vec2D(1.0, 0.0);
        Vec2D sumCorr(0.0, 0.0);

        CHECK_NOTHROW(b.naturalVeer(sumCorr));
        //other
    }

    // Test naturalVeer when theta is exactly 0 degrees
    SUBCASE("Natural Veer with Theta = 0") {
        Boid b;
        b.vel = Vec2D(1.0, 0.0);
        Vec2D sumCorr = Vec2D(1.0, 0.0);

        // Angle between vel and sumCorr is 0 degrees
        b.naturalVeer(sumCorr);

        // sumCorr should remain the same
        CHECK(sumCorr.getX() == 1.0);
        CHECK(sumCorr.getY() == 0.0);
    }

    // Test naturalVeer when theta is exactly 180 degrees
    SUBCASE("Natural Veer with Theta = 180") {
        Boid b;
        b.vel = Vec2D(1.0, 0.0);
        Vec2D sumCorr = Vec2D(-1.0, 0.0);

        // Angle between vel and sumCorr is 180 degrees
        b.naturalVeer(sumCorr);

        // sumCorr should be adjusted
        CHECK(sumCorr.getX() != -1.0); // Should have been rotated
    }
}

TEST_CASE("Flock evolution")
{
  std::vector<Boid> flock;
  Boid b1;
  b1.pos = Vec2D(0.0, 0.0);
  b1.vel = Vec2D(1.0, 0.0);
  b1.N   = 0;
  Boid b2;
  b2.pos = Vec2D(100.0, 0.0);
  b2.vel = Vec2D(1.0, 0.0);
  b2.N   = 1;
  flock.push_back(b1);
  flock.push_back(b2);

  Flock f(flock, 1.0, 1.0, 1.0);
  f.evolve(1.0, 800, 600);

  // Check positions have updated
  CHECK(f.flock_[0].pos.getX() != 0.0);
}
