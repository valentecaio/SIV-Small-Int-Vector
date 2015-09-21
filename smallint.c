 /* Caio Valente 1313402 3WB */
 /* Ilana Segal 1510329 3WB */

#include "smallint.h"
#include <stdio.h>
#include <math.h>
#define FLOWSIZE 32

typedef unsigned VetSmallInt;

// verifica se um inteiro é overflow e retorna 1 se sim
int overflow (int a) {
	return ( a>(FLOWSIZE-1) || a< -FLOWSIZE )
}



// coloca o boolean do overflow do vetor
setOverflow (int index, int status, int *v) {
	int bitOfOverflow, overflowInbinary, i;
	if (status) {
		bitOfOverflow = VECTORSIZE - (pow(2,index)+1); // calcula o numero do bit que deve ser sobrescrito
		overflowInbinary = pow(2,bitOfOverflow); // gera um numero binario com apenas um bit true (igual a 1)
		v = (v | overflowInbinary);
	}
}

VetSmallInt vs_new(int val[])
{
        int i, n;
        VetSmallInt siv; // é o vetor de pequenos inteiros

		// configura os overflow
        for (i = 0; i<4; i++)
			setOverflow(i, overflow(val[i]), &siv);



}

