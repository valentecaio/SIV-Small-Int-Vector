 /* Caio Valente 1313402 3WB */
 /* Ilana Segal 1510329 3WB */

#include "smallint.h"
#include <stdio.h>
#include <math.h>
#define TOTALSIZE 32 // 2^5 é o modulo maximo pra 6 bits signed
#define VECTORSIZE 4
#define SMALLINTBITS 6
typedef unsigned VetSmallInt;

void printInBinary (int n) {
	int i;
	for (i=31; i>=0; i--){
		if (!( (i+1)%4 ))
			printf (" ");
		printf("%d", (n >> i) & 1);
	}
}

void printVectorInBinary (int n) {
	int i;
	for (i=31; i>=0; i--){
		if (i==5 || i==11 || i==17 || i==23 || i==27)
			printf (" ");
		printf("%d", (n >> i) & 1);
	}
}

void printInBinaryRecursive (int n) {
	if (n>0) {
		printInBinaryRecursive (n/2); // chama outra instância da mesma funcao passando n/2
		printf ("%d", n%2);
	}
}

// verifica se um inteiro vai truncar na conversao e retorna 1 se sim
int overflow (int a) {
	return ( a>(TOTALSIZE-1) || a< -TOTALSIZE );
}

// retorna o bit do respectivo overflow de um small-int em determinado indice
int bitOfOverflow (int i) {
	return (TOTALSIZE - VECTORSIZE) +i;
	//return (TOTALSIZE - VECTORSIZE) +1 +i;
}

// retorna o bit do respectivo complemento a2 de um small-int em determinado indice
int bitOfComplementoA2 (int i) {
	return (SMALLINTBITS-1) + (i*SMALLINTBITS);
	//return (i+1) * SMALLINTBITS;
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

/* essa função eh desnecessaria pq podemos usar a vs_set passando zero
// zera os 6 bits de um determinado indice do SIV
void vs_remove (VetSmallInt *v, int index) {
	int filter = 0xFFFFFFC0;			// 0b 1100 0000
	while (index) {
		filter = filter<<SMALLINTBITS;	// empurra 6 bits pra esquerda
		filter = filter | 0x3F;			// preenche os 6 primeiros bits com true
		index--;
	}
	*v = *v & filter;
}
*/

// coloca um inteiro x em uma entrada index do VetSmallInt 
void vs_set (int index, int x, VetSmallInt *v) {
	// configura os overflow nos bits 28 a 31
	setOverflow(index, overflow(x), v);

	// trunca o valor caso haja overflow
	x = (x & 0x3F);

	// anda com o x o numero de bits do index
	x = x << (index*SMALLINTBITS);
	
	// faz o set
	*v = (*v | x);
}

// pega o small int do indice index e devolve como signed int
int vs_get (VetSmallInt *v, int index) {
	int x,negative;
	x = *v >> (index*SMALLINTBITS);		// empurra o vetor para a direita ate os bits desejados ficarem nas primeiras posicoes
	x = x & 0x3f;			// filtra o valor apagando todo o resto
	
	negative = x & 0x20;	// 0x20 === 0b 0010 0000
	if (negative) {			// 0xFFFFFFb0 é a mascara que preenche todos os bits com true a partir do 6 (a contar da direita pra esquerda)
		x |= 0xFFFFFFb0;	// preenche os bits a esquerda do numero com true
	}
	return x;
}

VetSmallInt vs_new(int val[]) {
	int i;
	VetSmallInt siv=0; // IMPORTANTE INICIALIZAR COM ZERO PRA APAGAR O LIXO
		
	// converte os inteiros e os coloca no SIV
	// essa parte ta errada pq nao ta considerando o complemento a 2
	for (i=0; i<VECTORSIZE; i++)
		vs_set (i, val[i], &siv);
	printf ("\nresultado do set: ");
	printVectorInBinary(siv);
	
	return siv;
}

void vs_print(VetSmallInt v) {
	int i,x;
	printf ("\n\nExibindo dados do vetor de pequenos inteiros: \n\nOverflow: ");
	for (i=TOTALSIZE-VECTORSIZE; i<TOTALSIZE; i++) {
		if (1<<i & v) {	// vai ser true somente se o bit i do vetor v for true
			printf ("\tsim");
		} else {
			printf ("\tnao");
		}
	}
	printf ("\nValores: ");
	for (i=0; i<VECTORSIZE; i++) {
		x = vs_get (&v, i);
		printf("\t%d (%02x)", x, x);
	}
	printf ("\n\n");
}

VetSmallInt vs_add(VetSmallInt v1, VetSmallInt v2)
{
	int i;
	int v[VECTORSIZE];
	VetSmallInt v_add;
	
	for (i=0; i<VECTORSIZE; i++) 
	{
		v[i] = vs_get (&v1,i) + vs_get (&v2,i);
	}
	
	v_add = vs_new(v);
	return v_add;
}

VetSmallInt vs_shl(VetSmallInt v, int n)
{
	int i;
	int s[VECTORSIZE];
	VetSmallInt v_shifted;
	
	for (i=0; i<VECTORSIZE; i++)
	{
		s[i] = vs_get(&v,i) << n;
	}
	
	v_shifted = vs_new(s);
	cleanOverflow(&v_shifted);
	
	return v_shifted;
}

// é o unico shift que da problema, pq o operador de shift >> funciona como aritmetico para signeds
VetSmallInt vs_shr(VetSmallInt v, int n)
{
	int i;
	int s[VECTORSIZE];
	VetSmallInt v_shifted;
	
	for (i=0; i<VECTORSIZE; i++)
	{	// a mascara 0x3F preenche tudo a esquerda do sexto bit com False (nao altera nada nos positivos)
		s[i] = (vs_get(&v,i) & 0x3F) >> n;
	}
	
	v_shifted = vs_new(s);
	cleanOverflow(&v_shifted);
	
	return v_shifted;
}

VetSmallInt vs_sar(VetSmallInt v, int n)
{
	int i;
	int s[VECTORSIZE];
	VetSmallInt v_shifted;
	
	for (i=0; i<VECTORSIZE; i++)
	{
		s[i] = vs_get(&v,i) >> n; // ja faz shift arimetico por padrao, pq x é signed 
	}
	
	v_shifted = vs_new(s);
	cleanOverflow(&v_shifted);
	
	return v_shifted;

}

