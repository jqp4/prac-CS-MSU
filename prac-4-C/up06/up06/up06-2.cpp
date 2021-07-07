#include <iostream>
#include <algorithm>
#include <map>

#define mpair std::pair<uint64_t, uint64_t>
enum {
    MOD = 4294967161
};

static void output(std::map<mpair, long long> &dubM) {
    for (auto it = dubM.begin(); it != dubM.end(); it++) {
        if (it->second) {
            std::cout << it->first.first << " ";
            std::cout << it->first.second << " ";
            std::cout << it->second << std::endl;
        }
    }
}

int main() {
    long long V = 0;
    uint64_t R, C = 0;
    std::map<mpair, long long> dubM;
    while (std::cin >> R >> C >> V) {
        if (R || C || V != MOD) {
            if (dubM.find(std::make_pair(R, C)) == dubM.end()) {
                dubM.insert(std::make_pair(mpair(R, C), V));
            } else {
                dubM[mpair(R, C)] = (dubM[mpair(R, C)] + V) % MOD;
            }
        }
    }
    output(dubM);
}
