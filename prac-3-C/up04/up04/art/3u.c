#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <fcntl.h>

enum { SUBTREES_COUNT = 2 };

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int
r_l_print(int des, int32_t node_idx)
{
    off_t sh = -1;
    struct Node node;
    if (read(des, &node, sizeof(node)) == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    int32_t next_idx = node.right_idx;
    for (int i = 0; i < SUBTREES_COUNT; ++i) {
        if (next_idx) {
            sh += next_idx - node_idx;
            if (lseek(des, sh * sizeof(node), SEEK_CUR) == -1) {
                fprintf(stderr, "lseek error\n");
                return 1;
            }
            if (r_l_print(des, next_idx)) {
                return 1;
            }
            sh = node_idx - next_idx;
        }
        if (!i) {
            printf("%d\n", node.key);
            next_idx = node.left_idx;
        }
    }
    if (lseek(des, sh * sizeof(node), SEEK_CUR) == -1) {
        fprintf(stderr, "lseek error\n");
        return 1;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "invalid arguments\n");
        return 1;
    }
    int des = open(argv[1], O_RDONLY);
    if (des == -1) {
        fprintf(stderr, "open error\n");
        return 1;
    }
    if (r_l_print(des, 0)) {
        return 1;
    }
    putchar('\n');
    return 0;
}
