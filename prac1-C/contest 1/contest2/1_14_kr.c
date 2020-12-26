#include <stdio.h>
#include <stdlib.h>
#define ll long long 
int main(void){
    unsigned int n;
    scanf("%d", &n);
    ll int p1 = 0, p2 = 0, m1 = 0, m2 = 0, a;
    if (n == 2){
        scanf("%lld%lld", &p1, &p2);
        if (p1 > p2) printf("%lld %lld\n", p2, p1);
        else printf("%lld %lld\n", p1, p2);
    }
    else
    {
        for (int i = 0; i < n; ++i){
            scanf("%lld",&a);
            if (a >= p1 && a > 0)
            {
                p2 = p1;
                p1 = a;
            }
            else if (a > p2 && a > 0) p2 = a;
            if (a <= m1 && a < 0){
                m2 = m1;
                m1 = a;
            }
            else if (a < m2 && a < 0) m2 = a;

        }
    }
    ll rp, rm;
    rp = p1 * p2;
    rm = m1 * m2;
    if (rp>rm) printf("%lld %lld\n", p2, p1);
    else printf("%lld %lld\n", m1, m2);

    return 0;
}