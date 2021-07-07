#include <iostream>
#include <vector>

template <typename T>
auto process(const T &_it) {
    auto it = _it.rbegin();
    auto sum = *it;
    sum = {};
    
    for (int i = 0; i < 5 && it != _it.rend(); i++){
        if (i == 0 || i == 2 || i == 4) {
            sum += *it;
        }
        it++;
    }
    return sum;
}


int main(int argc, const char * argv[]) {
    std::vector<int> v =  {4, 1, 2, 1};
    std::cout << "> " << process(v) << "\n";
    return 0;
}
