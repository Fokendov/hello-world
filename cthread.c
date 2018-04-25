/* Arthur Böckmann Grossi (275607), Cassiano Translatti Furlani(278038) e Ian Fischer Schilling(275603)
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
#define STU1 "Arthur Bockmann Grossi - 275607"
#define STU2 "Cassiano Translatti Furlani - 278038"
#define STU3 "Ian Fischer Schilling - 275603"

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
typedef struct waitingThread
{
	int waitingTID;
	int waitedTID;
} waitThread;

/* PROTOTYPES */

void* exitingThread();
TCB_t *getThread(int tid, PFILA2 queue);
waitThread* searchWaitQueue(int tid);

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

/*When a thread finishes, a thread might be waiting
	this function changes the current running thread
*/
void* exitingThread(){
	waitThread *wThread;
	TCB_t *waitingThread, *nextThread;
	
	wThread = searchWaitQueue(running->tid);//Found a pair of waiting and waited thread
	
	if(wThread != NULL)
	{//  A thread was waiting for this one to finish
		DeleteAtIterator(waiting); // Removes waiting thread from the waiting line
		waitingThread = getThread(wThread->waiting_tid, blocked);
		waitingThread->state = PROCST_APTO;
		AppendFila2(ready, waitingThread);
	}
	//No thread waiting, calls next ready thread
	nextThread = dispatcher();
	if(nextThread != NULL)
		runThread(nextThread);
	else
		printf("Error: could not dispatch thread\n");
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
			{
				DeleteAtIterator(queue);
				return thread;
			}
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
	if( GetAtIteratorFila2(ready) != NULL)
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
{	
	waitThread* wThread = calloc(1, sizeof(waitThread*));
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
		printf("Error at insertNewWait could not allocate memory\n");
		return -1;
	}
}
/*----------------------------------------------------------------------------------
						MAIN FUNCTIONS
------------------------------------------------------------------------------------*/

int ccreate(void *(*start)(void *), void *arg, int prio)
{
	if (!systemReady)
	{
		if(initialise != 0)//Error initializing
		{
			puts("Error ccreate: could not initialise system\n");
			return -1;
		}		
	}
	TCB_t *newThread;
	int newTid = Random2();
	
	if( (newThread = calloc(1, sizeof(TCB_t*) )
	{
		//Set new thread attributes
		newThread->tid = newTid;
		newThread->state = PROCST_APTO;
		newThread->prio = 0;
		
		getcontext(&(newThread->context));
		newThread->context.uc_link = &exitContext;
		newThread->context.uc_stack.ss_sp = malloc(SIGSTKSZ);
		newThread->context.uc_stack.ss_size = SIGSTKSZ;
		makecontext(&(newThread->context), (void (*)())start, 1, arg);
		
		if( AppendFila2(ready, newThread) == 0 )
			return tid;
		else
		{
			printf("Error: could not insert new Thread into ready queue\n");
			return -1;
		}
		
	else
	{
		printf("Error: could not allocate memory for creating thread\n");
		return -1;
	}
	
}

/*
	The threads give up the cpu for the next ready thread, putting itself on the ready queue
*/
int cyield()
{
	if (!systemReady)
	{
		if(initialise != 0)//Error initializing
		{
			puts("Error cyield: could not initialise system\n");
			return -1;
		}		
	}

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


int cjoin(int tid)
{
	//Check if the system is ready
	
	if (!systemReady)
	{
		if (initialise() != 0)
		{
			printf("Error cjoin: could not init system");
			return -1;
		}
	}	
	
	TCB_t *nextThread = calloc(1, sizeof(TCB_t*)); 
	nextThread = dispatcher();//Gets next thread in the ready queue
	
	if(nextThread != NULL)//there is a thread ready
	{
		if( searchWaitQueue(tid) == NULL) //No thread waiting for parameter tid
		{	
			if(insertNewWait(running->tid, tid) == 0) //Successful
			{
				//Blocks current thread
				running->state = PROCST_BLOQ;
				AppendFila2(blocked, running);
				
				runThread(nextThread);
				return 0;
			}
			else if (running == NULL)
			{
				runThread(nextThread);
				return 0;
			}
			else
			{
				printf("Error: could not insert waiter Thread\n");
				return -1;
			}
		}
		else
		{
			printf("Error: Thread was already joined by another\n");
			return -1;
		}
	}
	else //No thread available
	{
		printf("Error: Could not find thread to join\n");
		return -1;
	}
}

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
	else if( size >= 9) //At least 1 characters for each
	{
		letters = size/3 - 2;
		for(i = 0; i < letters; i++)
		{
			name[i] = student[i];
			
			name[i + letters + 1] = student[i + st2]
			
			name[i + 2*(letters + 1)] = student[i + st3];
		}
		
		name[letters] = '\n';
		name[2*(letters) +1]= '\n';
		name[i + 2*(letters + 1) + 1] = '\0';
	}
	else 
		return -1;
			
}

/*	SEMAPHORE        */
	     

int csuspend(int tid){
	TCB_t* thread = calloc(1, sizeof(TCB_t*));

	thread=removefromqueue(tid, ready);
	
	if(thread != NULL){
		//thread was in ready queue
		
		if(AppendFila2(suspendedReady, thread)){
			printf("Error: could not insert thread in suspendedReady queue");
			return -1;
		}else{
			return 0;
		}
	}
	
	thread=removefromqueue(tid, blocked);
	if(thread!=NULL){
		//thread was in blocked queue

		if(AppendFila2(suspendedBlocked, thread)){
			printf("Error: could not insert thread in suspendedBlocked queue");
			return -1;
		}else{
			return 0;
		}
	}
	
	printf("Error: could not find given tid in ready or blocked queue");
	return -1;
}

int cresume(int tid){
	TCB_t* thread = calloc(1, sizeof(TCB_t*));

	thread=removefromqueue(tid, suspendedReady);
	
	if(thread != NULL){
		//thread was in suspendedReady queue
		
		if(AppendFila2(ready, thread)){
			printf("Error: could not insert thread in ready queue");
			return -1;
		}else{
			return 0;
		}
	}
	
	thread = removefromqueue(tid, suspendedBlocked);
	if(thread != NULL){
		//thread was in suspendedBlocked queue

		if(AppendFila2(blocked, thread)){
			printf("Error: could not insert thread in blocked queue");
			return -1;
		}else{
			return 0;
		}
	}
	
	printf("Error: could not find given tid in suspendedReady or suspendedBlocked queue");
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
			if(removefromqueue(firstThread_sem->tid, blocked)==NULL){
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



/*----------------------------------------------------------------------------------
						AUXILIAR FUNCTIONS
------------------------------------------------------------------------------------*/
//remove thread with the given tid from the given queue
//returns pointer to deleted tcb if deleted successfully
//returns NULL if error
TCB_T* removefromqueue(int tid, PFILA2 queue){
	TCB_t* iterator = calloc(1, sizeof(TCB_t*));

	FirstFila2(queue);
	while(GetAtIteratorFila2(queue)!=NULL){
		iterator = GetAtIteratorFila2(queue);
		if(tid==iterator->tid){
			if(DeleteAtIteratorFila2(queue)==0){
				return iterator;
			}else{
				return NULL;
			}
		}else{
			NextFila2(queue);
		}
	}
	return NULL;
}


