#include <iostream>
#include <cmath>

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
T dist (Coord<T> size, Coord<T> c1, Coord<T> c2) {
    auto lr = c2.row - c1.row;
    auto lc = c2.col - c1.col;
    T sr = std::min(tabs(lr - size.row), tabs(lr + size.row));
    T sc = std::min(tabs(lc - size.col), tabs(lc + size.col));
    T dr = std::min(tabs(lr), sr);
    T dc = std::min(tabs(lc), sc);
    return tabs(dr - dc) + std::min(dr, dc);
}
