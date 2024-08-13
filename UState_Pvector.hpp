#ifndef PVECTOR_HPP
#define PVECTOR_HPP



class Vec2D {   //ho scelto class perchè non è un tipo banale, anche se non necessita di attributi 
                //privati ne di incapsulamento
public:
    double x_{};
    double y_{};

    //costruttore
    Vec2D(double x, double y) : x_{x}, y_{y} {}  

    //operatori
    Vec2D operator+(Vec2D const& vec2);
    Vec2D operator-(Vec2D const& vec2);
    Vec2D& operator+=(Vec2D const& vec2);


    double angle() const;

};

#endif