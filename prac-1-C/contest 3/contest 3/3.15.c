//
//  main.c
//  contest 3
//
//  Created by  Gleb on 25.10.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define bool int
#define True 1
#define False 0


struct TPoint{
    int x;
    int y;
};

struct TPoint input_point/*constructor*/(void){
    struct TPoint p;
    int x, y;
    scanf("%d %d", &x, &y);
    p.x = x;
    p.y = y;
    return p;
}


bool check_symmetry_recursive(int remaining_amount_points){
    struct TPoint p1 = input_point();
    if (remaining_amount_points > 0){
        bool flag = check_symmetry_recursive(remaining_amount_points - 1);
        if (flag){
            struct TPoint p2 = input_point();
            if (p1.x + p2.x == 0 && p1.y == p2.y){
                return True;
            } else {
                return False;
            }
        } else {
            return False;
        }
    } else {
        return True;
    }
}


bool check_symmetry(int n){
    n = (int)(n / 2);
    struct TPoint skip_point = input_point();
    if (skip_point.x == 0){  //the compiler requires the use of all objects
        skip_point.y = 0;
    }
    return check_symmetry_recursive(n - 1);
}



int main(void){
    int n;
    scanf("%d", &n);
    if (check_symmetry(n)){
        printf("Yes");
    } else {
        printf("No");
    }
    return 0;
}
