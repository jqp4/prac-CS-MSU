//
//  main.c
//
//  Created by  Gleb on 26/09/2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>



struct mcls{
    int n;
    
    void (*output) (struct mcls a);
};


void mcls_output(struct mcls a){
    printf("%d", a.n);
}




struct mcls mcls_constructor(void){
    struct mcls a;
    a.n = 6;
    a.output = mcls_output;
    return a;
}


int main(void){
    
    struct mcls a = mcls_constructor();
    a.output(a);
    
    return 0;
}
