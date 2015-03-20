#include <stdio.h>
#include "CPUs.h"
#include "processQueue.h"

/*DOCUMENTATION STATEMENT: 
C1C Nikolas Taromina Explained how I needed to structure the first algorithm.
I then used this as a template for the remainder of the algorithms. C1C John Miller 
explained how to add preemption and helped me test my code. He stated I should 
add print statements in certain areas to verify functionality. This in turn 
allows me to logically look at my code and see if it is working. 
*/

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
	int lowestArrival = 9999; //initialized bigger than anything foreseeable
	int index = 0; //gets reset anyways
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if(traverse->data->arrivalTime < lowestArrival){  
	    
	    lowestArrival = traverse->data->arrivalTime;
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

	p = Qremove(&(vars->readyQ),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", p->PID);
	
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
	int lowestBurst = 9999;
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if(traverse->data->burstTotal < lowestBurst){  
	    
	    lowestBurst = traverse->data->burstTotal;
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

	p = Qremove(&(vars->readyQ),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", p->PID);
	
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
	int lowestPriority = 9999; 
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if(traverse->data->priority < lowestPriority){  
	    
	    lowestPriority = traverse->data->priority;
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

	p = Qremove(&(vars->readyQ),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", p->PID);
	
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
	  
	  if(traverse->data->arrivalTime < lowArrivalTime){  
	    
	    lowArrivalTime = traverse->data->arrivalTime;
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

	p = Qremove(&(vars->readyQ),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", p->PID);
	
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
	int lowestBurstRemaining = 9999;
	int index = 0;
	int lowestIndex = 0;

	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;

	while(traverse != NULL){
	  
	  if(traverse->data->burstTotal < lowestBurstRemaining){  
	    
	    lowestBurstRemaining = traverse->data->burstTotal;
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

	p = Qremove(&(vars->readyQ),lowestIndex); //remove process from ready queue
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
      if((vars->readyQ).head->data->burstRemaining < p->burstRemaining){
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

  int lowestPriority = 9999;
  while(1){
    sem_wait(&(vars->cpuSems[threadNumber])); // waits for the semephore to open
    //only goes if there is no process yet
    if(p==NULL){
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section
      if((vars->readyQ).head !=NULL) { //only searches if something is in the ready Q
	//int lowestPriority = 9999;
	int index = 0;
	int lowestIndex = 0;
	node* traverse = (vars->readyQ).head; //helper node to traverse
	p = (vars->readyQ).head->data;
	while(traverse != NULL){ //searches entire Q 	  
	  if(traverse->data->priority < lowestPriority){ //gets lowest priority and sets it 	    
	    lowestPriority = traverse->data->priority;
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
	p = Qremove(&(vars->readyQ),lowestIndex); //remove process from ready queue
	printf("Process %d has been scheduled\n", (p->PID));	
      }//if((vars->readyQ)
      pthread_mutex_unlock(&(vars->readyQLock)); //exit critical section
    }//if(P==NULL)

    //only gues if there is a process running
      if(p!=NULL){
	p->burstRemaining--;
	//if process is finished
	if(p->burstRemaining==0){
	  pthread_mutex_lock(&(vars->finishedQLock));
	  Qinsert(&(vars->finishedQ),p);
	  pthread_mutex_unlock(&(vars->finishedQLock));
	  p=NULL;//this clears whichever process I'm working on
	}//if(p->burstRem


      }//if(p!=NULL)     
      pthread_mutex_lock(&(vars->readyQLock)); //enter critical section   
    //============START WORKING SECTION========================
	if((vars->readyQ).head !=NULL && p!=NULL){//makes sure ready Q is not empty so it can check for lower priority
	
	 
	  node* traverse = (vars->readyQ).head; //helper node to traverse
	  int requeued_flag = 0;
	  while(traverse->next !=NULL && p!=NULL){
	    if(traverse->data->priority < p->priority){
	   p->requeued = true;
	   printf("I was requeued\n");
	   Qinsert(&(vars->readyQ),p);
	   requeued_flag = 1;
	   p =traverse->data;
	 
          }//if(((vars->readyQ).head->data
	    
	    traverse=traverse->next;
	  }//while(traverse->next
	    if(requeued_flag = 1){
	       p=NULL;
	    }//if(requeued
        }//if((vars->readyQ
	//	p=NULL;
  
    //==================END WORKING SECTION===================
    
    pthread_mutex_unlock(&(vars->readyQLock));
    printf("time burst\n");
    sem_post(&(vars->mainSem)); //sig main thread  
  }//while(1)
}//VoidPPcpu

