 /* Caio Valente 1313402 3WB */
 /* Ilana Segal 1510329 3WB */

#include "smallint.h"
#include <stdio.h>
#include <math.h>
#define FLOWSIZE 32 // 2^5 é o modulo maximo pra 6 bits signed
#define VECTORSIZE 32

typedef unsigned VetSmallInt;

// verifica se um inteiro vai truncar na conversao e retorna 1 se sim
int overflow (int a) {
	return ( a>(FLOWSIZE-1) || a< -FLOWSIZE )
}

// coloca o boolean de overflow de um determinado indice do vetor no respectivo bit do SIV
void setOverflow (int index, int status, VetSmallInt *v) {
	int bitOfOverflow, overflowInbinary, i;
	if (status) {
		bitOfOverflow = VECTORSIZE - (index+1); // calcula o numero do bit que deve ser sobrescrito
		overflowInbinary = pow(2,bitOfOverflow); // gera um numero binario com apenas um bit true (igual a 1)
		v = (v | overflowInbinary);
	}
}

// preenche os bits 24 a 27 com zeros (e nao altera os outros)
void fillWithZeros (VetSmallInt *a) {
	*a = (*a & 0xFFFFFF0F);	
}

// coloca um inteiro x em uma entrada index do VetSmallInt 
void intToSmallInt (int index, int x, VetSmallInt *v) {
	// trunca o valor caso haja overflow
	while (overflow(x)) {
		x -= FLOWSIZE;
	}
	
	// anda com o x o numero de bits do index
	x << index*6; // acho que é o mesmo que ( x += index*FLOWSIZE )
	*v = (*v | x);
}

VetSmallInt vs_new(int val[]) {
	int i;
	VetSmallInt siv; // é o vetor de pequenos inteiros
	
	// preenche os bits 24 a 27
	fillWithZeros(&siv);
	
	// configura os overflow nos bits 28 a 31
	for (i=0; i<4; i++)
		setOverflow(i, overflow(val[i]), &siv);

	// converte os inteiros e os coloca no SIV
	for (i=0; i<4; i++)
		intToSmallInt (i, val[i], &siv);
	
	return siv;
}
