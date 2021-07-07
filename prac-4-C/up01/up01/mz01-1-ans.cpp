#define ll long long int

class Sum
{
    ll sum = 0;
public:
    Sum (ll a, ll b) : sum(a + b) {}
    Sum (const Sum &s1, ll c) : sum(s1.sum + c) {}
    ll get() const
    {
        return this->sum;
    }
};
