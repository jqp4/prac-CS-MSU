#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 101
#define bool int
#define True 1
#define False 0


const char vowels[] = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
const unsigned int vowels_count = 12;
const char consonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Z', 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'z'};


int len_str(char s[]){
    for(int n = 0; n < STR_SIZE; n++){
        if (s[n] == '\0'){
            return n;
        }
    }
    return -1;
}


int find(char *s, char a){
    int l = len_str(s);
    for (int i = 0; i < l; i++){
        if (s[i] == a){
            return i;
        }
    }
    return -1;
}


bool is_vowels(char a){
    /*if (find(vowels, a) != -1){
        return True;
    } else {
        return False;
    }*/
    for (int i = 0; i < vowels_count; i++){
        if (a == vowels[i]){
            return True;
        }
    }
    return False;
}


int number_of_syllables(char *s){
    int l = len_str(s), n = 0;
    bool vowels_flag = False;
    for (int i = 0; i < l; i++){
        if (is_vowels(s[i])){
            if (!vowels_flag){
                n++;
                vowels_flag = True;
            }
        } else {
            vowels_flag = False;
        }
        //flag = flag == 0;
    }
    return n;
}


void input_str(char *s){
    int k = 0;
    char a = getchar();
    while (a != '\n'){
        s[k] = a;
        k++;
        a = getchar();
    }
    s[k] = '\0';
}


void input_int_array(int *m, int n){
    for (int i = 0; i < n; i++){
        scanf("%d", &m[i]);
    }
}


void skip_char(int n){
    char skip_chr = '\0';
    for(int i = 0; i < n; i++){
        skip_chr = getchar();
    }
    if(skip_chr){
        skip_chr = '\0';
    }
}


int input_int(int *n){
    scanf("%d", n);
    return *n;
}


/*void input_strs(char **s){
    int i = 0, n = 0, c;
    while ((c = getchar()) != EOF){
        if (c == '\n'){
            s[i *STR_SIZE + n]  = '\0';
            n = 0;
            i++;
        } else {
            s[i *STR_SIZE + n] = (char)c;
            n++;
        }
    }
    s[i *STR_SIZE + n] = '\0';
}*/


bool is_valid_ss(char **s){
    return True;
}

int main(void){
    int n = input_int(&n);
    int m[n];
    input_int_array(m, n);
    char s[n][STR_SIZE];
    //char **s = (char**)malloc(n * STR_SIZE * sizeof(char));
    skip_char(1); // между числами и предложениями есть лишний \n
    for (int i = 0; i < n; i++){
        input_str(s[i]);
    }
    for (int i = 0; i < n; i++){
        if (number_of_syllables(s[i]) == m[i]){
            printf("%s\n", s[i]);
        }
    }
    return 0;
}
