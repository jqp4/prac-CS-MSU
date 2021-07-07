#include <iostream>
#define str std::string

//     1 01 34 5534 qwdf7q5f we7f

void lll(str word, int n){
    //std::cout << '[' << word << ']' << std::endl;
    bool is_num = true;
    for (int i = 0; i < n && is_num; i++){
        if (word[i] < '0' || word[i] > '9') {
            is_num = false;
            std::cout << word;
        }
    }
    if (is_num) {
        bool only_0_num = false;
        if (word[0] == '0') {
            only_0_num = true;
        }
        for (int i = 0; i < n; i++){
            if (word[i] != '0') {
                only_0_num = false;
            }
            if (!only_0_num) {
                std::cout << word[i];
            }
        }
        if (only_0_num) {
            std::cout << '0';
        }
    }
}



int main() {
    
    str word = "";
    int n = 0;
    char x;
   
    
    while ((x = getchar()) != EOF) {
        if (x != ' ') {
            word += x;
            n++;
        } else {
            lll(word, n);
            word = "";
            n = 0;
            std::cout << ' ';
        }
    }
    lll(word, n);
    
    
    
    /*while ((x = getchar()) != 'x') {
        if (x != ' ') {
            if (next_num) {
                next_num = false;
                if (x == '0') {
                    only_0_num = true;
                }
            }
            if (x != '0') {
                only_0_num = false;
            }
            if (!only_0_num) {
                std::cout << x;
            }
        } else if (x == ' ') {
            if (!next_num) {
                next_num = true;
                if (only_0_num) {
                    only_0_num = false;
                    std::cout << '0';
                }
            }
            std::cout << ' ';
        }
    }*/
    
    
}
