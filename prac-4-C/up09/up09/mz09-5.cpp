#include <boost/date_time/gregorian/gregorian.hpp>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

int main() {
    int y, m, d;
    uint64_t res = 0;
    std::cin >> y >> m >> d;
    boost::gregorian::date oldday(y, -m, -d);
    
    while (std::cin >> y >> m >> d) {
        boost::gregorian::date newday(y, -m, -d);
        res += abs((newday - oldday).days());
        oldday = newday;
    }
    
    std::cout << res << std::endl;
}
