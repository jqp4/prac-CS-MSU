#include <iostream>
#include <string>

int getp(int *c, char x) {
    int n = 0;
    while (*c == x) {
        n++;
        *c = getchar();
    }
    return n;
}

void pra0(int *c) {
    std::cout << "0" << std::endl;
    while (!isspace(*c) && *c != EOF) {
        *c = getchar();
    }
}

int main(void) {
    int c = getchar();
    while (c != EOF) {
        while (isspace(c)) c = getchar();
        if (c == EOF) break;
        int f0 = getp(&c, '0');
        if (f0 == 0) pra0(&c);
        else if (c == '1') {
            int f1 = getp(&c, '1');
            if (c == '0') {
                bool loop = true;
                while (loop && c == '0') {
                    if (getp(&c, '0') != f0 || getp(&c, '1') != f1 || (c != '0' && !isspace(c) && c != EOF)) {
                        loop = false;
                    }
                }
                if (loop) {
                    std::cout << "1" << std::endl;
                } else pra0(&c);
            } else if (isspace(c) || c == EOF) {
                std::cout << "1" << std::endl;
            } else pra0(&c);
        } else pra0(&c);
    }
}
