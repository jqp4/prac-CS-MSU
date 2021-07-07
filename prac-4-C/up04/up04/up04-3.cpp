#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
static void printvec(std::vector<T> &vec) {
    std::cout << "{ ";
    for (auto &x: vec) {
        std::cout << x << " ";
    }
    std::cout << "}\n";
}

void process_1(const std::vector<int> &v1, std::vector<int> &v2){
    if (v1.size() > 0 && v2.size() > 0) {
        std::vector<int> v = v1;
        std::sort(v.begin(), v.end());
        auto last = std::unique(v.begin(), v.end());
        auto first = v.begin();
        while (*first < 0) first++;
        std::vector<uint64_t> num(first, last);
        for (unsigned i = 0; i < num.size(); i++){
            num[i] -= i;
        }
        
        uint64_t len = v2.size();
        for (uint64_t &xi: num) {
            if (xi < len){
                while (++xi < len) {
                    std::swap(v2[xi - 1], v2[xi]);
                }
                len--;
            }
        }
        v2.resize(len);
    }
}


void process_2(const std::vector<int> &v1, std::vector<int> &v2){
    if (v1.size() > 0 && v2.size() > 0) {
        std::vector<int> v = v1;
        uint64_t lennum = v1.size();
        int maxnum = (int)v2.size() - 1;
        for (uint64_t xi = 0; xi < lennum; xi++) {
            if (v[xi] < 0 || v[xi] > maxnum) {
                std::swap(v[xi], v[--lennum]);
            }
        }
        std::sort(v.begin(), v.begin() + lennum);
        auto last = std::unique(v.begin(), v.begin() + lennum);
        std::vector<uint64_t> num(v.begin(), last);
        printvec(num);
        if (num.size() > 0) {
            uint64_t n = 0;
            for (uint64_t i = 0; i < v2.size(); i++) {
                std::swap(v2[i - n], v2[i]);
                if (i == num[n]) {
                    n++;
                }
            }
            v2.resize(v2.size() - num.size());
        }
    }
}


void process(const std::vector<int> &v1, std::vector<int> &v2) {
    if (v1.size() > 0 && v2.size() > 0) {
        std::vector<int> v = v1;
        std::sort(v.begin(), v.end());
        std::vector<int>::iterator last = std::unique(v.begin(), v.end());
        std::vector<int> num(v.begin(), last);
        //printvec(num);
        int i = 0;
        std::vector<int>::iterator itbeg = v2.begin();
        std::vector<int>::iterator itend = v2.begin();
        std::vector<int>::iterator itnum = num.begin();
        while (i < (int)v2.size() && itnum < num.end()) {
            if (*itnum < 0) itnum++;
            else {
                if (i++ == *itnum) itnum++;
                else {
                    std::swap(*itbeg, *itend);
                    itbeg++;
                }
                itend++;
            }
        }
        v2.erase(itbeg, itend);
    }
}


int main(int argc, const char * argv[]) {
    //std::vector<int> v1 = {1, 1, -9, 6, 3, 1, 3, -1, 5, 2, 4, 0, 5, 3};
    //std::vector<int> v2 = {1, 2, 5, 3, 7, 3, 8, 7, 6, 4, 5, 3, 6, 3};
    std::vector<int> v1 = {1, -21, 7, 15, 14};
    std::vector<int> v2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    printvec(v2);
    process(v1, v2);
    printvec(v2);
    return 0;
}
