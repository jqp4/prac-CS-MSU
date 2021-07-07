#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct seq{
    signed char c;
    short s;
    struct seq *next;
}

//extern f(struct seq *);

int fc(struct seq *q){
    if (!q) return 42;
    return (f(q->next) % 2) ? q->s : q->c;
}

int main(void){
    //srand((unsigned)time(NULL));
    //int n = 10;
    int ans = fc(&(struct seq){.s=2020, .c=16, .next=NULL});
    printf("f = %d\n", ans);
    



    return 0;
}