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


#define STR_MAX_SIZE 101


struct node{
    unsigned long int IP;
    char *name;
    int height;
    int balance;
    struct node *left;
    struct node *right;
};


struct SearchTreesMethods{
    struct node *(*create) (char *name, unsigned long int IP, int len);
    struct node *(*search) (struct node *root, char *name, int len);
    struct node *(*add) (struct node *root, char *name, unsigned long int IP, int len);
    void (*free) (struct node *root);
    int (*get_height) (struct node *p);
    int (*get_balance) (struct node *p);
    void (*fix_height) (struct node *p);
    struct node *(*fix_balance) (struct node *p);
    struct node *(*rotate_right) (struct node *p);
    struct node *(*rotate_left) (struct node *p);
};


struct node *TM_create_node(char *name, unsigned long int IP, int len){
    struct node *root = (struct node*)malloc(sizeof(struct node));
    //root->name = (char *)malloc(STR_MAX_SIZE * sizeof(char));
    root->name = (char *)malloc((len + 1) * sizeof(char));
    strcpy(root->name, name);
    root->IP = IP;
    root->height = 1;
    root->left = NULL;
    root->right = NULL;
    return root;
}


int name1_is_greater_than_name2(char *n1, char *n2, int len){
    // n1 строго больше n2 :
    // l1 = l2 по реализации
    for (int i = 0; i < len; i++){
        if (n1[i] > n2[i]){
            return 1;
        }
        if (n1[i] < n2[i]){
            return 0;
        }
    }
    return -1; // если равны (по условию такого не будет)
}


int name1_is_equal_name2(char *n1, char *n2, int len){
    for (int i = 0; i < len; i++){
        if (n1[i] != n2[i]){
            return 0;
        }
    }
    return 1;
}


struct node *TM_search_node(struct node *root, char *name, int len){
    if (root == NULL || name1_is_equal_name2(root->name, name, len)) { return root; }
    if (name1_is_greater_than_name2(name, root->name, len)){
        return TM_search_node(root->right, name, len);
    } else {
        return TM_search_node(root->left, name, len);
    }
}


int TM_get_height(struct node *p){
    return p ? p->height : 0;
}


int TM_get_balance(struct node *p){
    p->balance = TM_get_height(p->right) - TM_get_height(p->left);
    return p->balance;
}


void TM_fix_height(struct node *p){
    int hl = TM_get_height(p->left);
    int hr = TM_get_height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}


struct node *TM_rotate_right(struct node *p){
    struct node *tmp = p->left;
    p->left = tmp->right;
    tmp->right = p;
    TM_fix_height(p);
    TM_fix_height(tmp);
    return tmp;
}


struct node *TM_rotate_left(struct node *p){
    struct node *tmp = p->right;
    p->right = tmp->left;
    tmp->left = p;
    TM_fix_height(p);
    TM_fix_height(tmp);
    return tmp;
}


struct node *TM_fix_balance(struct node *p){
    TM_fix_height(p);
    if (TM_get_balance(p) == 2){
        if (TM_get_balance(p->right) < 0){
            p->right = TM_rotate_right(p->right);
        }
        p = TM_rotate_left(p);
    } else if (TM_get_balance(p) == -2){
        if (TM_get_balance(p->left) > 0){
            p->left = TM_rotate_left(p->left);
        }
        p = TM_rotate_right(p);
    }
    return p;
}


struct node *TM_add_element2tree(struct node *root, char *name, unsigned long int IP, int len){
    if(root == NULL) return TM_create_node(name, IP, len);
    if (name1_is_greater_than_name2(name, root->name, len)){
        root->right = TM_add_element2tree(root->right, name, IP, len);
    } else {
        root->left = TM_add_element2tree(root->left, name, IP, len);
    }
    return TM_fix_balance(root);
}



struct SearchTreesMethods TM_constructor(void){
    struct SearchTreesMethods stm;
    stm.create = TM_create_node;
    stm.add = TM_add_element2tree;
    stm.search = TM_search_node;
    stm.get_height = TM_get_height;
    stm.get_balance = TM_get_balance;
    stm.fix_height = TM_fix_height;
    stm.rotate_right = TM_rotate_right;
    stm.rotate_left = TM_rotate_left;
    stm.fix_balance = TM_fix_balance;
    return stm;
}





int main(void) {
    FILE *fin  = fopen("input.txt",  "r");
    FILE *fout = fopen("output.txt", "w");
    //FILE *fin  = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!input.txt",  "r");
    //FILE *fout = fopen("/Users/gleb/Projects/C/contest 6/contest 6/!output.txt", "w");
    struct SearchTreesMethods stm = TM_constructor();
    int size = STR_MAX_SIZE;
    struct node *domainTree[size], *tmp;
    for (int i = 0; i < size; i++){ domainTree[i] = NULL; }
    
    char *name = (char *)malloc(size * sizeof(char));
    int n, m, len;
    unsigned long int IP;
    fscanf(fin, "%d", &n);
    for (int i = 0; i < n; i++){
        fscanf(fin, "%s %lu", name, &IP);
        len = (int)strlen(name);
        domainTree[len] = stm.add(domainTree[len], name, IP, len);
    }
    
    fscanf(fin, "%d", &m);
    for (int i = 0; i < m; i++){
        fscanf(fin, "%s", name);
        len = (int)strlen(name);
        tmp = stm.search(domainTree[len], name, len);
        if (tmp != NULL){
            fprintf(fout, "%lu\n", tmp->IP);
        } else {
            fprintf(fout, "%d\n", -1);
        }
    }
    
    free(name);
    fclose(fin);
    fclose(fout);
    return 0;
}
