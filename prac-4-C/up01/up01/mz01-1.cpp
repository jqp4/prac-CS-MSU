#include <iostream>

#define ll long long int
class Sum
{
    ll sum = 0;
public:
    Sum (ll a, ll b) : sum(a + b) {}
    Sum (const Sum &s1, ll c) : sum(s1.sum + c) {}
    ll get() const {
        return this->sum;
    }
};


int main(int argc, const char * argv[]) {
    ll a = 3;
    ll b = 5;
    ll c = -1;
    ll s1 = Sum(a, b).get();
    ll s2 = Sum(Sum(a,b),c).get();
    std::cout << s1 << " " << s2 << std::endl;
    return 0;
}
