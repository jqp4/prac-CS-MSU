//
//  main.c
//  contest 1 1.14
//
//  Created by  Gleb on 26/09/2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <math.h>


void swap(double *a, double *b){
    double tmp = *a;
    *a = *b;
    *b = tmp;
}


void output(int ans){
    if (ans == 1) printf("YES");
    else printf("NO");
}


double get_len(double h, double c, double d){
    double diag, alpha, betta, gamma, len;
    diag = sqrt(pow(c, 2) + pow(d, 2));
    if ((h / diag > 1) || (c / diag > 1))
        return -1;
    else {
        alpha = acos(h / diag);
        betta = asin(c / diag);
        gamma = alpha + betta;
        if (gamma * 180/M_PI > 90.0)
            return -1;
        else {
            len = c * cos(gamma) + d * sin(gamma);
            return len;
        }
    }
}


int main(void) {
    double a, b, c, d;
    int ans = 0;
    scanf("%lf%lf%lf%lf", &a, &b, &c, &d);
    if (c > d) swap(&c, &d);

    double len_a = get_len(a, c, d);
    double len_b = get_len(b, c, d);

    if ((a >= c) && (b >= d)) ans = 1;
    else if ((a >= d) && (b >= c)) ans = 1;
    else if (((int)len_a != -1) && (len_a <= b)) ans = 1;
    else if (((int)len_b != -1) && (len_b <= a)) ans = 1;

    output(ans);
    return 0;
}
