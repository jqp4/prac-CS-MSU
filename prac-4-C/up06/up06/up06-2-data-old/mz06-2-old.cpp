#include <iostream>
#include <algorithm>
#include <vector>
#define lli long long int

enum
{
    MOD = 4294967161
};

int main(int argc, const char * argv[]) {
    std::vector<std::vector<lli>> m[2];
    unsigned long R, C;
    int i = 0;
    lli V;
    
    while (std::cin >> R >> C >> V) {
        if (R == 0 && C == 0 && V == MOD) {
            i++;
        } else {
            if (R >= m[0].size()) {
                int j0 = (int)m[0].size();
                int jm = (int)R + 1;
                m[0].resize(R + 1);
                m[1].resize(R + 1);
                int l = (int)m[0][0].size();
                for (int j = j0; j < jm; j++) {
                    m[0][j].resize(l);
                    m[1][j].resize(l);
                }
            }
            if (C >= m[0][0].size()) {
                int jm = (int)m[0].size();
                for (int j = 0; j < jm; j++) {
                    m[0][j].resize(C + 1);
                    m[1][j].resize(C + 1);
                }
            }
            m[i][R][C] = V;
        }
    }
    
    int rc = (int)m[0].size();
    int cc = (int)m[0][0].size();
    
    for (int r = 0; r < rc; r++) {
        for (int c = 0; c < cc; c++) {
            lli v = (m[0][r][c] + m[1][r][c]) % MOD;
            if (v != 0) {
                std::cout << r << " " << c << " " <<  v << std::endl;
            }
        }
    }
    return 0;
}
