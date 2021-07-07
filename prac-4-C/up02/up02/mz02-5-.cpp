#include <iostream>
#include <string>

class BinaryNumber
{
public:
    std::string num;
    BinaryNumber(const std::string &s){
        this->num = s;
    }
    operator std::string () const;
    const BinaryNumber &operator++(){
        unsigned long len = num.length();
        num[len - 1] = '1';
        
        BinaryNumber ans = BinaryNumber(num);
        
        return ans;
    }
};




int main(){
    BinaryNumber x = BinaryNumber("123fgh");
    std::cout << x.num << "\n";
    x++;
    std::cout << x.num << "\n";
}
