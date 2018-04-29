#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

/*
	Teste das funções ccreate, cyield e cjoin
*/
void* func0(void *i){
	int j;
	printf("Eu sou a id0, comecei e já vou ceder\n");
	for(j = 0; j < 3; j++){
		printf("%i\n",j);
		cyield();
		printf("Aqui é a id0 voltando\n");
	}
}

void* func(void *i){
	printf("Imprimindo i: %i\n", *((int *)i));
}

void* func1(void *i){
	printf("Eu sou a id3 me juntando com a id0 : %i\n", *((int *)i) );
	cjoin( *((int *)i) );
	printf("Ei, a id0 acabou\n");
}
int main(){
	int id0, id1, id2, id3;
	int i = 0;
	
	id0 = ccreate(func0,(void *)&i ,0);
	i++;
	id1 = ccreate(func,(void *)&i ,0);
	i++;
	id2 = ccreate(func,(void *)&i ,0);
	id3 = ccreate(func1,(void *)&id0 ,0);
	
	printf("Main fazendo cjoin com id2\n");
	cjoin(id2);
	printf("id2: %i terminou\n", id2);
	cjoin(id0);
	
	printf("Main fazendo cyield\n");
	cyield();
	printf("Mais duas para a id3 rodar\n");
	cyield();
	cyield();
	printf("Main Terminando\n");
	
	return 0;
}
