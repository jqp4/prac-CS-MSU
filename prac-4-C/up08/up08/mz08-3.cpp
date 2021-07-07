#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace Game {
    template <typename T>
    class Coord {
    public:
        typedef T value_type;
        T row, col;
        Coord (T _row = 0, T _col = 0) : row {_row}, col {_col} {}
    };

    template <typename T>
    T tabs(T a) {
        return a < 0 ? -a : a;
    }

    template <typename T>
    T dist(Coord<T> size, Coord<T> c1, Coord<T> c2) {
        T tmp1 = c1.row - c2.row - (c1.col + 1) / 2 + (c2.col + 1) / 2;
        T tmp2 = -tmp1 - c1.col + c2.col;
        T dca = tabs(c1.col - c2.col);
        return std::max(std::max(tabs(tmp1), dca), tabs(tmp2));
    }
};




