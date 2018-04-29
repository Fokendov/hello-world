#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

int main(){
	int m = 40;
	char n[200];
	
	m = 9;
	cidentify(n, m);
	printf("Com 9: %s", n);
	
	return 0;
}
