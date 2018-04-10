/* Arthur Böckmann Grossi (275607), Cassiano Translatti Furlani(278038) e Ian Fischer Schilling(#)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>


#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"

#define TRUE 1
#define FALSE 0

#define MAX_STU_CHAR 100
#define STU1 "Arthur Bockmann Grossi - 275000\n"
#define STU2 "Cassiano Translatti Furlani - 278038\n"
#define STU3 "Ian Fischer Schilling - 275001\n"

/*	GLOBAL VARIABLES */

//Queues
PFILA2 ready;
PFILA2 blocked;
PFILA2 suspendedReady;
PFILA2 suspendedBlocked;
PFILA2 waiting;

int systemReady = 0;
ucontext_t currentContext;
TCB_t *running = NULL; //Current running thread

/* STUCTURE */
typedef struct waitingThread{
	int waitingTID;
	int waitedTID;
} waitThread;

/* PROTOTYPES */

void* exitingThread();
TCB_t *getThread(int tid, PFILA2 queue);

int initialise()
{	//Initialise queues
	ready = calloc(1, sizeof(PFILA2);
	if(ready == NULL)
	{	
		puts("Error allocating memory for ready queue");
		return -1;
	}
	CreateFila2(ready);
	
	blocked = calloc(1, sizeof(PFILA2));
	if (blocked == NULL)
	{
		puts("Error allocating memory for blocked queue");
		return -1;
	}
	CreateFila2(blocked);
	
	suspendedReady = calloc(1, sizeof(PFILA2));
	if (suspendedReady == NULL)
	{
		puts("Error alloocating memory for suspended ready queue").
		return -1
	}
	CreateFila2(suspendedReady);
	
	suspendedBlocked = calloc(1, sizeof(PFILA2));
	if (suspendedBlocked == NULL)
	{
		puts("Error allocating memory for suspended blocked queue");
		return -1;
	}
	CreateFila2(suspendedBlocked);
	
	//Create a context for the thread
	getcontext(&currentContext);
	currentContext.uc_stack.ss_sp = malloc(SIGSTKSZ);
	currentContext.uc_stack.ss_size = SIGSTKSZ;
	makecontext(&currentContext, (void (*)())exitingThread, 0);
	
	//Create main thread
	TCB_t *mainThread = calloc(1, sizeof(TCB_t));
	mainThread->tid = 0;
	mainThread->state = PROCST_EXEC;
	mainThread->prio = 0;
	running = mainThread;
	
	systemReady = 1;
	return 0;
}

/*When a thread finishes, some thread(s) might be waiting
	this function changes the current running thread
*/
void* exitingThread(){
	waitThread *wThread;
	TCB_t *waitingThread, *nextThread;
	
	wThread = removeWaitingThread(running->tid);
	
	if(wThread != NULL)
	{//  A thread was waiting for this one to finish
		waitingThread = getThread(wThread->waiting_tid, blocked);
	}
	else
	{
		
	}
	
}

TCB_t* getThread(int tid, PFILA2 queue)
{
	TCB_t *thread;
	
	if ( (thread = calloc(1, sizeof(TCB_t))) )
	{
		FirstFila2(queue);
		do
		{
			thread = GetAtIteratorFila2(queue); // Look in queue for the waiting process
			if(thread->tid == tid)
				return thread;
			NextFila2(queue);
		}while(thread != NULL)
	}
	else
	{
		printf("Error at getThread: could not allocate memory.\n");
		return NULL;
	}
	
	printf("Could not find thread of id: %i\n", tid);
	return NULL;
		
}

/*
	Gets the next thread in the ready queue and deletes it
*/
TCB_t* dispatcher()
{
	TCB_t* nextThread = calloc(1, sizeof(TCB_t));
	
	FirstFila2(ready);
	if( GetAtIteratorFila2 != NULL)
		{
			nextThread = (TCB_*t)GetAtIteratorFila2(ready);
			DeleteAtIterator(ready);
			return nextThread;
		}
	return NULL;
}
/*
	Swap the context of the running thread and the parameter thread
*/
int runThread(TCB_t* thread)
{
	TCB_t* oldThread;
	oldThread = running;
	
	thread->state = PROCST_EXEC;
	running = thread;
	swapcontext( &(oldThread->context), &(thread->context) );
	return 0;
}

/*
	Searches in the wait queue if the thread with the parameter tid is already being waited by someone else
*/
waitThread* searchWaitQueue(int tid)
{	waitThread* wThread = calloc(1, sizeof(waitThread*));
	FirstFila2(waiting);
	
	while( GetAtIteratorFila2(waiting) != NULL)
	{
		wThread = GetAtIteratorFila2(waiting);
		if( wThread->waitedTid == tid ) //Found thread
			return wThread;
		NextFila2(waiting);
	}
	return NULL;
	
}
int insertNewWait(int waitingTID, int waitedTID)
{
	waitThread* wThread;
	
	if(wThread = calloc(1, sizeof(waitThread*)) != NULL)
	{
			wThread->waitingTID = waitingTID;
			wThread->waitedTID = waitedTID;
			return AppendFila2(waiting, wThread);
	}
	else
	{
		printf("Error at inseertNewWait could not allocate memory\n");
		return -1;
	}
}
/*----------------------------------------------------------------------------------
						MAIN FUNCTIONS
------------------------------------------------------------------------------------*/

/*		NOT FINISHED		*/
int ccreate(void *(*start)(void *), void *arg, int prio)
{
	if (firstThread)
	{
		if(initialise != 0)//Error initializing
		{
			puts("Error ccreate: could not initialise system\n");
			return -1;
		}		
		
		
	}
	
}

/*
	The threads give up the cpu for the next ready thread, putting itself on the ready queue
*/
int cyield()
{
	TCB_T *nextThread;
	
	running->state = PROCST_APTO;
	if( AppendFila2(ready, running) == 0)
	{
		//Calls scheduler
		nexthThread = dispatcher();
		if(nextThread != NULL)
		{
			runThread(nextThread);
			return 0;
		}
		else
		{
			puts("Error: could not dispatch next thread.");
			return -1;
		}
	}
	else
	{
		puts("Error: thread could not be inserted into ready queue");
		retun -1;
	}
}

/*		NOT FINISHED		*/
int cjoin(int tid)
{
	//Initialise yet to be implemented 
	
	TCB_t *nextThread = calloc(1, sizeof(TCB_t*)); 
	nextThread = dispatcher();
	
	if(nextThread != NULL)
	{
		if( searchWaitQueue(tid) == NULL) //No thread waiting for parameter tid
		{	
			if(insertNewWait(running->tid, tid) == 0)
			{
				//Block running thread
			
			
		
}

/* 		NOT FINISHED		*/
int cindentify(char *name, int size){
	char student[MAX_STU_CHAR] = "";
	int i;
	int st2 = strlen(STU1);
	int st3 = st2 + strlen(STU2);
	int letters;
	
	strcat(student, STU1);
	strcat(student, STU2);
	strcat(student, STU3);
	
	if(size >= MAX_STU_CHAR) //Size is equal to or greater than students names
	{
		for( i = 0; i < MAX_STU_CHAR;i++)
			name[i] = student[i];
		return 0;
	}
	else if( size >= 9) //At least 3 characters for each
	{
		letters = size/3 - 2;
		
		
	}
	else 
		return -1;
			
}


/*----------------------------------------------------------------------------------
						SEMAPHORE FUNCTIONS
------------------------------------------------------------------------------------*/


int csem_init(csem_t *sem, int count){

	sem->count = count;
	sem->fila = calloc(1, sizeof(PFILA2))

	if(CreateFila2(sem->fila)==0){
		return 0;
	}else{
		printf("Error trying to initialize semaphore's queue");
		return -1;
	}
}

int cwait(csem_t *sem){
	TCB_t* nextThread= calloc(1, sizeof(TCB_t*));
	
	if(sem->PFILA2!=NULL){
		if((sem->count)>0){
			sem->count--;
			return 0;
		}else if((sem->count)<=0){

			//block current thread
			running->state=PROCST_BLOQ;
			if(AppendFila2(blocked, running)){
				printf("Error: could not insert blocked thread in blocked queue");
				return -1;
			}

			//add thread to semaphore's queue
			if(AppendFila2(sem->fila, running)){
				printf("Error: could not insert blocked thread in semaphore's queue");
				return -1;
			}

			//run next thread
			nextThread = dispatcher();
			if(nextThread!=NULL){
				runThread(nextThread);
			}
		}
	}else{
		printf("Error: semaphore's queue was not initialized");
		return -1;
	}

	return 0;
}

int csignal(csem_t *sem){
	TCB_t* firstThread_sem= calloc(1, sizeof(TCB_t*));

	if(sem->PFILA2!=NULL){
		//checks if the queue is empty
		if(FirstFila2(sem->PFILA2)){
			//remove first thread from semaphore's queue
			firstThread_sem=(TCB_t*)GetAtIteratorFila2(sem->PFILA2);
			DeleteAtIteratorFila2(sem->PFILA2);

			//remove thread from blocked queue
			if(removefromqueue(firstThread_sem, blocked)){
				printf("Error: could not remove thread from blocked queue");
				return -1;
			}

			//and insert in ready queue;
			firstThread_sem->state=PROCST_APTO;
			if(AppendFila2(firstThread_sem, ready)){
				printf("Error: could not insert thread from semaphore's queue in ready queue");
				return -1
			}
		}
	sem->count++;
	}else{
		printf("Error: semaphore's queue was not initialized");
		return -1;
	}
	return 0;
}

int removefromqueue(TCB_t* thread, PFILA2 queue){
	TCB_t* iterator = calloc(1, sizeof(TCB_t*));

	FirstFila2(queue);
	while(GetAtIteratorFila2(queue)!=NULL){
		iterator = GetAtIteratorFila2(queue);
		if(thread->tid==iterator->tid){
			if(DeleteAtIteratorFila2(queue)){
				return -1;
			}
		}else{
			NextFila2(queue);
		}
	}
	return 0;
}