#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

#define unipFig std::unique_ptr<Figure>


class Factory{
private:
    Factory() {};
public:
    static Factory &factory_instance() {
        static Factory single;
        return single;
    }
    
    unipFig create_figure(std::string s) {
        char f;
        std::istringstream str(s);
        str >> f;
        std::getline(str, s);
        
        if (f == 'R') {
            return std::unique_ptr<Rectangle>(Rectangle::make(s));
        } else if (f == 'S') {
            return std::unique_ptr<Square>(Square::make(s));
        } else if (f == 'C') {
            return std::unique_ptr<Circle>(Circle::make(s));
        } else return nullptr;
    }
};


bool comparator(const unipFig &f, const unipFig &g) {
    bool tc = (f->get_square() < g->get_square());
    return tc;
}

int main(void) {
    std::string input;
    std::vector<unipFig> fig_v;
    Factory factory = Factory::factory_instance();
    while (std::getline(std::cin, input)) {
        fig_v.push_back(factory.create_figure(input));
    }
    
    std::stable_sort(fig_v.begin(), fig_v.end(), comparator);
    for (auto &fig: fig_v) {
        std::cout << fig->to_string();
        std::cout << std::endl;
    }
}
