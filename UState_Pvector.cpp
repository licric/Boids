#include <UState_Pvector.hpp>
#include <cmath>


Vec2D Vec2D::operator-(Vec2D const& B2){
     return Vec2D{x_- B2.x_, y_ - B2.y_};
}

double Vec2D::angle() const{
        return std::atan2(y_, x_);
}

