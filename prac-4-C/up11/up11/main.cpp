#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <queue>

enum
{
    N = 26,
    SHIFT = 'A',
};


int main() {
    bool flags[N]{};
    std::vector<std::vector<int>> sn(N);
    std::vector<std::pair<char, std::string>> mem;
    
    char left;
    std::string right;
    
    while (std::cin >> left >> right) {
        mem.emplace_back(std::make_pair(left, right));
        int tmp = (int)(mem.size() - 1);
        sn[left - SHIFT].push_back(tmp);
    }
    std::queue<char> st({'S'});

    while (!st.empty()) {
        char c = st.front();
        flags[c - SHIFT] = true;
        
        st.pop();
        for (auto n: sn[c - SHIFT])
            for (auto t: mem[n].second)
                if (!flags[t - SHIFT])
                    if (t >= 'A' && t <= 'Z') st.push(t);
    }

    for (auto &v: mem) {
        if (flags[v.first - SHIFT]) {
            std::cout << v.first << " ";
            std::cout << v.second << std::endl;
        }
    }
}
