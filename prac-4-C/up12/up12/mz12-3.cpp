#include <algorithm>

struct bin {
    uint64_t C;
    bin(int m, int k) {
        C = (!m||!k||m==k) ? 1 : bin(m-1,k).C + bin(m-1,k-1).C;
    }
};

void calc_binom(int m, int k, std::function<void (uint64_t) > cb) {
    cb(bin(m, k).C);
}


#include <iostream>
int main() {
    calc_binom(3, 2, [](uint64_t z) { std::cout << z << std::endl; });
    return 0;
}
