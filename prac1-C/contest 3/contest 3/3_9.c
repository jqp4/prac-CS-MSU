//
//  main.c
//  contest 3
//
//  Created by  Gleb on 25.10.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <math.h>


#define i_r ((i + 1) % 3)
#define i_l ((n + i - 1) % 3)
#define n 3

//const unsigned int n = 3;


float float_abs(float a){
    return a >= 0 ? a : -a;
}


struct triangle{
    float side[n];
    float angle[n];
    float vertex[n][2];
    float vector[n][2];
    float bisector[n][2];
    
    void (*calculate_angles) (struct triangle *a);
    void (*calculate_vectors) (struct triangle *a);
    void (*calculate_sides) (struct triangle *a);
    void (*calculate_bisectors) (struct triangle *a);
    void (*input_vertices) (struct triangle *a);
    void (*output_bisectors) (struct triangle a);
    void (*output) (struct triangle a);
    
    /*
     треугольник ABC :
        стороны :
            1 - AB
            2 - BC
            3 - CA
        углы :
            1 - A
            2 - B
            3 - C
     */
};


void tr_calculate_angles(struct triangle *a){
    for (int i = 0; i < n; i++){
        float k1 = float_abs(a->vector[i][0] * a->vector[i_l][0] + a->vector[i][1] * a->vector[i_l][1]);
        float k2 = sqrt(pow(a->vector[i][0], 2) + pow(a->vector[i][1], 2));
        float k3 = sqrt(pow(a->vector[i_l][0], 2) + pow(a->vector[i_l][1], 2));
        float k4 = k1 / (k2 + k3);
        a->angle[i] = acos(k4);
    }
}


void tr_calculate_sides(struct triangle *a){
    for (int i = 0; i < n; i++){
        a->side[i] = sqrt(pow(a->vertex[i][0] - a->vertex[(i + 1) % n][0], 2) + pow(a->vertex[i][1] - a->vertex[(i + 1) % n][1], 2));
    }
}


void tr_calculate_vectors(struct triangle *a){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < 2; j++){
            a->vector[i][j] = a->vertex[i_r][j] - a->vertex[i][j];
        }
    }
}


void tr_calculate_bisectors(struct triangle *a){
    for (int i = 0; i < n; i++){ // для каждого угла :
        float k = a->side[i] / (a->side[i] + a->side[i_l]);
        //printf("{%.3f}\n", k);
        float IM[2] = {a->vector[i_r][0] * k, a->vector[i_r][1] * k};
        for (int j = 0; j < 2; j++){
            a->bisector[i][j] = a->vertex[i_r][j] + IM[j];
        }
    }
}


void tr_input_vertices(struct triangle *a){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < 2; j++){
            scanf("%f", &a->vertex[i][j]);
        }
    }
}


void tr_output(struct triangle a){
    printf("\n--- tr:\n");
    for (int i = 0; i < n; i++){
        printf("tr.vertex[%d] = [%.2f, %.2f];\n", i, a.vertex[i][0], a.vertex[i][1]);
    }
    for (int i = 0; i < n; i++){
        printf("tr.vector[%d] = [%.2f, %.2f];\n", i, a.vector[i][0], a.vector[i][1]);
    }
    for (int i = 0; i < n; i++){
        printf("tr.side[%d] = %.2f;\n", i, a.side[i]);
    }
    for (int i = 0; i < n; i++){
        printf("tr.bisector[%d] = [%.2f, %.2f];\n", i, a.bisector[i][0], a.bisector[i][1]);
    }
    for (int i = 0; i < n; i++){
        printf("tr.angle[%d] = %.2f;\n", i, a.angle[i]);
    }
    printf("---\n");
}


void tr_output_bisectors(struct triangle a){
    for (int i = 0; i < n; i++){
        printf("%.5f %.5f\n", a.bisector[i_l][0], a.bisector[i_l][1]);
    }
}


struct triangle tr_constructor(void){
    struct triangle a;
    a.calculate_angles = tr_calculate_angles;
    a.calculate_vectors = tr_calculate_vectors;
    a.calculate_sides = tr_calculate_sides;
    a.input_vertices = tr_input_vertices;
    a.calculate_bisectors = tr_calculate_bisectors;
    a.output = tr_output;
    a.output_bisectors = tr_output_bisectors;
    return a;
}



int main(void) {
    struct triangle a = tr_constructor();
    a.input_vertices(&a);
    a.calculate_vectors(&a);
    a.calculate_sides(&a);
    a.calculate_bisectors(&a);
    a.output_bisectors(a);
    return 0;
}

