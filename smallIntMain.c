#include "smallint.h"
#define VECTORSIZE 4
void printa_vetor (int *a, int n) {
	int i;
	printf ("\nExibindo dados do vetor de inteiros:\n");
	for (i=0; i<n; i++) {
		printf ("\nentrada %d \t valor: %d\tbinario: ", i, a[i]);
		printInBinary(a[i]);
	}
	printf ("\n");
}

int main (void) {
	int a[VECTORSIZE] = {4, 20, -4, 50};
	VetSmallInt v;
	
	printa_vetor(a, VECTORSIZE);
	
	// teste de criação e print
	printf ("\n\n------TESTE DE CRIACAO------\n\n");
	v = vs_new(a);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SOMA------\n\n");
	v = vs_add(vs_new(a), vs_new(a));
	vs_print(v);
	
	printf ("\n\n------TESTE DE SHIFT PARA ESQUERDA------\n\n");
	v = vs_shl(vs_new(a), 1);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SHIFT ARITMETICO------\n\n");
	v = vs_sar(vs_new(a), 1);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SHIFT LOGICO------\n\n");
	v = vs_shr(vs_new(a), 1);
	vs_print(v);
		
	return 0;
}