
int csuspend(int tid){
	TCB_t* thread = calloc(1, sizeof(TCB_t*));

	thread=removefromqueue(tid, ready);
	
	if(thread=!=NULL){
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
			printf("Error: could not insert thread in suspendedReady queue");
			return -1;
		}else{
			return 0;
		}
	}
	
	printf("Error: could not find given tid in ready or blocked queue");
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