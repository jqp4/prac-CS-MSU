#include <cmath>
#include <iostream>
#include <iomanip>

enum
{
    FORMAT = 10
};

int main() {
    double sum = 0;
    double sum2 = 0;
    long long n = 0;
    double x;
    
    //while (scanf("%lf", &x) != EOF) {
    while (std::cin >> x) {
        n++;
        sum += x;
        sum2 += x * x;
    }
    
    double MS = sum / n;
    double MSE = sqrt(sum2 / n - MS * MS);
    std::cout << std::fixed;
    std::cout << std::setprecision(FORMAT) << MS << "\n";
    std::cout << std::setprecision(FORMAT) << MSE << "\n";
}
