#include <iostream>
#include <vector>

class Pred {
public:
    bool operator()(const int x) const {
        return x % 2 == 0;
    }
};


template <typename T, typename P>
T myfilter(T &sc, P pred) {
    T newsc;
    auto it = sc.begin();
    auto nit = newsc.end();
    while (it != sc.end()){
        if (pred(*it)) {
            newsc.insert(nit, *it);
            nit = newsc.end();
        }
        it++;
    }
    return newsc;
}


int main(int argc, const char * argv[]) {
    std::vector<int> v =  {4, 1, 2, 1};
    std::vector<int> v1 = myfilter(v, Pred());
    for (auto x: v1) {
         std::cout << x << " ";
    }
    return 0;
}
