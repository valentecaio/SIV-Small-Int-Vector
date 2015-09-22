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

// retorna o bit do respectivo overflow de um small-int em determinado indice
int bitOfOverflow (int i) {
	return (FLOWSIZE-4) +i;
}

// retorna o bit do respectivo complemento a2 de um small-int em determinado indice
int bitOfComplementoA2 (int i) {
	return 5+(i*6);
}

// coloca o boolean de overflow de um determinado indice do vetor no respectivo bit do SIV
void setOverflow (int index, int status, VetSmallInt *v) {
	int overflowInbinary;
	overflowInbinary = 1<<bitOfOverflow(index); // gera um numero binario com apenas um bit true (igual a 1)
	if (status) {
		*v = (*v | overflowInbinary);
	} else {
		overflowInbinary = ~overflowInbinary;	// inverte os bits
		*v = *v & overflowInbinary;
	}
}

// preenche os bits 28 a 31 com zeros (e nao altera os outros)
void cleanOverflow (VetSmallInt *a) {
	*a = (*a & 0x0FFFFFFF);	
}

// zera os 6 bits de um determinado indice do SIV
void deleteFromSmallVector (VetSmallInt *v, int index) {
	int filter = 0xFFFFFF30;			// 
	while (index) {
		filter = filter<<6; 			// empurra 6 bits pra direita
		filter = filter | 0x0000003F	// preenche os 6 primeiros bits com true
		index--;
	}
	*v = *v & filter;
}

// coloca um inteiro x em uma entrada index do VetSmallInt 
void pushToSmallIntVector (int index, int x, VetSmallInt *v) {
	// trunca o valor caso haja overflow
	x = (x & 0x3F); // 0x3F é uma mascara que zera tudo a partir do bit 6

	// configura os overflow nos bits 28 a 31
	setOverflow(index, overflow(x), v);
	
	// apaga o inteiro antigo antes de colocar o novo
	deleteFromSmallVector (v, index);
	
	// anda com o x o numero de bits do index
	x << index*6;
	*v = (*v | x);
}

VetSmallInt vs_new(int val[]) {
	int i;
	VetSmallInt siv; // é o vetor de pequenos inteiros
	
	// preenche os bits 24 a 27
	fillWithZeros(&siv);
	
	// converte os inteiros e os coloca no SIV
	// essa parte ta errada pq nao ta considerando o complemento a 2
	for (i=0; i<4; i++)
		pushToSmallIntVector (i, val[i], &siv);
	
	return siv;
}

// pega o small int do indice index e devolve como signed int
int getCastedToInt (VetSmallInt v, int index) {
	int x,negative;
	x = v >> (index*6);		// empurra o vetor para a direita ate os bits desejados ficarem nas primeiras posicoes
	x = x & 0x3f;			// filtra o valor apagando todo o resto
	
	negative = x & 0x20;	// 0x20 === 0b 0010 0000
	if (negative) {			// 0xFFFFFFb0 é a mascara que preenche todos os bits com true a partir do 6 (a contar da direita pra esquerda)
		x |= 0xFFFFFFb0;	// preenche os bits a esquerda do numero com true
	}
	return x;
}

void vs_print(VetSmallInt v) {
	int i,x;
	printf ("\n\nOverflow: ");
	for (i=28; i<31; i++) {
		if (1<<i & v) {	// vai ser true somente se o bit i do vetor v for true
			printf ("\tsim");
		} else {
			printf ("\tnao");
		}
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
	int s[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++)
	{
		x = getCastedToInt(v,i);
		y = x << n;
		s[i] = y;
	}
	
	siv = vs_new(s);
	cleanOverflow(&siv);
	
	return siv;
}

// é o unico shift que da problema, pq o operador de shift >> funciona como aritmetico para signeds
VetSmallInt vs_shr(VetSmallInt v, int n)
{
	int i, x, y;
	int s[4];
	VetSmallInt siv;
	
	for (i=0; i<4; i++)
	{
		x = getCastedToInt(v,i);
		if (x<0) {			// se x for negativo
			x = x & 0x3F;	// preenche tudo a esquerda do bit 6 com False
		}
		y = x >> n;
		s[i] = y;
	}
	
	siv = vs_new(s);
	cleanOverflow(&siv);
	
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
		y = x >> n; // ja faz shift arimetico por padrao, pq x é signed
		v[i] = y;
	}
	
	siv = vs_new(v);
	cleanOverflow(&siv);
	
	return siv;

}

