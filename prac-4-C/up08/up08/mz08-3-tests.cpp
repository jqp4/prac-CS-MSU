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
    T dist_(Coord<T> size, Coord<T> c1, Coord<T> c2) {
        T dc2 = (c1.col + 1) / 2 - (c2.col + 1) / 2;
        T dca = tabs(c1.col - c2.col);
        T dr = c1.row - c2.row;
        T tmp1 = tabs(dr - dc2);
        T tmp2 = tabs(dr + dc2);
        return std::max(std::max(tmp1, tmp2), dca);
    }

    template <typename T>
    T dist(Coord<T> size, Coord<T> c1, Coord<T> c2) {
        T dca = tabs(c1.col - c2.col);
        T dra = tabs(c1.row - c2.row);
        //std:: cout << "<" << dca << " " << dra << ">";
        return std::max(dra + dca / 2, dca);
    }

    template <typename T>
    T dist_old(Coord<T> size, Coord<T> c1, Coord<T> c2) {
        T rr1 = c1.row - (c1.col + 1) / 2;
        T rr2 = c2.row - (c2.col + 1) / 2;
        T h1 = -(rr1 + c1.col);
        T h2 = -(rr2 + c2.col);
        T max2 = std::max(tabs(rr1 - rr2), tabs(c1.col - c2.col));
        return std::max(max2, tabs(h1 - h2));
    }
};




#define r100 (rand() % 100)
int main_() {
    srand((unsigned)time(NULL));
    Game::Coord<int> size(10, 10);
    
    for (int i = 100; i--;) {
        Game::Coord<int> c1(r100, r100);
        Game::Coord<int> c2(r100, r100);
        
        int dold = dist_old(size, c1, c2);
        int dnew = dist_(size, c1, c2);
        
        if (dold != dnew) {
            std::cout << "old = " << dold << "; ";
            std::cout << "new = " << dnew << std::endl;
        }
    }
    
    /*Game::Coord<int> c1(75, 72);
    Game::Coord<int> c2(30, 57);
    int dold = dist_old(size, c1, c2);
    int dnew = dist_(size, c1, c2);
    std::cout << "old = " << dold << "; ";
    std::cout << "new = " << dnew << std::endl;*/
    
    return 0;
}










