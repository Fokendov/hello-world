#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

/*
	Teste das funções do semafrodo OO
*/

void func(csem_t *sem){

	printf("primeira thread executando\n");
	if(cwait(sem)==0)
		printf("primeira thread alocando recurso gerenciado pelo semaforo\n");

	printf("primeira thread liberando o processador\n");
	cyield();
	
	printf("primeira thread voltando\n");
	if(csignal(sem)==0)
		printf("primeira thread liberando recurso gerenciado pelo semaforo\n");
	
	printf("primeira thread acabando \n");
	return 0;
}

void func1(csem_t *sem){

	printf("segunda thread executando\n");
	
	printf("segunda thread alocando recurso gerenciado pelo semaforo\n");	
	cwait(sem);
		
	
	printf("segunda thread voltando da fila de bloqueados do semaforo\n");
	if(csignal(sem)==0)
		printf("segunda thread liberando recurso gerenciado pelo semaforo\n");
	
	printf("segunda thread acabando \n");
	return 0;
}

void func2(int *i){
	int j;

	for (j=0;j<2;j++){
	printf("terceira thread no loop : %d\n", j);
	cyield();
	printf("terceira thread voltando a executar\n");
	}
	return 0;
}


int main(){
	csem_t semaf;
	int id0, id1, id2;
	int count = 1;

	printf("Main inicializando o semaforo\n");
	if(csem_init(&semaf, count)==0)
		printf("Semaforo inicializado\n");
	else
		printf("Erro ao inicializar o semaforo\n");

	id0 = ccreate(func, (void *)&semaf, 0);
	printf("primeira thread criada\n");
	id1 = ccreate(func1, (void *)&semaf, 0);
	printf("segunda thread criada\n");
	id2 = ccreate(func2, (void *)&count, 0);
	printf("terceira thread criada\n");


	printf("main se juntando a terceira thread\n");	
	cjoin(id2);

	return 0;
}
