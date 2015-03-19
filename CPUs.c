#include <stdio.h>
#include "CPUs.h"
#include "processQueue.h"



//=======================================================================
//--------------First Come First Serve-------------------*works----------
//=======================================================================
void* FCFScpu (void* param){

  sharedVars* vars = ((cpuParams*)param)->svars;
  int threadNumber = ((cpuParams*)param)->threadNumber;
  process* p = NULL; //Nik said to use a helper process to navigate things



  while(1){

    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      
      if((vars->readyQ).head !=NULL) {
	int leastArrival = 9999; //initialized bigger than anything foreseeable
	int index = 0; //gets reset anyways
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if((traverse->data->arrivalTime) < (leastArrival)){  
	    
	    leastArrival = ((traverse->data->arrivalTime));
	    p = traverse->data;
	    lowestIndex = index;
	  
	  }//if((traverse->data

	  if(traverse->next != NULL){
	    traverse = traverse->next;
	  }else{
	    traverse = NULL;
	  }//if(traverse->
	  
	  index++;

	}//while(traverse!=Null)

	p = Qremove((&(vars->readyQ)),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));
	
      }//if((vars->readyQ)

      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section


  }

    if(p !=NULL){
      p->burstRemaining = p->burstRemaining -1; //simulates running process
      
      if(p->burstRemaining ==0){
	pthread_mutex_lock(&(vars->finishedQLock));
	Qinsert(&(vars->finishedQ),p); //add process to finished Queue
	  pthread_mutex_unlock(&(vars->finishedQLock));

	p = NULL;
      }//if(p->burstRem

    }//if(p!=NULL)
    
    sem_post(&(vars->mainSem)); //sig main thread
  }

}



//=======================================================================
//----------------------Shortest Job First----------*works---------------
//=======================================================================




void* SJFcpu (void* param){

 sharedVars* vars = ((cpuParams*)param)->svars;
  int threadNumber = ((cpuParams*)param)->threadNumber;
  process* p = NULL; //Nik said to use a helper process to navigate things



  while(1){

    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      
      if((vars->readyQ).head !=NULL) {
	int lowBurst = 9999;
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if((traverse->data->burstTotal) < (lowBurst)){  
	    
	    lowBurst = ((traverse->data->burstTotal));
	    p = traverse->data;
	    lowestIndex = index;
	  
	  }//if((traverse->data

	  if(traverse->next != NULL){
	    traverse = traverse->next;
	  }else{
	    traverse = NULL;
	  }//if(traverse->
	  
	  index++;

	}//while(traverse!=Null)

	p = Qremove((&(vars->readyQ)),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));
	
      }//if((vars->readyQ)

      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section


  }

    if(p !=NULL){
      p->burstRemaining = p->burstRemaining -1; //simulates running process
      
      if(p->burstRemaining ==0){
	pthread_mutex_lock(&(vars->finishedQLock));
	Qinsert(&(vars->finishedQ),p); //add process to finished Queue
	  pthread_mutex_unlock(&(vars->finishedQLock));

	p = NULL;
      }//if(p->burstRem

    }//if(p!=NULL)
    
    sem_post(&(vars->mainSem)); //sig main thread
  }

}


//=======================================================================
//------------------Non-Preemptive Priority------------*works------------
//=======================================================================



void* NPPcpu (void* param){
  sharedVars* vars = ((cpuParams*)param)->svars;
  int threadNumber = ((cpuParams*)param)->threadNumber;
  process* p = NULL; //Nik said to use a helper process to navigate things





  while(1){

    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      
      if((vars->readyQ).head !=NULL) {
	int lowPriority = 9999; 
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if((traverse->data->priority) < (lowPriority)){  
	    
	    lowPriority = ((traverse->data->priority));
	    p = traverse->data;
	    lowestIndex = index;
	  
	  }//if((traverse->data

	  if(traverse->next != NULL){
	    traverse = traverse->next;
	  }else{
	    traverse = NULL;
	  }//if(traverse->
	  
	  index++;

	}//while(traverse!=Null)

	p = Qremove((&(vars->readyQ)),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));
	
      }//if((vars->readyQ)

      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section


  }

    if(p !=NULL){
      p->burstRemaining = p->burstRemaining -1; //simulates running process
      
      if(p->burstRemaining ==0){
	pthread_mutex_lock(&(vars->finishedQLock));
	Qinsert(&(vars->finishedQ),p); //add process to finished Queue
	pthread_mutex_unlock(&(vars->finishedQLock));

	p = NULL;
      }//if(p->burstRem

    }//if(p!=NULL)
    
    //(used for verification) printf("timestep\n");
    sem_post(&(vars->mainSem)); //sig main thread
  }


}


//=======================================================================
//----------Round Robin-------------------*works-------------------------
//=======================================================================



