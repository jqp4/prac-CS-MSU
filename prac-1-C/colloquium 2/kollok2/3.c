//
//  main.c
//  kollok2
//
//  Created by  Gleb on 28.11.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>


struct S{
    int x;
    char s[5];
};

struct S1{
    long int x;
    char df[3];
};

struct S0{
    
};

int main(int argc, const char * argv[]) {
    unsigned short a[3] = {111, 222, 333};
    unsigned short *p = &a[1];
    unsigned short *q = a + 2;
    
    
    struct S s[] = {{5, "1234"}, {7, "5678"}};
    
    int fg = s[0].s[4] + s[0].x;
    
    printf("%d\n", fg);
    
    return 0;
}
