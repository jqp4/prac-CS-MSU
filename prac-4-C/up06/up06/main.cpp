#include <iostream>
#include <algorithm>
#include <vector>

double f(long L, long V) {
    return -1 + 2 * (double)L/V;
}

int main(int argc, const char * argv[]) {
    long W1, W2, x;
    std::vector<long> k;
    std::cin >> W1 >> W2;
    while (std::cin >> x) {
        k.push_back(x);
    }
    long L_real = 0;
    long V = k.size() * 32;
    for (long &x: k) {
        for (int i = 0; i < 32; i++) {
            if (x & 1) L_real++;
            x >>= 1;
        }
    }
    long L_max = L_real + W1;
    L_max = L_max > V ? V : L_max;
    long L_min = L_real - W2;
    L_min = L_min < 0 ? 0 : L_min;
    std::cout << f(L_min, V) << " " << f(L_max, V) << std::endl;
    return 0;
}
