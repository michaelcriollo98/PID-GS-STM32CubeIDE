#include <stdio.h>
#include <stdlib.h>
#include "random_numbers.h"


// numero aleatorio tipo float entre 0.0000 y 0.99999
float randf(void){
	float numrand = (float) (rand() % (10000+1))/10000;
	return numrand;
}

// numero enetero aleatorio dado un inicio y final incluidos.
int randi(int inicio,int fin){
	int numrand = rand () % (fin-inicio+1) + inicio;
	return numrand;
}

