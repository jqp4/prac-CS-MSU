#include <stdio.h>
#include <stdbool.h>

/*regular find func: ^[a-z](<...>*/

int
main(int argc, char *argv[])
{
    double x, y;
    if (scanf("%lf%lf", &x, &y) != 2) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    bool a = (x >= 1 && x <= 7 && y >= 2 && y <= 5 && y <= -x + 10);
    printf("%d\n", a);
}