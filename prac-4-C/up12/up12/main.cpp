#include <iostream>

void A();
void B();
void S();

void S() {
    char xc;
    std::cin >> xc;
    if (xc == 'a') {
        A();
        std::cin >> xc;
        if (xc != 'b') throw 1;
    } else if (xc == 'c') B();
    else throw 1;
}
void A() {
    char xc;
    std::cin >> xc;
    if (xc == 'c') {
        A();
        std::cin >> xc;
        if (xc != 'd') throw 1;
    } else if (xc != 'e') throw 1;
}
void B() {
    char xc;
    if (std::cin >> xc) {
        if (xc == 'b') B();
        else if (xc != 'd') throw 1;
    }
}

int main(void) {
    try {
        S();
        std::cout << "1\n";
    } catch (...) {
        std::cout << "0\n";
    }
}


