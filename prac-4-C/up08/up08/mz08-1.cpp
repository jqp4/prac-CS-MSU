#include <iostream>
#include <cmath>

int get_d(int *R, int i) {
    int A = abs(R[1] - R[0]);
    return std::min(A, abs(A - i));
}

int main() {
    int m, n;
    std::cin >> m >> n;
    int R[2], C[2];
    while (std::cin>>R[0]>>C[0]>>R[1]>>C[1]) {
        int drc = get_d(R, m) + get_d(C, n);
        std::cout << drc << std::endl;
    }
    return 0;
}