void* RRcpu (void* param){

    sharedVars* vars = ((cpuParams*)param)->svars;
  int threadNumber = ((cpuParams*)param)->threadNumber;
  process* p = NULL; //Nik said to use a helper process to navigate things

  int quantum = ((cpuParams*)param)->svars->quantum;
  int roundRobin = 0;



  while(1){

    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      
      if((vars->readyQ).head !=NULL) {
	int lowArrivalTime = 9999;
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if((traverse->data->arrivalTime) < (lowArrivalTime)){  
	    
	    lowArrivalTime = ((traverse->data->arrivalTime));
	    p = traverse->data;
	    lowestIndex = index;
	  
	  }//if((traverse->data

	  if(traverse->next != NULL){
	    traverse = traverse->next;
	  }else{
	    traverse = NULL;
	  }//if(traverse->
	  
	  index++;

	}//while(traverse!=Null)

	p = Qremove((&(vars->readyQ)),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));
	
      }//if((vars->readyQ)

      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section


  }

    if(p !=NULL){

      roundRobin++;
      p->burstRemaining = p->burstRemaining -1; //simulates running process
      
      if(p->burstRemaining ==0){
	pthread_mutex_lock(&(vars->finishedQLock));
	Qinsert(&(vars->finishedQ),p); //add process to finished Queue
	  pthread_mutex_unlock(&(vars->finishedQLock));

	p = NULL;
	roundRobin = 0;
      }//if(p->burstRem

    }//if(p!=NULL)

    if(roundRobin == quantum && p!=NULL){
      p->requeued = true;
      pthread_mutex_lock(&(vars->readyQLock));
      Qinsert(&(vars->readyQ),p);
      pthread_mutex_unlock(&(vars->readyQLock));

      p = NULL;
      roundRobin = 0;
    }
    //(used for verification)  printf("timestamp\n");    
    sem_post(&(vars->mainSem)); //sig main thread
  }
}



//========================================================
//--------Shortest Remaining Time First------*works-------
//========================================================



void* SRTFcpu (void* param){

 sharedVars* vars = ((cpuParams*)param)->svars;
  int threadNumber = ((cpuParams*)param)->threadNumber;
  process* p = NULL; //Nik said to use a helper process to navigate things



  while(1){

    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      
      if((vars->readyQ).head !=NULL) {
	int lowBurstRemaining = 9999;
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if((traverse->data->burstTotal) < (lowBurstRemaining)){  
	    
	    lowBurstRemaining = ((traverse->data->burstTotal));
	    p = traverse->data;
	    lowestIndex = index;
	  
	  }//if((traverse->data

	  if(traverse->next != NULL){
	    traverse = traverse->next;
	  }else{
	    traverse = NULL;
	  }//if(traverse->
	  
	  index++;

	}//while(traverse!=Null)

	p = Qremove((&(vars->readyQ)),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));
	
      }//if((vars->readyQ)

      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section
    }
      if(p!=NULL){
	p->burstRemaining = p->burstRemaining-1;
	if(p->burstRemaining==0){
	  pthread_mutex_lock(&(vars->finishedQLock));
	  Qinsert(&(vars->finishedQ),p);
	  pthread_mutex_unlock(&(vars->finishedQLock));
	  p=NULL;
	}//if(p->burstRem
      }//if(p!=NULL)


    
     
    pthread_mutex_lock(&(vars->readyQLock));

    if((vars->readyQ).head !=NULL && p!=NULL){
      if(((vars->readyQ).head->data->burstRemaining) < (p->burstRemaining)){
	p->requeued = true;
	Qinsert(&(vars->readyQ),p);
	p = NULL;
      }//if(((vars->readyQ).head->data

    }//if((vars->readyQ

    pthread_mutex_unlock(&(vars->readyQLock));
    sem_post(&(vars->mainSem)); //sig main thread
  }

}




//========================================================
//------Priority Preemtive-------*almost works------------
//========================================================



void* PPcpu (void* param){

 sharedVars* vars = ((cpuParams*)param)->svars;
  int threadNumber = ((cpuParams*)param)->threadNumber;
  process* p = NULL; //Nik said to use a helper process to navigate things



  while(1){

    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      
      if((vars->readyQ).head !=NULL) {
	int lowPriority = 9999;
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if((traverse->data->priority) < (lowPriority)){  
	    
	    lowPriority = ((traverse->data->priority));
	    p = traverse->data;
	    lowestIndex = index;
	  
	  }//if((traverse->data

	  if(traverse->next != NULL){
	    traverse = traverse->next;
	  }else{
	    traverse = NULL;
	  }//if(traverse->
	  
	  index++;

	}//while(traverse!=Null)

	p = Qremove((&(vars->readyQ)),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));
	
      }//if((vars->readyQ)

      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section

    }
      if(p!=NULL){
	p->burstRemaining--;
	if(p->burstRemaining==0){
	  pthread_mutex_lock(&(vars->finishedQLock));
	  Qinsert(&(vars->finishedQ),p);
	  pthread_mutex_unlock(&(vars->finishedQLock));
	  p=NULL;
	}//if(p->burstRem
      }//if(p!=NULL)


  

     
    pthread_mutex_lock(&(vars->readyQLock));

    //TODO check entire list and not just head for lowest priority
    if((vars->readyQ).head !=NULL && p!=NULL){
      if(((vars->readyQ).head->data->priority) < (p->priority)){
	p->requeued = true;
	Qinsert(&(vars->readyQ),p);
	p = NULL;
      }//if(((vars->readyQ).head->data

    }//if((vars->readyQ

    pthread_mutex_unlock(&(vars->readyQLock));
    sem_post(&(vars->mainSem)); //sig main thread
  }
}

