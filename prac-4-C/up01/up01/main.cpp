#include <iostream>
#include <iomanip>

#define get(c) ((c = getchar()) != EOF)

int main1(){
    char c;
    
    while (get(c)) {
        
        if (c >= '0' && c <= '9') {
            
            if (c == '0') {
                while (get(c) && c == '0') {}
            }
            
            if (c >= '0' && c <= '9') {
                std::cout << c;
                while (get(c) && c >= '0' && c <= '9') {
                    std::cout << c;
                }
                std::cout << c;
                
                
                
            } else std::cout << '0' << c;
        } else std::cout << c;
    }
    return 0;
}



int main()
{
    char c;
    std::cin >> std::noskipws;
    
    while (std::cin >> c) {
        if (c >= '0' && c <= '9') {
            if (c == '0') {
                while (std::cin >> c && c == '0') {}
            }
            if (c >= '0' && c <= '9') {
                while (std::cin >> c && c >= '0' && c <= '9') {
                    //tmp.push_back(c);
                }
                //tmp.erase(0, tmp.find_first_not_of('0'));
                if (tmp.length() == 0) {
                    std::cout << '0';
                } else {
                    //std::cout << tmp;
                }
                
                if (c <'0' || c >'9') {
                    std::cout << c;
                }
                
            }
        } else {
            std::cout << c;
        }
    }
}
