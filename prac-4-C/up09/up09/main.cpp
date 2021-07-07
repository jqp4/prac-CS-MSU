#include <iostream>
#define ll long long

class Result {
    ll res;
public:
    Result(ll inp_r) : res(inp_r) {};
    ll get() const {
        return res;
    }
};

/* this recursive function is a
 function of three Ackermann arguments */

void Ackkerfunc(ll a, ll b, ll k) {
    if (k == 0) throw Result(a + b);
    else if (b == 1) throw Result(a);
    else {
        try {
            Ackkerfunc(a, b - 1, k);
        } catch (const Result& R) {
            Ackkerfunc(a, R.get(), k - 1);
        }
    }
}

int main(void) {
    ll a, b, k;
    while (std::cin >> a >> b >> k) {
        try {
            Ackkerfunc(a, b, k);
        } catch (const Result &res) {
            std::cout << res.get() << std::endl;
        }
    }
    return 0;
}
