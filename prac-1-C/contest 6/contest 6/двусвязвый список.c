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
    struct list *(*sort) (struct list *root);
    void (*add) (struct list *root, int x);
    void (*free) (struct list *root);
    void (*swap) (struct list **a, struct list **b);
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


struct list* LM_create_list_root_old(void){
    struct list *root = (struct list*)malloc(sizeof(struct list));
    root->next = NULL;
    root->prev = NULL;
    return root;
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


struct list *LM_deepcopy(struct list *a){
    struct list *b = (struct list*)malloc(sizeof(struct list));
    b->data = a->data;
    b->next = a->next;
    b->prev = a->prev;
    return b;
}


void LM_swap_list_elemenst(struct list **a, struct list **b){
    /*struct list *a = *a_;
    struct list *b = *b_;
    
    struct list *tmpa = LM_deepcopy(a);
    struct list *tmpb = LM_deepcopy(b);
    
    
    b->next = a;
    b->prev = tmpa->prev;
    a->next = tmpb->next;
    a->prev = b;
    if(tmpb->next != NULL)
      tmpb->next->prev = a;
    if (tmpa->prev != NULL)
      tmpa->prev->next = b;
    
    free(tmpa);
    free(tmpb);*/
    
    int tmp = (*a)->data;
    (*a)->data = (*b)->data;
    (*b)->data = tmp;
}


struct list *LM_sort_list(struct list *root){
    struct list *l, *tail = NULL;
    while(root != tail){
        l = root;
        while (l->next != tail){
            if (l->data > l->next->data){
                LM_swap_list_elemenst(&l, &(l->next));
            }
            l = l->next;
        }
        tail = l;
    }
    return root;
}


void LM_add_element2list(struct list *root, int x){
    while(root->next != NULL){
        root = root->next;
    }
    root->next = LM_create_list_element(x);
    root->next->prev = root;
}


struct ListsMethods LM_constructor(void){
    struct ListsMethods lm;
    lm.output = LM_output_list;
    lm.foutput = LM_foutput_list;
    lm.create = LM_create_list_element;
    lm.free = LM_free_list;
    lm.sort = LM_sort_list;
    lm.add = LM_add_element2list;
    lm.swap = LM_swap_list_elemenst;
    return lm;
}



int main(void) {
    FILE *fin  = fopen("input.txt",  "r");
    FILE *fout = fopen("output.txt", "w");
    //FILE *fin  = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!input.txt",  "r");
    //FILE *fout = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!output.txt", "w");
    struct ListsMethods lm = LM_constructor();
    
    int x;
    if (fscanf(fin, "%d", &x) != -1){
        struct list *l = lm.create(x);
        char c;
        while ((c = getc(fin)) != EOF){
            fscanf(fin, "%d", &x);
            lm.add(l, x);
        }
        
        l = lm.sort(l);
        lm.foutput(fout, l);
        lm.free(l);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
