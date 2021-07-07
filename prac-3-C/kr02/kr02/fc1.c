#include <stdio.h>
int main(void) {
   const char* reject = "reject";
   const char* summon = "summon";
   const char* disqualify = "disqualify"; 
   signed long long x = 0; 
   while (scanf("%lld", &x) != EOF) { 
       printf("%s\n", x > 5?summon:disqualify);
   }
   return 0;
}