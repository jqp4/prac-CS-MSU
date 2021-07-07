#include <iostream>
#include <vector>

static void printv(std::vector<uint64_t> &v) {
    //std::cout << v.size() << " : ";
    /*for (int i = 0; i < v.size(); i++){
        std::cout << v[i] << " ";
    }*/
    /*for (auto const &elem: v){
        std::cout << elem << " ";
    }*/
    std::cout << "{ ";
    std::vector<uint64_t>::const_iterator it = v.begin();
    while (it != v.end()) {
        std::cout << *it << " ";
        it++;
    }
    std::cout << "}" << std::endl;
}


void process(const std::vector<uint64_t> &v1, std::vector<uint64_t> &v2, int h){
    if (v1.size() > 0 && v2.size() > 0) {
        std::vector<uint64_t>::const_iterator it1 = v1.begin();
        std::vector<uint64_t>::iterator it2 = v2.end() - 1;
        
        while (it1 < v1.end() && it2 >= v2.begin()) {
            *it2 += *it1;
            it1 += h;
            it2--;
        }
    }
}


int main(int argc, const char * argv[]) {
    std::vector<uint64_t> v1 = {1, 2, 3, 4};
    std::vector<uint64_t> v2 = {1, 2, 3};
    int h = 2;
    
    process(v1, v2, h);
    printv(v2);
    return 0;
}
