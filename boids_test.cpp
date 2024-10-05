// test.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Vec2D.hpp"
#include "Boid.hpp"
#include "Flock.hpp"

// Test cases for Vec2D
TEST_CASE("Vec2D operations") {
    Vec2D v1(3.0, 4.0);
    CHECK(v1.getX() == 3.0);
    CHECK(v1.getY() == 4.0);
    CHECK(v1.magnitude() == doctest::Approx(5.0));

    Vec2D v2(1.0, 2.0);
    Vec2D v3 = v1 + v2;
    CHECK(v3.getX() == 4.0);
    CHECK(v3.getY() == 6.0);
}

// Test cases for Boid
TEST_CASE("Boid visibility and proximity") {
    Boid b1;
    b1.pos = Vec2D(0.0, 0.0);
    b1.vel = Vec2D(1.0, 0.0);

    Boid b2;
    b2.pos = Vec2D(100.0, 0.0);

    CHECK(b1.boidCanSee(b2, 90.0, 200.0) == true);
    CHECK(b1.isTooClose(b2, 50.0) == false);
}

// Test cases for Flock
TEST_CASE("Flock evolution") {
    std::vector<Boid> flock;
    Boid b1;
    b1.pos = Vec2D(0.0, 0.0);
    b1.vel = Vec2D(1.0, 0.0);
    b1.N = 0;
    Boid b2;
    b2.pos = Vec2D(100.0, 0.0);
    b2.vel = Vec2D(1.0, 0.0);
    b2.N = 1;
    flock.push_back(b1);
    flock.push_back(b2);

    Flock f(flock, 1.0, 1.0, 1.0);
    f.evolve(1.0, 800, 600);

    // Check positions have updated
    CHECK(f.flock_[0].pos.getX() != 0.0);
}
