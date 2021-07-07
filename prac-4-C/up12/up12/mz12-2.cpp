#include <algorithm>

namespace Sample{
    template<class T, const size_t n, const size_t m>
    class Matrix {
        class Line {
            T line[m];
        public:
            T& operator[](size_t index) {
                return line[index];
            }
            const T& operator[](size_t index) const {
                return line[index];
            }
        };
        Line lines[n];
    public:
        Line &operator[](size_t index) {
            return lines[index];
        }
        const Line &operator[](size_t index) const {
            return lines[index];
        }
    };
}






#include <iostream>
void mp(Sample::Matrix<int, 2, 2> m) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------\n";
}

int main() {
    Sample::Matrix<int, 2, 2> m;
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    mp(m);
    m[0][1] = m[1][0];
    mp(m);
    return 0;
}
