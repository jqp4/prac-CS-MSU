#include <stdio.h>
#include <stdlib.h>


#define STR_SIZE 10001
#define bool int
#define True 1
#define False 0



int min(int a, int b){
    if (a < b){
        return a;
    } else {
        return b;
    }
}


int len_str(char s[]){
    for(int n = 0; n < STR_SIZE; n++){
        if (s[n] == '\0'){
            return n;
        }
    }
    return -1;
}


bool prefsuf_equal(char s1[], char s2[], int n){
    int l2 = len_str(s2);
    for (int i = 0; i < n; i++){
        if (s1[i] != s2[l2 - n + i]){
            return False;
        }
    }
    return True;
}


bool get_max_prefsuf3(char s1[], char s2[]){
    int l1 = len_str(s1), l2 = len_str(s2), l = min(l1, l2), max = 0;
    for (int n = 0; n <= l; n++){
        if (prefsuf_equal(s1, s2, n)){
            max = n;
        }
    }
    return max;
}




int main(void){
    char s1[STR_SIZE], s2[STR_SIZE];
    scanf("%s", s1);
    scanf("%s", s2);
    if (len_str(s1) == 0 && len_str(s2) == 0){
        printf("0 0");
    } else {
        printf("%d %d", get_max_prefsuf3(s1, s2), get_max_prefsuf3(s2, s1));
    }
    return 0;
}
 
