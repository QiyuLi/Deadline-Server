#ifndef PTHREAD_AFFINITY
#define PTHREAD_AFFINITY

#include <pthread.h>

int pa_set_core(pthread_t thd, int core_id);

int pa_get_core_num();


#endif
