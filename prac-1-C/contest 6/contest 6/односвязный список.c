//
//  main.c
//  contest 6
//
//  Created by  Gleb on 22.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct list{
    int data;
    struct list *next;
};


struct ListsMethods{
    void (*output) (struct list *root);
    void (*foutput) (FILE *f_output, struct list *root);
    //struct list *(*create) (void);
    struct list *(*create) (int x);
    struct list *(*sort) (struct list *root);
    void (*add) (struct list *root, int x);
    void (*free) (struct list *root);
};


void LM_foutput_list(FILE *f_output, struct list *root){
    if (root != NULL){
        while (root->next != NULL){
            fprintf(f_output, "%d ", root->data);
            root = root->next;
        }
        fprintf(f_output, "%d ", root->data);
    }
}


void LM_output_list(struct list *root){
    if (root != NULL){
        while (root->next != NULL){
            printf("%d ", root->data);
            root = root->next;
        }
        printf("%d ", root->data);
    }
}


struct list* LM_create_list_root(void){
    struct list *root = (struct list*)malloc(sizeof(struct list));
    root->next = NULL;
    return root;
}


struct list* LM_create_list_element(int x){
    struct list *root = (struct list*)malloc(sizeof(struct list));
    root->data = x;
    root->next = NULL;
    return root;
}


void LM_free_list(struct list *root){
    if (root->next != NULL){
        LM_free_list(root->next);
    }
    free(root);
}


struct list *LM_sort_list_1(struct list *root){
    struct list *p, *key;
    struct list *result = root;
    root = root->next;      /* Головой стал следующий элемент */
    result->next = NULL;    /* Первый элемент отсортированного списка */

    while (root->next != NULL){
        key = root;
        root = root->next;
        if (key->data <= result->data){   /* Вставляем результат в голову */
            key->next = result;
            result = key;
        } else {
            p = result;
            while (p->next != NULL){     /* Бежим по уже сформированному результату */
                if (p->next->data > key->data)
                    break;
                p = p->next;
            }
            key->next = p->next;
            p->next = key;
        }
    }
    //root = result;
    return result;
}


struct list *LM_sort_list_2(struct list *root){
    
    return root;
}






void LM_add_element2list(struct list *root, int x){
    while(root->next != NULL){
        root = root->next;
    }
    root->next = LM_create_list_element(x);
}


struct ListsMethods LM_constructor(void){
    struct ListsMethods lm;
    lm.output = LM_output_list;
    lm.foutput = LM_foutput_list;
    lm.create = LM_create_list_element;
    lm.free = LM_free_list;
    lm.sort = LM_sort_list;
    lm.add = LM_add_element2list;
    return lm;
}



int main(void) {
    //FILE *fin  = fopen("input.txt",  "r");
    //FILE *fout = fopen("output.txt", "w");
    FILE *fin  = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!input.txt",  "r");
    FILE *fout = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!output.txt", "w");
    struct ListsMethods lm = LM_constructor();
    
    int x;
    char c;
    fscanf(fin, "%d", &x);
    struct list *l = lm.create(x);
    
    while ((c = getc(fin)) != EOF){
        fscanf(fin, "%d", &x);
        lm.add(l, x);
    }
    
    l = lm.sort(l);
    lm.foutput(fout, l);
    lm.free(l);
    fclose(fin);
    fclose(fout);
    return 0;
}
