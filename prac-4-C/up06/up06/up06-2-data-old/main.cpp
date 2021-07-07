#include <iostream>
#include <algorithm>
#include <vector>
#define lli long long int
#include <fstream>
#include <cstdlib>
#include <ctime>
std::string root = "/Users/gleb/Projects/prac/up06/up06/up062data/";
std::string fns_out[2] = {root + "gout.txt", root + "kout.txt"};
std::string fn_in = root + "_in.txt";
std::ofstream fout;
std::ifstream fin;
enum
{
    MOD = 4294967161
};
#include <map>
typedef std::pair<uint64_t,uint64_t> my_key_type;
typedef std::map<my_key_type,long long> my_map_type;
 








int main_k(){
    my_map_type a, b;
    uint64_t r,c = 0;
    long v = 0;
    while (std::cin>>r>>c>>v) {
        if (r == 0 && c == 0 && v == 4294967161) break;
        a.insert(std::make_pair(my_key_type(r,c), v));
    }
    while (std::cin>>r>>c>>v) {
        if (r == 0 && c == 0 && v == 4294967161) {
            continue;
        } else {
            if (a.find(std::make_pair(r,c)) == a.end()){
                a.insert(std::make_pair(my_key_type(r,c), v));
            } else {
                a[my_key_type(r,c)]  = (a[my_key_type(r,c)] + v) % 4294967161;
            }
        }
    }
    for (auto it = a.begin(); it != (a.end()); ++it){
        auto el = *it;
        if (el.second != 0) {
            std::cout << el.first.first << " " << el.first.second << " " << el.second << "\n";
        }
    }
    return 0;
}


int main_g() {
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








int main() {
    fout.open(fn_in);
    for (int i = 20; i--;) {
        int n = rand() % 5;
        int h1 = rand() % 100;
        int h2 = rand() % 100;
        
        fout << n << std::endl;
    }
    
    
    
    fout.close();
    
    
    for (int i = 2; i--;) {
        fin.open(fn_in);
        fout.open(fns_out[i]);
        //fout[i] << "Writing this to a file.\n" << i;
        //main_g();
        fout.close();
        fin.close();
    }
    return 0;
}
