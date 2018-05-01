#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

/*
	Teste das funções cresume, csuspend
*/
void *func0(void *x){
	printf("Eu sou a thread 0 \n");
	cyield();
	printf("Thread 0 voltou e acaba\n");
	return 0;
}

void *func1(void *x){
	printf("Eu sou a thread 1 e mostro a id0: %i\n", *( (int *)x) );
	printf("Vou esperar pela thread 0\n");
	cjoin( *( (int *)x) );
	printf("Thread 1 voltou e acaba\n");
	return 0;
}

void *func2(void *x){
	printf("Eu sou a thread 2 e mostro a id1: %i\n", *( (int *)x) );
	printf("Vou suspender a thread 1\n");
	csuspend( *( (int *)x) );
	printf("Thread 2 voltou e acaba\n");
	return 0;
}
int main(){
	int id0, id1, id2;
	int a = 1;
	id0 = ccreate(func0, (void *)&a, 0);
	id1 = ccreate(func1, (void *)&id0, 0);
	id2 = ccreate(func2, (void *)&id1, 0);
	printf("Main juntou\n");
	cjoin(id2);
	printf("ID2 acabou, vou desbloquear id1\n");
	cresume(id1);
	cyield();
	printf("Terminando a main\n");
	
	

	return 0;
}
