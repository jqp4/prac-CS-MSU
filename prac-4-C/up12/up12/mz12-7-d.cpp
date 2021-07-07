#include <iostream>
#include <string>
#include <map>
#include <vector>

int A() {
    char c;
    int res(0);
    std::cin >> c;
    if (c == 'c') {
        res = A();
        if (res) {
            std::cin >> c;
            if (c == 'd') {
                return 1;
            } else {
                return 0;
            }
        }
        return res;
    }
    if (c == 'e') {
        {
            std::cin >> c;
            if (c == 'd') {
                return 1;
            } else {
                return 0;
            }
        }
    }
    return 0;
}

int B() {
    char c;
    if (std::cin >> c) {
        if (c == 'b') {
            return B();
        }
        if (c == 'd') {
            if (!(std::cin >> c)) {
                return 1;
            }
        }
        return 0;
    } 
    return 1;
}

int main() {
    char c;
    int res(0);
    std::cin >> c;
    if (c == 'a') {
        std::cin >> c;
        if (c == 'd') {
            res = A();
        } else if (c == 'e') {
            res = 1;
        } else {
            res = 0;
        }

        if (res) {
            std::cin >> c;
            if (c == 'b') {
                res = 1;
            } else {
                res = 0;
            }
        }
    } else if (c == 'c') {
        res = B();
    } 
    std::cout << res << std::endl;
    return 0;
}
