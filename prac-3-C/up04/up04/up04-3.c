#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void generator_old(int argc, char *argv[], int index){
    //printf("%3d %d %d\n", p.key, p.left_idx, p.right_idx);
    struct Node p;
    srand((unsigned)time(NULL));
    int fd = open(argv[index], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
    int len = 7;
    int k[] = {4, 2, 6, 1, 3, 5, 7};
    int l[] = {1, 3, 5, 0, 0, 0 ,0};
    int r[] = {2, 4, 6, 0, 0, 0, 0};
    for (int i = 0 ; i < len; i++) {
        /*p.key = rand() % 1000;
        p.left_idx = rand() % len;
        p.right_idx = rand() % len;*/
        p.key = k[i];
        p.left_idx = l[i];
        p.right_idx = r[i];
        write(fd, &p, sizeof(p));
    }
    close(fd);
}



void getMinNode(int fd, struct Node p){
    struct Node newp;
    if(p.right_idx){
        lseek(fd, sizeof(p) * p.right_idx, SEEK_SET);
        if (read(fd, &newp, sizeof(newp)) != sizeof(newp)){
            return;
        }
        getMinNode(fd, newp);
    }
    printf("%d ", p.key);
    if(p.left_idx){
        lseek(fd, sizeof(p) * p.left_idx, SEEK_SET);
        if (read(fd, &newp, sizeof(newp)) != sizeof(newp)){
            return;
        }
        getMinNode(fd, newp);
    }
    return;
}


int main(int argc, char *argv[]){
    int fd = open(argv[1], O_RDONLY);
    struct Node p;
    if (read(fd, &p, sizeof(p)) == sizeof(p)){
        getMinNode(fd, p);
    }
    putchar('\n');
    return 0;
}
