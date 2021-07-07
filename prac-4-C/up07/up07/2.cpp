#include <sstream>
#include <iostream>
#include <string>
#include <cmath>


class Rectangle: public Figure {
public:
    double _sideA, _sideB;
    double get_square() const {
        return _sideA * _sideB;
    }
    
    static Rectangle * make (std::string s) {
        std::istringstream str(s);
        double a, b;
        str >> a >> b;
        Rectangle *rec = new Rectangle;
        rec->_sideA = a;
        rec->_sideB = b;
        return rec;
    }
};


class Square: public Figure {
public:
    double _side;
    double get_square() const {
        return _side * _side;
    }
    
    static Square * make (std::string s) {
        std::istringstream str(s);
        double x;
        str >> x;
        Square *sq = new Square;
        sq->_side = x;
        return sq;
    }
};


class Circle: public Figure {
public:
    double _R;
    double get_square() const {
        return M_PI * _R * _R;
    }
    
    static Circle * make (std::string s) {
        std::istringstream str(s);
        double r;
        str >> r;
        Circle *tmp = new Circle;
        tmp->_R = r;
        return tmp;
    }
};
