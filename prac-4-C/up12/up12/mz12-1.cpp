#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

void process(std::vector<uint64_t> &v, uint32_t s) {
    if (!v.empty()) {
        uint32_t n = uint32_t(v.size() / s);
        v.resize(v.size() + n);
        uint64_t sum = 0;
        for (uint64_t x: v) sum += x;
        uint64_t left = v.size() - n - 1;
        uint64_t right = v.size() - 1;
        
        while (right >= s && left >= 0) {
            if ((left + 1) % s) {
                v[right] = v[left];
            } else {
                v[right] = sum;
                v[--right] = v[left];
            }
            sum -= v[left--];
            right--;
        }
    }
}


void vp(std::vector<uint64_t> v) {
    for (uint64_t x: v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}


int main(void) {
    std::vector<uint64_t> v = {2, 3, 4, 5, 6};
    uint32_t s = 2;
    vp(v);
    process(v, s);
    vp(v);
    return 0;
}
