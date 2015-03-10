#include <stdbool.h>

#ifndef processQueueDotHStructs
#define processQueueDotHStructs
typedef struct {
   int PID;
	int arrivalTime;
   int priority;
   int burstTotal;
   int burstRemaining;
   int initialWait;
   int totalWait;
   bool requeued;
} process;

typedef struct s_node{
   process* data;
   struct s_node* prev;
   struct s_node* next;
} node;

typedef struct{
   node* head;
   node* tail;
} queue;

#endif

void initQueue(queue* Q);
void Qinsert(queue* Q, process* proc);
process* Qremove(queue* Q, int which);
void Qprint(queue Q);
void incrementWaitTimes(queue* Q);
void processPrint(process proc);
void Qsort(queue* Q);
