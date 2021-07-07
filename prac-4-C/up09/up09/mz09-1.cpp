#include <iostream>
#include <string>

class RevStr {
    std::string str;
public:
    RevStr(std::string s): str(s) {};
    ~RevStr() {
        std::cout << str << std::endl;
    }
};

void func() {
    std::string s;
    if (std::cin >> s) {
        RevStr tmp(s);
        try {
            func();
        } catch (const int x) {
            throw 2;
        }
    } else throw 2;
}

int main() {
    try {
        func();
    } catch (const int x) {}
    return 0;
}
