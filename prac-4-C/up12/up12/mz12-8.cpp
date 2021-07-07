#include <algorithm>
#include <string>

std::string gen_power(uint32_t power) {
    if (power) {
        uint32_t i;
        std::string s;
        for (i = 0; i < power; ++i) s.push_back('x');
        for (i = 1; i < power; ++i) s.push_back('*');
        return s;
    } else {
        return "1";
    }
}


#include <iostream>
int main() {
    std::cout << gen_power(4) << std::endl;
    return 0;
}
