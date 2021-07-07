//
//  main.c
//  kollok2
//
//  Created by  GlebGlock on 28.11.2019.
//  Copyright © 2019  GlebGlock. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int len(char *s){
    int l = 0;
    while(s[l] != '\0'){
        l++;
    }
    return l;
}


int char_count(char *s, char a){
    int count = 0, n = len(s);
    for (int i = 0; i < n; i++){
        if (s[i] == a){
            count++;
        }
    }
    return count;
}


int find(char *s, char a, int num){
    int i, n = len(s);
    for (i = 0; i < n; i++){
        if (s[i] == a && !--num){
            break;
        }
    }
    return i!=n?i:-1;
}


char *sub_str(char *s, int i_beg, int i_end){
    int n = i_end - i_beg + 1;
    //char *ss = (char *)malloc((n + 1) * sizeof(char));
    char ss[n];
    for (int i = 0; i < n; i++){
        ss[i] = s[i + i_beg];
    }
    return ss;
}


char *str_cat(char *s, char *ns){
    int l = len(ns);
    int n = len(s);
    s = (char *)realloc(s, l * sizeof(char));
    for (int i = 0; i < l; i++){
        s[i + n] = ns[i];
    }
    return s;
}


char *str_cat_plus(char *s, char *ns, char plsh){
    int l = len(ns);
    int n = len(s);
    s = (char *)realloc(s, (l + 1) * sizeof(char));
    for (int i = 0; i < l; i++){
        s[i + n] = ns[i];
    }
    s[l + n] = plsh;
    return s;
}


char *canonical(char *path){
    int n = char_count(path, '/'), done = 0, cur = 0;;
    char *f[n];
    while(!done){
        if (find(path, '/', n + 2) == -1){
            done = 1;
        } else {
            f[cur++] = sub_str(path, find(path, '/', cur + 1) + 1, find(path, '/', cur + 2) - 1);
        }
    }
    char *filname = sub_str(path, find(path, '/', cur + 1) + 1, len(path) - 1);
    
    
    
    char *can_path = (char *)malloc(sizeof(char));
    can_path[0] = '/';
    
    for (int i = 0; i < n; i++){
        can_path = str_cat_plus(can_path, f[i], '/');
        
        
    }
    
    
    
    
    
    return can_path;
}



int main (void) {
    //char s[] = "/a/asd/../file";
    char s[] = "/a/f/f/file";
    printf("%s\n", canonical(s));
    
    return 0;
}


int main_ (void) {
    //char s[] = "/a/b/cd/asd/../file";
    
    
    char *a = (char *)malloc(sizeof(char));
    a[0] = '/';
    int n = len(a);
    a = str_cat(a, "123");
    /*a = (char *)realloc(a, 2*sizeof(char));
    a[n++] = 'w';
    a[n++] = '2';
    a = (char *)realloc(a, sizeof(char));
    a[n++] = 'g';
    a = (char *)realloc(a, sizeof(char));
    a[n++] = 't';*/
    
    printf("%s", a);
    
    return 0;
}

