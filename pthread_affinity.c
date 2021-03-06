#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>   //cpu_set_t , CPU_SET
#include <pthread.h> //pthread_t
#include<unistd.h>  //sysconf

#include <pthread_affinity.h>


int 
pa_get_core_num()
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}


int 
pa_set_core(pthread_t thd, int core_id) 
{
	int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (core_id < 0 || core_id >= num_cores)
		return -1;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(core_id, &cpuset);
   
	return pthread_setaffinity_np(thd, sizeof(cpu_set_t), &cpuset);
}
