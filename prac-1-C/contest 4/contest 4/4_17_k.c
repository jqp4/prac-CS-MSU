#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define dbg printf("Below this point")
#define  ll long long

void min_cyclic_shift (char * s) {
    strcat(s,s);
    ll int n = strlen(s);
    ll int i=0, ans=0;
    while (i < n/2) {
        ans = i;
        ll int j=i+1, k=i;

        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j])
                k = i;
            else
                ++k;
            ++j;
        }
        while (i <= k)  i += j - k;
    }
    i = ans;
    for (; i < ans+n/2; ++i){
        printf("%c", s[i]);
    }

}
int main(void){

    char input[4000002]={0};
    scanf("%s", input);
    min_cyclic_shift(input);
    return 0;
}
