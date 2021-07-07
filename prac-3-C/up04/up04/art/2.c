#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

enum { SUM_SHIFT = 16 };

struct Accounting
{
    char name[SUM_SHIFT];
    int price; 
};

int
main(int argc, char *argv[])
{
    int second_max = INT_MIN, max = INT_MIN, f = 0;
    for (int i = 1; i < argc; ++i) {
        int des = open(argv[i], O_RDONLY);
        if (des == -1) {
            fprintf(stderr, "open error\n");
            return 1;
        }
        while (1) {
            struct Accounting x;
            int k = read(des, &x, sizeof(x));
            if (k < sizeof(x)) {
                break;
            }
            if (k == -1) {
                fprintf(stderr, "read error\n");
                return 1;
            }
            if (x.price > max || !f) {
                second_max = max;
                max = x.price;
                if (f < 2) {
                    ++f;
                }
            } else if (x.price != max && (x.price > second_max || f < 2)) {
                second_max = x.price;
                f = 2;
            }
        }
        close(des);
    }
    if (f < 2) {
        return 0;
    }
    f = 0;
    if (second_max == INT_MIN) {
        f = 1;
    }
    if (second_max < 0) {
        putchar('-');
        second_max = -(second_max + f);
    }
    printf("%d.%02d\n", second_max / 100, second_max % 100 + f);
    return 0;
}
