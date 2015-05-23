#include "sigma0/console.h"
#include "nul/program.h"

#define MAS_MAX 2<<15
#define MAS_MAX_RT 10
class Hello : public NovaProgram, public ProgramConsole
{
public:
	static __inline__ unsigned long long RDTSC(void)
		{
		 unsigned hi, lo;
		__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
		return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
	}

	void run(Utcb *utcb, Hip *hip)
	{
		long long time_start=RDTSC(); //Time start
		long long proc_power=1.7*1000000000; // Тк вычислительная мощность процессора 1.7GHz
		long long time_stop=time_start+proc_power; //Time stop
		long long rdtsc_new=0, rdtsc_old=0, tsc_min=time_start, tsc=0, rdtsc_start=0, tsc_avg; 
		long long *mas = new long long[MAS_MAX];//[MAS_MAX];
		long long *mas_tsc = new long long[MAS_MAX];//[MAS_MAX];
		int mas_rdtsc[MAS_MAX_RT];
		long long iter_tsc = 10000;
		long i=0;
		init(hip);
		init_mem(hip);
		console_init("Hello", new Semaphore(alloc_cap(), true));
		for (i=0; i<MAS_MAX; i++) {
			rdtsc_new = RDTSC();
			mas[i]=rdtsc_new-rdtsc_old;
			rdtsc_old=rdtsc_new;
			if (mas[i]<0) i--;
		}
		for(i=10; i<MAS_MAX; i++) {
			tsc_avg+=mas[i];
		}	
		tsc_avg=tsc_avg>>15;
		for (i=1000; i<MAS_MAX; i++) {
			if(mas[i] < iter_tsc)
				iter_tsc = mas[i];
		}
		i=0;
		for (;rdtsc_new<time_stop;) {
			rdtsc_new = RDTSC();
			tsc=rdtsc_new-rdtsc_old;
			if (tsc>tsc_avg*2) {
				mas[i++]=rdtsc_new-rdtsc_start - tsc;
				mas_tsc[i]=rdtsc_new;
				rdtsc_start=rdtsc_new;
		}
		rdtsc_old=rdtsc_new;
		}
		long i_max = i; long RDTSC_min,RDTSC_now;
		RDTSC_min=(-1)*(RDTSC()-RDTSC()); 
		RDTSC_min=(-1)* RDTSC_min;
		for(i=1; i<i_max; i++) 
			Logging::printf("mas[%ld]= %lld \t\tmas_tsc[%ld]= %lld\n", i, mas[i], i, mas_tsc[i]);
		for(i=0; i<MAS_MAX_RT; i++) {
			RDTSC_now=(RDTSC() - RDTSC());
			RDTSC_now=(-1)*RDTSC_now;
			if(RDTSC_now<RDTSC_min) 
				RDTSC_min=RDTSC_now;
		}
		Logging::printf("RDTSC= %d\n", RDTSC_min);
		Logging::printf("AVG=%lld\n", tsc_avg);
		Logging::printf("ITER=%ld\n", iter_tsc);
		Logging::printf("%lld\n", sizeof(uint32));	
	}
};

ASMFUNCS(Hello, NovaProgram)	
	
