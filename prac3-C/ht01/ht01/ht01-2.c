#include <stdio.h>
#include <stdlib.h>

/* LCG - Linear congruential generator */

enum
{
    LCG_A = 1103515245,
    LCG_C = 12345,
    LCG_M = 1 << 31
};

typedef struct RandomGenerator
{
    struct RandomOperations
    {
        unsigned (*next)(struct RandomGenerator *);
        void (*destroy)(struct RandomGenerator *);
    } *ops;
    unsigned Xn;
} RandomGenerator;

unsigned
RG_next(RandomGenerator *r)
{
    return r->Xn = ((unsigned long long)LCG_A * r->Xn + LCG_C) % (unsigned)LCG_M;
}

void
RG_destroy(RandomGenerator *r)
{
    free(r->ops);
    free(r);
}

RandomGenerator *
random_create(int seed)
{
    RandomGenerator *rg = calloc(1, sizeof(*rg));
    rg->ops = calloc(1, sizeof(*(rg->ops)));
    rg->ops->destroy = RG_destroy;
    rg->ops->next = RG_next;
    rg->Xn = seed;
    return rg;
}

int
main(void)
{
    RandomGenerator *rr = random_create(1234);
    for (int j = 0; j < 10; ++j) {
        printf("%d\n", rr->ops->next(rr));
    }
    rr->ops->destroy(rr);
}
