#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
 
int spiral(int w, int h, int x, int y)
{
    return y ? w + spiral(h - 1, w, y - 1, w - x - 1) : x;
}
 
int main(int argc, char **argv)
{
    char *eс;
    int h = (int)strtol(argv[2], &eс, 10);
    int w = (int)strtol(argv[3], &eс, 10);
    int i, j, cur;
    int fd = open(argv[1], O_RDWR | O_CREAT, (mode_t)0600);
    unsigned size_of_map = h * w * sizeof(int);
    ftruncate(fd, size_of_map);
    int *map = mmap(NULL, size_of_map, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "Error mapping");
        return -1;
    }
    
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++){
            cur = spiral(w, h, j, i) + 1;
            map[i * w + j] = cur;
            printf("%2d ", cur);
        }
        printf("\n");
    }
    munmap(map, sizeof(map));
    close(fd);
    return 0;
}
