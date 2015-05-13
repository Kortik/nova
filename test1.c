#include "stdio.h"
#include "math.h"

inline volatile long long RDTSC() {
   register long long TSC asm("eax");
   asm volatile (".byte 15, 49" : : : "eax", "edx");
   return TSC;
} 
  int main(void)
{
	long long time_start=RDTSC();
	long long tsc_max=0;
	int num=0;
	long long proc_power=1.7*1000000000;
	long long time_stop=time_start+proc_power;
	long long *mas = new long long [600000];
	long long tmp=time_start;
	long long i=1;
    	printf("%lld time stop!\n", time_stop);
      printf("%lld Hello World!\n", i);
   mas[0]=tmp;
    for (i=1; i<=600000; i++) {
	mas[i]=RDTSC();
	
	if (mas[i]>tsc_max) tsc_max=mas[i];
	if (mas[i]>=time_stop) break;
	}
	for (long long x=0;x<=i;x++) {
      printf("%lld mas[%lld] -- mas[%lld]-mas[%lld]=%lld\n ", mas[x], x, x+1, x, mas[x+1]-mas[x]); 
	if (mas[x+1]-mas[x]>7000) num++; }
	printf("%lld time stop!\n", time_stop);
      printf("%lld mas col\n", i); 
	printf("%lld time start\n", time_start); 
	printf("%lld time stop \n", time_stop); 
	printf("%lld-MAXIMUM, %d-NUM", tsc_max, num);
return 0;
  }



