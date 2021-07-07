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
    struct list *prev;
};


struct ListsMethods{
    void (*output) (struct list *root);
    void (*foutput) (FILE *f_output, struct list *root);
    struct list *(*create) (int x);
    void (*add2root) (struct list *root, int x);
    void (*add2tail) (struct list *tail, int x);
    void (*free) (struct list *root);
};


void LM_foutput_list(FILE *f_output, struct list *root){
    if (root != NULL){
        while (root->next != NULL){
            fprintf(f_output, "%d ", root->data);
            root = root->next;
        }
        fprintf(f_output, "%d", root->data);
    }
}


void LM_output_list(struct list *root){
    if (root != NULL){
        while (root->next != NULL){
            printf("%d ", root->data);
            root = root->next;
        }
        printf("%d", root->data);
    }
}

struct list* LM_create_list_element(int x){
    struct list *root = (struct list*)malloc(sizeof(struct list));
    root->data = x;
    root->next = NULL;
    root->prev = NULL;
    return root;
}


void LM_free_list(struct list *root){
    if (root->next != NULL){
        LM_free_list(root->next);
    }
    free(root);
}


void LM_add_element2list_root(struct list *root, int x){
    while(root->next != NULL){
        root = root->next;
    }
    root->next = LM_create_list_element(x);
    root->next->prev = root;
}


void LM_add_element2list_tail(struct list *tail, int x){
    tail->next = LM_create_list_element(x);
    tail->next->prev = tail;
}


struct ListsMethods LM_constructor(void){
    struct ListsMethods lm;
    lm.output = LM_output_list;
    lm.foutput = LM_foutput_list;
    lm.create = LM_create_list_element;
    lm.free = LM_free_list;
    lm.add2root = LM_add_element2list_root;
    lm.add2tail = LM_add_element2list_tail;
    //lm.add_with_ret = LM_add_element2list;
    return lm;
}


void xxx(struct list **root, struct list **b, struct list **c){
    if ((*b)->prev != NULL){
        (*b)->prev->next = (*c)->next;
        if ((*c)->next != NULL){
            (*c)->next->prev = (*b)->prev;
        }
        (*c)->next = *root;
        (*root)->prev = *c;
        (*b)->prev = NULL;
        *root = *b;
    }
}




int main(void) {
    //FILE *fin  = fopen("input.txt",  "r");
    //FILE *fout = fopen("output.txt", "w");
    FILE *fin  = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!input.txt",  "r");
    FILE *fout = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!output.txt", "w");
    struct ListsMethods lm = LM_constructor();
    
    int n, m;
    fscanf(fin, "%d %d", &n, &m);
    struct list *root = lm.create(1);
    struct list *list_array[n + 1];
    list_array[1] = root;
    struct list *l = root;
    
    for (int i = 2; i <= n; i++){
        lm.add2tail(l, i);
        l = l->next;
        list_array[i] = l;
    }
   
    int b[m], c[m];
    for (int i = 0; i < m; i++){
        fscanf(fin, "%d %d", &b[i], &c[i]);
    }
    
    for (int i = 0; i < m; i++){
        xxx(&root, &list_array[b[i]], &list_array[c[i]]);
    }
    
    lm.foutput(fout, root);
    lm.free(root);
    fclose(fin);
    fclose(fout);
    return 0;
}
