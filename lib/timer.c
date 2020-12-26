#include <stdio.h>
#include <sys/time.h>
struct timeval tv1,tv2,dtv;
struct timezone tz;
void time_start (){
    gettimeofday(&tv1, &tz);
}
long time_stop (){
    gettimeofday(&tv2, &tz);
    dtv.tv_sec  = tv2.tv_sec  - tv1.tv_sec;
    dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
    if (dtv.tv_usec < 0){
        dtv.tv_sec--;
        dtv.tv_usec += 1000000;
    }
    return dtv.tv_sec * 1000 + dtv.tv_usec / 1000;
}




int
main(void)
{
    short N, a, b, c, cmax, i;
    scanf("%hi", &N);
    time_start();
    const short r = N > 100 ? N / 10 : N;
    short m[r][N];
    for (i = 0; i < N; i += r){
        cmax = i + r < N ? i + r : N;
        for (b = 0; b < N; b++){
            for (a = 0; a < N; a++){
                c = (a * b) % N;
                if (c >= i && c < cmax){
                    m[c % r][a] = b;
                }
            }
        }
        cmax -= i;
        for (c = 0; c < cmax; c++){
            for (a = 1; a < N; a++){
                //printf("%hi ", m[c][a]);
            }
            //printf("\n");
        }
    }
    printf("Time: %ld\n", time_stop());
    return 0;
}


