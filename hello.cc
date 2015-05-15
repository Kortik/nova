/*
 * Test application that burns CPU time.
 *
 * Copyright (C) 2010, Bernhard Kauer <bk@vmmon.org>
 * Economic rights: Technische Universitaet Dresden (Germany)
 *
 * This file is part of Vancouver.nova.
 *
 * Vancouver.nova is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * Vancouver.nova is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#include "sigma0/console.h"
#include "nul/program.h"
#include "service/helper.h"

class Hello : public NovaProgram, public ProgramConsole
{
 
static __inline__ unsigned long long RDTSC(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
//static long long int nn=0;
typedef struct note
{
	struct note * prevptr, *nextptr;
	long long value, num;
} Note;
typedef Note * Noteptr; 	
void input(Noteptr *root, long long value)
{
	Noteptr cur, prev=NULL;
	//Noteptr newn=malloc(sizeof(*newn));  //p = malloc(sizeof(*p))
	Noteptr newn = new Note;
	
	newn->value=value;
	newn->prevptr=NULL;
	newn->nextptr=NULL;

	if ( *root == NULL)
	{
		*root=newn;
		//printf("test %lld", value);
	}
	else 
	{
		cur = *root;
		while(cur!=NULL)
		{
			prev = cur;
			cur = cur->nextptr;
		}
		prev->nextptr = newn;
		newn->prevptr=prev;
	//printf("test2 %lld->%lld\n", value, newn->prevptr->value);

	}
}
void print(Noteptr root)
{
	Noteptr prev = NULL;
	Noteptr cur = root;

	if(root == NULL)
	{
		Logging::printf("> null list!");
		return;
	}

	while(cur!=NULL)
	{
		
		//Logging::printf("Rdtsc[%lld]=%lld\n ||||| 2-1=%lld ",cur->num,cur->value, cur->prevptr->value); //, cur->value-cur->prevptr->value  2-1=%lld
		prev = cur;
		cur = cur->nextptr;
	Logging::printf("Rdtsc=%lld ||||| cur->prevptr->value-cur->value=%lld \n ",cur->prevptr->value,cur->value-cur->prevptr->value );
	
	}
	return;

}


  void run(Utcb *utcb, Hip *hip)
  {
	long long time_start=RDTSC();
	long long proc_power=1.7*1000000000;
	long long time_stop=time_start+proc_power;
	long long tmp=time_start;
    init(hip);
    console_init("Hello", new Semaphore(alloc_cap(), true));
    	Logging::printf("%lld time stop!\n", time_stop);
	Noteptr root=NULL;
    for (int u=0;u<=1000;u++) {
	input(&root,RDTSC());
	//if (root==NULL){ printf("error"); break;} else printf ("%d", root);	RDTSC()<=time_stop
	//if (mas[i]>tsc_max) tsc_max=mas[i];
	//if (RDTSC()>=time_stop) break;
	}
     print(root); }
};


ASMFUNCS(Hello, NovaProgram)
