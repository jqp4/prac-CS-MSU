#include <stdio.h>

typedef struct t_s stadio;

struct t_s {
    void* team;
    int capasity;
    union {
        int sectors;
        char stands[7];
    } parts;
    double popularity;
};


static stadio s, *p;
//static double d;


int main_(void) {
    s.team = 0;
    s.capasity = 2134;
    s.parts.sectors = 23;
    s.popularity = 98.12314;
    
    p = &s;

    for (int i = 0; i < sizeof(stadio); i++){
        //printf("%2d | %p | %d\n", i, (&p + i), *(*(&p + i)));
        
    }
    
    
    
    return 0;
}

int mai1n(void){
    int ax = 4, bx = 324, *a, *b;
    a = &bx;
    b = &ax;
    printf("%p %p\n", &a - 1, &b);
    printf("%d %d\n", *(*(&a - 1)), *(*(&b)));
    //int x = sizeof(union { int sectors; char stands[7]; });
    //int x = sizeof(stadio);
    //printf("%d\n", x);
    int *d = 0;
    d = &ax;
    d = 0;
    return 0;
}



struct aa{
    char c;
    char y;
    short v;
    int t;
};


int mai000n(void){
    struct aa s_deb, *s_deb_point = &s_deb;
    char   *c_point = &(s_deb.c);
    char   *y_point = &(s_deb.y);
    short  *v_point = &(s_deb.v);
    int    *t_point = &(s_deb.t);
    return 0;
}



int main(void){
    struct t_s s_deb, *s_deb_point = &s_deb;
    void* *team_pointer = &(s_deb.team);
    int *capasity_pointer = &(s_deb.capasity);
    double *popularity_pointer = &(s_deb.popularity);
    return 0;
}
