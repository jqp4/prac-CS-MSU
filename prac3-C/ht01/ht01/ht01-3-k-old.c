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
	int h = atoi(argv[1]), w = atoi(argv[2]), i, j, cur;
    // long int *arr = calloc(w * h, sizeof(*arr));
    int fd = open("test_file", O_RDWR | O_CREAT, (mode_t)0600);
    // if ( lseek(fd, sizeof(arr), SEEK_SET) == -1 ) 
    //     fprintf(stderr,"Error lseek");
    ftruncate(fd, w*h*sizeof(w));
    unsigned *map=mmap(0, w*h*sizeof(*map), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "Error mapping");
        return -1;
    }
    for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++){
            cur = spiral(w, h, j, i)+1;
            printf("%4d ", cur);
            map[i * w + j] = cur;
        }
        printf("\n");
	}
    // msync(map, w*h*sizeof(*map), MS_SYNC);
    munmap(map, sizeof(map));
    close(fd);
    return 0;
}