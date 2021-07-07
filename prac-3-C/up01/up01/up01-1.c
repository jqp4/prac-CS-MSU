#include <stdio.h>

int
main(void)
{
    double x, y;
    scanf("%lf%lf", &x, &y);
    int ans = y >= 2 && y <= 5 && x >= 1 && x <= 7 && x + y <= 10;
    printf("%d\n", ans);
    return 0;
}
