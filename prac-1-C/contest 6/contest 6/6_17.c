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



struct node{
    int key;
    int data;
    int height;
    int balance;
    struct node *left;
    struct node *right;
};


struct SearchTreesMethods{
    struct node *(*create) (int key, int data);
    struct node *(*search) (struct node *root, int key);
    struct node *(*add) (struct node *root, int key, int data);
    void (*free) (struct node *root);
    int (*get_height) (struct node *p);
    int (*get_balance) (struct node *p);
    void (*fix_height) (struct node *p);
    struct node *(*fix_balance) (struct node *p);
    struct node *(*rotate_right) (struct node *p);
    struct node *(*rotate_left) (struct node *p);
    struct node *(*delete) (struct node *p, int key);
};


struct node *TM_create_node(int key, int data){
    struct node *root = (struct node*)malloc(sizeof(struct node));
    root->key = key;
    root->data = data;
    root->height = 1;
    root->left = NULL;
    root->right = NULL;
    return root;
}


void TM_free_tree(struct node *root){
    if (root != NULL){
        TM_free_tree(root->left);
        TM_free_tree(root->right);
        free(root);
    }
}


struct node *TM_search_node(struct node *root, int key){
    if (root == NULL || root->key == key) { return root; }
    if (key < root->key){
        return TM_search_node(root->left, key);
    } else {
        return TM_search_node(root->right, key);
    }
}


int TM_get_height(struct node *p){
    return p ? p->height : 0;
}


int TM_get_balance(struct node *p){
    p->balance = TM_get_height(p->right) - TM_get_height(p->left);
    return p->balance;
}


int TM_get_key(struct node *p){
    return p ? p->key : 0;
}


void TM_show_tree(struct node *root){
    if (root != NULL){
        printf("%d: left - %d, right - %d\n", root->key, TM_get_key(root->left), TM_get_key(root->right));
        TM_show_tree(root->left);
        TM_show_tree(root->right);
    }
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


struct node *TM_add_element2tree(struct node *root, int key, int data){
    if(root == NULL) return TM_create_node(key, data);
    if(key == root->key){
        root->data = data;
        return root;
    }
    if(key < root->key){
        root->left = TM_add_element2tree(root->left, key, data);
    } else {
        root->right = TM_add_element2tree(root->right, key, data);
    }
    return TM_fix_balance(root);
}


struct node* TM_find_min_element(struct node *p){
    return (p->left != NULL) ? TM_find_min_element(p->left) : p;
}


struct node *TM_delete_min_element(struct node *p){
    if(p->left == NULL){
        return p->right;
    } else {
        p->left = TM_delete_min_element(p->left);
        return TM_fix_balance(p);
    }
}


struct node *TM_delete_element(struct node *root, int key){
    if(root == NULL) return NULL;
    if(key < root->key){
        root->left = TM_delete_element(root->left,key);
    } else if(key > root->key) {
        root->right = TM_delete_element(root->right,key);
    } else {
        struct node *l = root->left;
        struct node *r = root->right;
        if( !r ) return l;
        struct node *min = TM_find_min_element(r);
        min->right = TM_delete_min_element(r);
        min->left = l;
        return TM_fix_balance(min);
    }
    return TM_fix_balance(root);
}



struct SearchTreesMethods TM_constructor(void){
    struct SearchTreesMethods stm;
    stm.create = TM_create_node;
    stm.free = TM_free_tree;
    stm.add = TM_add_element2tree;
    stm.search = TM_search_node;
    stm.get_height = TM_get_height;
    stm.get_balance = TM_get_balance;
    stm.fix_height = TM_fix_height;
    stm.rotate_right = TM_rotate_right;
    stm.rotate_left = TM_rotate_left;
    stm.fix_balance = TM_fix_balance;
    stm.delete = TM_delete_element;
    return stm;
}





int main(void) {
    struct SearchTreesMethods stm = TM_constructor();
    struct node *root = NULL, *tmp = NULL;
    int key, data;
    char c = '\0';
    
    while (c != 'F'){
        scanf("%c", &c);
        if (c == 'A'){
            scanf("%d %d", &key, &data);
            root = stm.add(root, key, data);
        } else if (c == 'D'){
            scanf("%d", &key);
            root = stm.delete(root, key);
        } else if (c == 'S'){
            scanf("%d", &key);
            tmp = stm.search(root, key);
            if (tmp != NULL){
                printf("%d %d\n", tmp->key, tmp->data);
            }
        }
    }
    stm.free(root);
    return 0;
}
