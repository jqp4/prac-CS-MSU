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





    template <typename T>
    std::pair<bool, std::vector<std::complex<T>>>
    quadratic1(std::array<std::complex<T>, 3> arr) {
        std::complex<T> nullnum {0.0, 0.0};
        std::complex<T> a{arr[2]}, b{arr[1]}, c{arr[0]};
        if (a == nullnum && b == nullnum && c == nullnum) {
            return {false, {}};
        } else if (a == nullnum && b == nullnum) {
            return {true, {}};
        } else if (a == nullnum) {
            return {true, { -c / b }};
        } else {
            std::complex<T> discr = b * b - std::complex<T>{4.0, 0.0} * a * c;
            std::vector<std::complex<T>> vec = {
                (-b + sqrt(discr)) / (std::complex<T>{2.0, 0.0} * a),
                (-b - sqrt(discr)) / (std::complex<T>{2.0, 0.0} * a)
            };
            return {true, vec};
        }
    }
};






static void out(const std::pair<bool, std::vector<std::complex<double>, std::allocator<std::complex<double>>>> &res) {
    std::cout << res.first << ", { ";
    for (auto &x: res.second) {
        std::cout << x << " ";
    }
    std::cout << "}\n";
}

int main()
{
    std::array<std::complex<double>, 3> v1{
        std::complex<double>(14.10, 17.033),
        std::complex<double>(21.73, 9.41),
        std::complex<double>(3.9, 1.34)
    };

    out(Equations::quadratic(v1));
    out(Equations::quadratic1(v1));
}
