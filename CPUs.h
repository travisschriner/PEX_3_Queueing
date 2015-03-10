#include <pthread.h>
#include <semaphore.h>
#include "processQueue.h"

#ifndef CPUsDotHStructs
#define CPUsDotHStructs
typedef struct{
	pthread_mutex_t readyQLock;
	pthread_mutex_t finishedQLock;
	sem_t* cpuSems;
	sem_t mainSem;
	queue readyQ;
	queue finishedQ;
	int quantum;
} sharedVars;

typedef struct{
	int threadNumber;
	sharedVars* svars;
} cpuParams;
#endif

void* FCFScpu (void* param);
void* SJFcpu (void* param);
void* NPPcpu (void* param);
void* RRcpu (void* param);
void* SRTFcpu (void* param);
void* PPcpu (void* param);
