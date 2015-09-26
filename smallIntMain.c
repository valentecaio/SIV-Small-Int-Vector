#include "smallint.h"
#include <stdlib.h>
#define VECTORSIZE 4
#define TOTALSIZE 32

void printIntInBinary (int n) {
	int i;
	for (i=TOTALSIZE; i>=0; i--){
		printf("%d", (n >> i) & 1);
		if (!( i%4 ))
			printf (" ");
	}
}

void printa_vetor (int *a, int n) {
	int i;
	printf ("\nExibindo dados do vetor de inteiros:\n");
	for (i=0; i<n; i++) {
		printf ("\nindice %d | decimal: %d\t| binario: ", i, a[i]);
		printIntInBinary(a[i]);
	}
	printf ("\n");
}

int main (void) {
	int a[VECTORSIZE] = {4, 20, -4, 50};
	VetSmallInt v;
	FILE *f;
	printa_vetor(a, VECTORSIZE);
	
	printf ("\n\n------TESTE DE CRIACAO------\n");
	v = vs_new(a);
	printf ("\nvetor criado:\t\t");
	vs_printInBinary(v);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SOMA------\n");
	v = vs_new(a);
	printf ("\nvetor 1:\t");
	vs_printInBinary(v);
	printf ("\nvetor 2:\t");
	vs_printInBinary(v);
	v = vs_add(v, v);
	printf ("\nvetor somado:\t");
	vs_printInBinary(v);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SHIFT PARA ESQUERDA------\n");
	v = vs_new(a);
	printf ("\nvetor passado:\t");
	vs_printInBinary(v);
	v = vs_shl(v, 1);
	printf ("\nvetor shiftado:\t");
	vs_printInBinary(v);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SHIFT ARITMETICO------\n");
	v = vs_new(a);
	printf ("\nvetor passado:\t");
	vs_printInBinary(v);
	v = vs_sar(v, 1);
	printf ("\nvetor shiftado:\t");
	vs_printInBinary(v);
	vs_print(v);
	
	printf ("\n\n------TESTE DE SHIFT LOGICO------\n");
	v = vs_new(a);
	printf ("\nvetor passado:\t");
	vs_printInBinary(v);
	v = vs_shr(v, 1);
	printf ("\nvetor shiftado:\t");
	vs_printInBinary(v);
	vs_print(v);
	
	printf ("\n\n------TESTE DE WRITE------\n");
	f = fopen ("writeTest.txt", "w");
	v = vs_new(a);
	if (vs_write(v, f) == 0) {
		printf ("\nWrite funcionou!");
	} else {
		printf ("\nWrite NAO funcionou!");
	}
	printf ("\nvetor escrito:\t");
	vs_printInBinary(v);
	vs_print(v);
	fclose(f);
	
	printf ("\n\n------TESTE DE WRITE BINARIO------\n");
	f = fopen ("writeInBinaryTest.txt", "w");
	v = vs_new(a);
	if (vs_writeInBinary(v, f) == 0) {
		printf ("\nWrite binario funcionou!");
	} else {
		printf ("\nWrite binario NAO funcionou!");
	}
	printf ("\nvetor escrito:\t");
	vs_printInBinary(v);
	vs_print(v);
	fclose(f);
	
	/*
	printf ("\n\n------TESTE DE READ------\n");
	f = fopen ("readTest.txt", "r");
	v = vs_shr(vs_new(a), 1);
	vs_print(v);
	*/
	return 0;
}