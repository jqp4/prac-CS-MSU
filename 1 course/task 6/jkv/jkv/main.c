//
//  main.c
//  TASK 6 V 1 1 1
//
//  Created by  Gleb on 17.05.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define N 3
#define epsilon  1e-3
#define epsilon1 1e-4
#define epsilon2 1e-4
// intersection borders
#define ib01bot  1.0
#define ib01top  1.5
#define ib12bot  0.0
#define ib12top  1.0
#define ib02bot -3.0
#define ib02top -2.0

int noi; // number of iterations
#define p(x) (f(x) - g(x)) // for root
#define cmparg(arg, s1, s2) (!strcmp(arg, s1) || !strcmp(arg, s2)) // for args processing

// NASM funcs:
//extern double f1(double);
//extern double f2(double);
//extern double f3(double);
// C funcs:
double (*f1)(double);
double (*f2)(double);
double (*f3)(double);
double _f1(double x){ return pow(2, x) + 1; }
double _f2(double x){ return pow(x, 5);     }
double _f3(double x){ return (1 - x) / 3;   }
double (**fs)(double);

// TFData - struct with f1, f2, f3 data
struct TFData{
    double fb_bot[N], fb_top[N];   // functions borders
    double fa[N];                  // functions areas
    double area;                   // figure area
    double abscissa01;             // abscissas
    double abscissa12;
    double abscissa02;
    int a01noi, a12noi, a02noi;    // number of iterations for calculate abscissas
};

// pre-initialization functions:
void   TAPI_get_nasm_functions(void);
double TAPI_root(double (double), double (double), double, double, double);
double TAPI_integral(double (double), double, double, double);
void   TAPI_get_functions_abscissas(struct TFData *);
void   TAPI_get_functions_borders(struct TFData *);
void   TAPI_get_areas(struct TFData *);
void   TUI_show_help(void);
void   TUI_show_functions(void);
void   TUI_show_abscissas_of_crossing(struct TFData data);
void   TUI_show_number_of_iterations_abscissas_of_crossing(struct TFData data);
void   TUI_show_all_functions_info(struct TFData);
void   TUI_show_area(struct TFData);

// TAPI - struct with math methods and user interface
struct TAPI{
    void   (*get_nasm_functions)(void);
    double (*root)(double (double), double (double), double, double, double);
    double (*integral)(double (double), double, double, double);
    void   (*get_functions_abscissas)(struct TFData *);
    void   (*get_functions_borders)(struct TFData *);
    void   (*get_areas)(struct TFData *);
    
    struct TUserInterface{
        void (*show_help)(void);
        void (*show_functions)(void);
        void (*show_abscissas_of_crossing)(struct TFData);
        void (*show_number_of_iterations_abscissas_of_crossing)(struct TFData);
        void (*show_all_functions_info)(struct TFData);
        void (*show_area)(struct TFData);
    } ui;
    
} api = {TAPI_get_nasm_functions, TAPI_root, TAPI_integral, \
    TAPI_get_functions_abscissas, TAPI_get_functions_borders, \
    TAPI_get_areas, {TUI_show_help, TUI_show_functions, \
    TUI_show_abscissas_of_crossing, TUI_show_number_of_iterations_abscissas_of_crossing, \
    TUI_show_all_functions_info, TUI_show_area}};


void   TAPI_get_nasm_functions(void){
     f1 = _f1;
     f2 = _f2;
     f3 = _f3;
     fs = malloc(sizeof(double (*[3])(double)));
     fs[0] = *f1;
     fs[1] = *f2;
     fs[2] = *f3;
}

double TAPI_root(double f(double), double g(double), double a, double b, double eps){
    noi = 0;
    while (b - a >= eps){
        noi ++;
        double x0 = (a + b) / 2;
        if (p(x0) == 0){ return x0; }
        else if (p(a) * p(x0) < 0){ b = x0; }
        else { a = x0; }
    }
    return (a + b) / 2;
}

double TAPI_integral(double f(double), double a, double b, double eps){
    int n = (int)((b - a) / eps) + 1;
    double step = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; i++){
        sum += f(a + step * (i + 0.5));
    }
    return sum * step;
}

void   TAPI_get_functions_abscissas(struct TFData *data){
    data->abscissa01 = api.root(fs[0], fs[1], ib01bot, ib01top, epsilon1);
    data->a01noi = noi;
    data->abscissa12 = api.root(fs[1], fs[2], ib12bot, ib12top, epsilon1);
    data->a12noi = noi;
    data->abscissa02 = api.root(fs[0], fs[2], ib02bot, ib02top, epsilon1);
    data->a02noi = noi;
}

