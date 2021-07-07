#include <iostream>

class A
{
    int x;
    bool bdestruct;
public:
    A() : bdestruct(false)
    {
        std::cin >> x;
    }
    A(const A& temp) : bdestruct(true)
    {
        int y;
        std::cin >> y;
        x = temp.x + y;
    }
    ~A()
    {
        if (bdestruct) {
            std::cout << x << "\n";
        }
    }
};

/*int main() {
    std::cout << "> ";
    A a, b(a);
}*/
