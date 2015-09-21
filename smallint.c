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
		bitOfOverflow = (VECTORSIZE -4) +index; // calcula o numero do bit que deve ser sobrescrito
		overflowInbinary = (1<<bitOfOverflow); // gera um numero binario com apenas um bit true (igual a 1)
		v = (v | overflowInbinary);
	}
}

// preenche os bits 24 a 27 com zeros (e nao altera os outros)
void fillWithZeros (VetSmallInt *a) {
	*a = (*a & 0xFFFFFF0F);	
}

void deleteFromSmallVector (VetSmallInt *v, int index) {
	// 11111100000000000000000000000000 === 0xFC000000
	int filter = 0x03FFFFFF;			// 00000011111111111111111111111111 === 0x03FFFFFF
	while (index) {
		filter = filter>>6; 			// empurra 6 bits pra direita
		filter = filter & 0xFC000000	// preenche os 6 primeiros bits com true
		index--;
	}
	*v = *v & filter;
}

// coloca um inteiro x em uma entrada index do VetSmallInt 
void pushToSmallIntVector (int index, int x, VetSmallInt *v) {
	// trunca o valor caso haja overflow
	while (overflow(x)) {
		x -= FLOWSIZE;
	}
	
	// apaga o inteiro antigo antes de colocar o novo
	deleteFromSmallVector (v, index);
	
	// anda com o x o numero de bits do index
	x << index*6; // é o mesmo que ( x += index*FLOWSIZE )
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
	// essa parte ta errada pq nao ta considerando o complemento a 2
	for (i=0; i<4; i++)
		pushToSmallIntVector (i, val[i], &siv);
	
	return siv;
}

// pega o small int do indice index e devolve como signed int
int getCastedToInt (VetSmallInt v, int index) {
	int x,a;
	/*
	if (i==0)
		x = v & 0xF8000000; // 11111000000000000000000000000000
	else if (i==1)
		x = v & 0x03E00000; // 00000011111000000000000000000000
	else if (i==2)
		x = v & 0x000F8000; // 00000000000011111000000000000000
	else if (i==3)
		x = v & 0x00003E00; // 00000000000000000011111000000000
	*/
	// as duas linhas abaixo resumem o comentario acima
	x = (0xF8000000 >> (i*6));
	x = (x & v);
	
	a = 5+(index*6); // calcula qual bit é o complemento a 2 desse small int
	if (1<<a & v) // (1<<a) equivale a (pow (2,a))
		x = -x;
	return x;
}

void vs_print(VetSmallInt v) {
	int i,x;
	printf ("\n\nOverflow: ");
	for (i=28; i<31; i++) {
		// pow(2,i) (o mesmo que 1<<i) so possui um bit true, que eh o bit que vamos filtrar
		if (1<<i & v) {	// vai ser true somente se o bit i do vetor v for true
			printf ("\tsim");
		}
		else
			printf ("\tnao");
	}
	printf ("\nValores: ");
	for (i=0; i<4; i++) {
		x = getCastedToInt (v, i);
		printf("\t%d (%02x)", x, x);
	}
}

VetSmallInt vs_add(VetSmallInt v1, VetSmallInt v2)
{
	int i, x, y;
	int v[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++) 
	{
		x = getCastedToInt (v1,i);
		y = getCastedToInt (v2,i);
		v[i] = x+y;
	}
	
	siv = vs_new(v);
	
	return siv;
}

VetSmallInt vs_shl(VetSmallInt v, int n)
{
	int i, x, y;
	int v[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++)
	{
		x = getCastedToInt(v,i);
		y = x << n;
		v[i] = y;
	}
	
	siv = vs_new(v);
	siv = (siv & 0x0FFFFFFF);	
	
	return siv;
	
}

VetSmallInt vs_shl(VetSmallInt v, int n)
{
	int i, x, y;
	int v[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++)
	{
		x = getCastedToInt(v,i);
		y = x << n;
		v[i] = y;
	}
	
	siv = vs_new(v);
	siv = (siv & 0x0FFFFFFF);	
	
	return siv;
	
}

VetSmallInt vs_shl(VetSmallInt v, int n)
{
	int i, x, y;
	int v[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++)
	{
		x = getCastedToInt(v,i);
		y = x >> n;
		v[i] = y;
	}
	
	siv = vs_new(v);
	siv = (siv & 0x0FFFFFFF);	
	
	return siv;
}

VetSmallInt vs_sar(VetSmallInt v, int n)
{
	int i, x, y;
	int v[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++)
	{
		x = getCastedToInt(v,i);
		y = x >> n;
		v[i] = y;
	}
	
	siv = vs_new(v);
	siv = (siv & 0x0FFFFFFF);	
	
	return siv;

}

