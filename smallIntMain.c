
#include "smallint.h"
#define VECTORSIZE 4
void printa_vetor (int *a, int n) {
	int i;
	printf ("\nPrintando dados do vetor de inteiros:\n");
	for (i=0; i<n; i++) {
		printf ("\nentrada %d \t valor: %d", i, a[i]);
	}	
}

int main (void) {
	int a[VECTORSIZE] = {4, 10, -20, 60};
	printa_vetor(a, VECTORSIZE);
	VetSmallInt v = vs_new(a);
	vs_print(v);
	return 0;
}