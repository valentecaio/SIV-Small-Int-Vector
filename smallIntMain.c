
#include "smallint.h"
#define VECTORSIZE 4
void printa_vetor (int *a, int n) {
	int i;
	printf ("\nExibindo dados do vetor de inteiros:\n");
	for (i=0; i<n; i++) {
		printf ("\nentrada %d \t valor: %d", i, a[i]);
	}
	printf ("\n");
}

int main (void) {
	int a[VECTORSIZE] = {4, 20, -30, 60};
	int b[VECTORSIZE] = {4, 20, -30, 60};
	VetSmallInt v;
	
	printa_vetor(a, VECTORSIZE);
	printa_vetor(b, VECTORSIZE);
	
	// teste de criação e print
	printf ("\n\n------TESTE DE CRIACAO------\n\n");
	v = vs_new(a);
	vs_print(v);
	v = vs_new(b);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SOMA------\n\n");
	v = vs_add(vs_new(a), vs_new(b));
	vs_print(v);
	
	return 0;
}