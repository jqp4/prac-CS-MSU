#include <iostream>
#include <complex>
#include <vector>
#include <array>

namespace Equations {
    template <typename T>
    std::pair<bool, std::vector<std::complex<T>>> quadratic(std::array<std::complex<T>, 3> v) {
        std::complex<T> a{v[2]}, b{v[1]}, c{v[0]};
        std::complex<T> n{0.0, 0.0};
        if (a == n && b == n && c == n) return {false, {}};
        if (a == n && b == n) return {true, {}};
        if (a == n) return {true, {-c / b}};
        
        std::complex<T> _2a = std::complex<T>{2.0, 0.0} * a;
        std::complex<T> dscrS = sqrt(b * b - std::complex<T>{4.0, 0.0} * a * c);
        std::vector<std::complex<T>> ans = {(-b + dscrS)/_2a, (-b - dscrS)/_2a};
        return {true, ans};
    }
};
