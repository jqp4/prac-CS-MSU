#include <iostream>
#include <algorithm>
#include <vector>




template <typename It_n, typename It_x>
auto myremove(const It_n &n_beg, const It_n &n_end, const It_x &x_beg, const It_x &x_end) {
    std::vector<int> v(n_beg, n_end);
    std::sort(v.begin(), v.end());
    std::vector<int>::iterator last = std::unique(v.begin(), v.end());
    std::vector<int> num(v.begin(), last);
    
    int i = 0;
    auto next_X = x_beg;
    for (auto iter = x_beg; iter != x_end; iter++, i++) {
        bool flag1 = true;
        
        for (unsigned long xi = 0; xi < num.size() && flag1; xi++) {
            if (num[xi] == i) flag1 = false;
        }
        
        if (flag1) {
            *next_X = std::move(*iter);
            next_X++;
        }
    }
    return next_X;
}
