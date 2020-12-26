//
//  main.c
//  kollok2
//
//  Created by  GlebGlock on 28.11.2019.
//  Copyright © 2019  GlebGlock. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define len(xs) (int)(sizeof(xs)/sizeof(xs[0]))


struct list {
    int x;
    struct list *big, *next;
};


const unsigned long list_SIZE = sizeof(struct list);


struct list *create_head_list (int x){
    struct list *head = malloc(list_SIZE);
    head->next = NULL;
    head->big = NULL;
    head->x = x;
    return head;
}


void add_to_list (struct list *head, int new_x){
    struct list *new_el = malloc(list_SIZE);
    struct list *el = head;
    while (el->next != NULL){
        el = el->next;
    }
    el->next = new_el;
    el = el->next;
    el->x = new_x;
    el->big = NULL;
    el->next = NULL;
}


void stabilize_list (struct list *head){
    while (head->next != NULL){
        head->big = NULL;
        struct list *el = head;
        while (el->next != NULL){
            el = el->next;
            if (el->x > head->x){
                head->big = el;
                break;
            }
        }
        head = head->next;
    }
    head->big = NULL;
}


struct list *create_list (int n, int xs[]){
    struct list *head = create_head_list(xs[0]);
    for (int i = 1; i < n; i++){ add_to_list(head, xs[i]); }
    stabilize_list(head);
    return head;
}


void free_list (struct list *head){
    if (head->next != NULL){
        free_list(head->next);
    }
    free(head);
}


void print_el (struct list *el){
    printf("x = %2d, addr = %4lld, next = %4lld, big = %4lld\n", el->x, (long long int)(el) % 10000, (long long int)(el->next) % 10000, (long long int)(el->big) % 10000);
}


void output_list (struct list *head){
    print_el(head);
    while (head->next != NULL){
        head = head->next;
        print_el(head);
    }
}


struct list *remove1 (struct list *l, int x){
    struct list *head = l;
    if (l->x == x){
        head = l->next;
    } else {
        while(l->next != NULL){
            if (l->next->x == x){
                l->next = l->next->next;
                break;
            }
            l = l->next;
        }
    }
    
    stabilize_list(head);
    return head;
}


struct list *remove2 (struct list *l, int x){
    //не работает
    struct list *head = l;
    while(l->next->next->x != x){
        l = l->next;
    }
    l->next = l->next->next;
    
    
    stabilize_list(head);
    return head;
}



int main (int argc, const char * argv[]) {
    //int xs[] = {1,1,1,1,1,1,2,2,3,3,4,56,6,7,8,98,9,0,9,9,9,9,-90,0,-788778,34252};
    int xs[] = {5, 1, 3, 7, 4};
    struct list *head = create_list(len(xs), xs);
    
    printf("до:\n");
    output_list(head);
    
    printf("после:\n");
    head = remove1(head, 7);
    output_list(head);
    
    free_list(head);
    return 0;
}

