#include <iostream>
#include <vector>

static void printv(std::vector<int64_t> &v) {
    //std::cout << v.size() << " : ";
    /*for (int i = 0; i < v.size(); i++){
        std::cout << v[i] << " ";
    }*/
    /*for (auto const &elem: v){
        std::cout << elem << " ";
    }*/
    std::cout << "{ ";
    std::vector<int64_t>::const_iterator it = v.begin();
    while (it != v.end()) {
        std::cout << *it << " ";
        it++;
    }
    std::cout << "}" << std::endl;
}


void process(std::vector<int64_t> &v, int64_t h){
    if (v.size() > 0) {
        size_t n = v.capacity();
        for (auto &i: v) {
            n += (i >= h);
        }
        v.reserve(n);
        
        std::vector<int64_t>::iterator it = v.end() - 1;
        
        while (it >= v.begin()) {
            if (*it >= h) {
                v.push_back(*it);
            }
            it--;
        }
    }
}


int main(int argc, const char * argv[]) {
    std::vector<int64_t> v1 = {1, 4, 3, 2};
    int64_t h = 3;
    
    process(v1, h);
    printv(v1);
    return 0;
}
