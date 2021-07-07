//
//  main.c
//  contest 1 1.14
//
//  Created by  Gleb on 26/09/2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>

#define ll long long


void swap(ll *a, ll *b){
    ll tmp = *a;
    *a = *b;
    *b = tmp;
}


void normalization_p(ll *p, ll x){
    if (x > p[0]){
        p[1] = p[0];
        p[0] = x;
    } else if (x > p[1]){
        p[1] = x;
    }
}


void normalization_m(ll *m, ll x){
    if (x < m[0]){
        m[1] = m[0];
        m[0] = x;
    } else if (x < m[1]){
        m[1] = x;
    }
}


void normalization(ll *p, ll *m, ll x){
    if (x > 0){
        normalization_p(p, x);
        p[2]++;
    }
    else if (x < 0){
        normalization_m(m, x);
        m[2]++;
    }
    else if (x == 0) {
        p[2]++;
        m[2]++;
        //normalization_p(p, x);
        //normalization_m(m, x);
    }
}


void output_couple(ll *a) {
    if (a[0] + a[1] >= 0) printf("%lld %lld", a[1], a[0]);
    else printf("%lld %lld", a[0], a[1]);
}


void output_info(ll *p,  ll *m){
    printf("num_p = %lld, p = {%lld, %lld}\nnum_m = %lld, m = {%lld, %lld}\n", p[2], p[0], p[1], m[2], m[0], m[1]);
}


void output(ll *p,  ll *m){
    if ((p[2] >= 2) && (m[2] >= 2)) {
        if (p[0] * p[1] > m[0] * m[1]){
            output_couple(p);
        } else if (p[0] * p[1] <= m[0] * m[1]){
            output_couple(m);
        }
    } else if ((p[2] >= 2) && (m[2] < 2)) {
        output_couple(p);
    } else if ((p[2] < 2) && (m[2] >= 2)) {
        output_couple(m);
    } else {
        //output_info(a, ma);
    }
}


void spec_main_n2(void){
    ll a, b;
    scanf("%lld", &a);
    scanf("%lld", &b);
    if (a > b) swap(&a, &b);
    printf("%lld %lld", a, b);
}


int main(void) {
    int n;
    scanf("%d", &n);
    if (n != 2){
        ll x, p[3] = {0, 0, 0}, m[3] = {0, 0, 0};
        for (int i = 0; i < n; i++){
            scanf("%lld", &x);
            normalization(p, m, x);
        }
        output(p, m);
    } else spec_main_n2();
    return 0;
}
