#include <iostream>

class S {
    int value;
    bool flag;
    bool isempty;
    
public:
    S(): value(0), isempty(true) {
        flag = bool(std::cin >> value);
    }
    
    S(S &&_S): value(0), isempty(false) {
        flag = bool(std::cin >> value);
        value += _S.value;
        _S.isempty = true;
    }
    
    operator bool() const {
        return flag;
    }
    
    ~S() {
        if (!isempty) {
            std::cout << value << std::endl;
        }
    }
};
