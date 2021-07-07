#include <iostream>
#include <algorithm>
#include <vector>

class Functor {
    double _sum;
public:
    Functor() : _sum(0.0) {}
    void operator () (double x) {
        _sum += x;
    }
    double sum() {
        return _sum;
    }
};

int main() {
    std::vector<double> v;
    double c;
    while (std::cin >> c) v.push_back(c);
    int delta = (int)v.size() / 10;
    auto itbeg = v.begin() + delta;
    auto itend = v.end() - delta;
    std::sort(itbeg, itend);
    delta = (int)(std::distance(itbeg, itend)) / 10;
    itbeg += delta;
    itend -= delta;
    int len = (int)(std::distance(itbeg, itend));
    Functor f = std::for_each(itbeg, itend, Functor());
    double ans = f.sum() / len;
    int maxlen = 10000;
    char stform[maxlen];
    snprintf(stform, maxlen, "%.10g", ans);
    std::string s(stform);
    std::cout << s << std::endl;
    return 0;
}