void   TAPI_get_functions_borders(struct TFData *data){
    /*data->fb_bot[0] = api.root(fs[0], fs[2], ib02bot, ib02top, epsilon1);
    data->fb_top[0] = api.root(fs[0], fs[1], ib01bot, ib01top, epsilon1);
    data->fb_bot[1] = api.root(fs[1], fs[2], ib12bot, ib12top, epsilon1);
    data->fb_top[1] = data->fb_top[0];
    data->fb_bot[2] = data->fb_bot[0];
    data->fb_top[2] = data->fb_bot[1];*/
    data->fb_bot[0] = data->abscissa02;
    data->fb_top[0] = data->abscissa01;
    data->fb_bot[1] = data->abscissa12;
    data->fb_top[1] = data->abscissa01;
    data->fb_bot[2] = data->abscissa02;
    data->fb_top[2] = data->abscissa12;
}

void   TAPI_get_areas(struct TFData *data){
    for (int i = 0; i < N; i++){
        data->fa[i] = api.integral(fs[i], data->fb_bot[i], data->fb_top[i], epsilon2);
    }
    data->area = data->fa[0] - data->fa[1] - data->fa[2];
}

void   TUI_show_help(void){
    printf("Task 6 option 1 1 1\nSkryabin Gleb 104 group CS MSU.\n");
    printf("Type '-h' or '--help' to see this list.\nThe following options are available:\n");
    printf("\t-r   \t(--roots)           Display list of abscissas of functions crossing\n");
    printf("\t-ri  \t(--roots-iteration) Display list of number of iterations to calculate abscissas of functions crossing\n");
    printf("\t-f   \t(--functions)       Display list of investigated functions\n");
    printf("\t-a   \t(--area)            Display area of ​​the figure bounded by curves of three functions\n");
}

void   TUI_show_functions(void){
    printf("Functions:\n\tf1 = 2^x + 1\n\tf2 = x^5\n\tf3 = (1 − x) / 3\n");
}

void   TUI_show_area(struct TFData data){
    printf("Figure area = %f\n", data.area);
}

void   TUI_show_abscissas_of_crossing(struct TFData data){
    printf("Abscissa of crossing:\n");
    printf("\tf1 and f2 - %f\n", data.abscissa01);
    printf("\tf2 and f3 - %f\n", data.abscissa12);
    printf("\tf1 and f3 - %f\n", data.abscissa02);
}

void   TUI_show_number_of_iterations_abscissas_of_crossing(struct TFData data){
    printf("Number of iterations for calculate abscissa of crossing:\n");
    printf("\tf1 and f2 - %d\n", data.a01noi);
    printf("\tf2 and f3 - %d\n", data.a12noi);
    printf("\tf1 and f3 - %d\n", data.a02noi);
}

void   TUI_show_all_functions_info(struct TFData data){
    api.ui.show_abscissas_of_crossing(data);
    api.ui.show_number_of_iterations_abscissas_of_crossing(data);
    printf("Borders:\n");
    for (int i = 0; i < N; i++){
        printf("\tf%d - from %f to %f\n", i + 1, data.fb_bot[i], data.fb_top[i]);
    }
    printf("Areas:\n");
    for (int i = 0; i < N; i++){
        printf("\tf%d - %f\n", i + 1, data.fa[i]);
    }
    api.ui.show_area(data);
}


double a;


double f(double x){
    return cos(2*x)/pow(log(x+1) - log(x), a);
}

double absf(double x){
    double cf = f(x);
    return cf >= 0 ? cf : -cf;
}


int _main(int argc, const char * argv[]) {
    struct TFData data = {0};
    api.get_nasm_functions();
    api.get_functions_abscissas(&data);
    api.get_functions_borders(&data);
    api.get_areas(&data);
    //api.ui.show_all_functions_info(data);
    
    for (int i = 1; i < argc; i++){
        if (cmparg(argv[i],  "-h", "--help")){
            api.ui.show_help();
        } else if (cmparg(argv[i], "-r", "--roots")){
            api.ui.show_abscissas_of_crossing(data);
        } else if (cmparg(argv[i], "-ri", "--roots-iteration")){
            api.ui.show_number_of_iterations_abscissas_of_crossing(data);
        } else if (cmparg(argv[i], "-f", "--functions")){
            api.ui.show_functions();
        } else if (cmparg(argv[i], "-a", "--area")){
            api.ui.show_area(data);
        }
    }
    
    printf("%f\n", api.integral(fs[2], 1, 2, epsilon2));
    
    
    
    
    
    
    free(fs);
    return 0;
}


int main(void){
    a = -1.000000001;
    double (*pf)(double) = *absf;
    //printf("%f\n", (*pf)(1));
    printf("%f\n", api.integral((*pf), 1, 100, 0.001));
    
    return 0;
}
