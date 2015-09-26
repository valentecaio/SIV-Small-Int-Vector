 /* Caio Valente 1313402 3WB */
 /* Ilana Segal 1510329 3WB */

#include "smallint.h"
#include <stdio.h>
#include <math.h>
#define TOTALSIZE 32	// numero maximo de bits em um SIV
#define VECTORSIZE 4	// numero de indices do SIV
#define SMALLINTBITS 6	// numero de bits de cada entrada do SIV
typedef unsigned VetSmallInt;

typedef union {
	char character[TOTALSIZE/8];
	VetSmallInt inteiro;
} VetSmallIntUnion;

int maskFirstIndex() { 	
	return ~(~0 << SMALLINTBITS);	// ~(~0 << SMALLINTBITS) equivale a 0x3F para 4 indices
}

int smallIntIsNegative(int x) {
	return x & ( 1<<(SMALLINTBITS-1) );	// 0x20 === 0b 0010 0000
}

// serve para generalizar o codigo e evitar muitas comparações no if da vs_printInBinary
int isLastBitOfAnyIndex(int i) {
		int j;
		for (j=1; j<=VECTORSIZE; j++) {
			if ((j*SMALLINTBITS)==i) {
				return 1;
			}
		}
		return 0;
}

void printIntInBinaryRecursive (int n) {
	if (n>0) {
		printIntInBinaryRecursive (n/2); // chama outra instância da mesma funcao passando n/2
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
}

// retorna o bit do respectivo complemento a2 de um small-int em determinado indice
int bitOfComplementoA2 (int i) {
	return (SMALLINTBITS-1) + (i*SMALLINTBITS);
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

// preenche os bits de overflow com zeros (e nao altera os outros)
void cleanOverflow (VetSmallInt *a) { // os dois metodos funcionam, mas o primeiro parece mais eficiente
	int mask = ((unsigned)-1) >> VECTORSIZE;
	//int mask = ~( ~0 << (TOTALSIZE-VECTORSIZE) );
	*a = (*a & mask);	// para 4 indices, mask = 0x0FFFFFFF 
}

/* essa função eh desnecessaria pq podemos usar a vs_set passando zero
// ficou salva para efeitos de documentacao
// zera os 6 bits de um determinado indice do SIV
void vs_remove (VetSmallInt *v, int index) {
	int filter = ~maskFirstIndex();			// -0b 1100 0000
	while (index) {
		filter = filter<<SMALLINTBITS;	// empurra bits pra esquerda
		filter = filter | maskFirstIndex();			// preenche os primeiros bits com true
		index--;
	}
	*v = *v & filter;
}
*/

void vs_printInBinary (int n) {
	int i;
	for (i=TOTALSIZE-1; i>=0; i--){
		printf("%d", (n >> i) & 1);
		if (isLastBitOfAnyIndex(i) || i==TOTALSIZE-VECTORSIZE) {
			printf (" ");
		}
	}
}

// coloca um signed int x em uma entrada index do VetSmallInt 
void vs_set (int index, int x, VetSmallInt *v) {
	// configura os overflow nos bits 28 a 31
	setOverflow(index, overflow(x), v);

	// trunca o valor caso haja overflow
	x = (x & maskFirstIndex());	

	// anda com o x o numero de bits do index
	x = x << (index*SMALLINTBITS);
	
	// faz o set
	*v = (*v | x);
}

// pega o small int do indice index e devolve como signed int
int vs_get (VetSmallInt *v, int index) {
	int x;
	x = *v >> (index*SMALLINTBITS);	// empurra o vetor para a direita ate os bits desejados ficarem nas primeiras posicoes
	x = x & maskFirstIndex();		// filtra o valor apagando todo o resto
	
	if (smallIntIsNegative(x)) {
		x |= ~maskFirstIndex();		//  (~0 << SMALLINTBITS) === 0xFFFFFFc0 === -0b 1100 0000
	}
	
	return x;
}

VetSmallInt vs_new(int val[]) {
	int i;
	VetSmallInt siv = 0; // IMPORTANTE INICIALIZAR COM ZERO PRA APAGAR O LIXO
		
	// converte os inteiros e os coloca no SIV
	for (i=0; i<VECTORSIZE; i++) {
		vs_set (i, val[i], &siv);
	}
	
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
	printf ("\nValor do vetor em unsigned int: %u", v);
	printf ("\n\n");
}

VetSmallInt vs_add(VetSmallInt v1, VetSmallInt v2) {
	int i, v[VECTORSIZE];
	VetSmallInt v_add;
	
	for (i=0; i<VECTORSIZE; i++) {
		v[i] = vs_get (&v1,i) + vs_get (&v2,i);
	}
	
	v_add = vs_new(v);
	return v_add;
}

VetSmallInt vs_shl(VetSmallInt v, int n) {
	int i, s[VECTORSIZE];
	VetSmallInt v_shifted;
	
	for (i=0; i<VECTORSIZE; i++){
		s[i] = vs_get(&v,i) << n;
	}
	
	v_shifted = vs_new(s);
	cleanOverflow(&v_shifted);
	
	return v_shifted;
}

// é o unico shift que da problema, pq o operador de shift >> funciona como aritmetico para signeds
VetSmallInt vs_shr(VetSmallInt v, int n) {
	int i, s[VECTORSIZE];
	VetSmallInt v_shifted;
	
	for (i=0; i<VECTORSIZE; i++){	// a mascara nao altera nada nos positivos
		s[i] = ( (vs_get(&v,i) & maskFirstIndex() ) >> n);
	}
	
	v_shifted = vs_new(s);
	cleanOverflow(&v_shifted);
	
	return v_shifted;
}

VetSmallInt vs_sar(VetSmallInt v, int n) {
	int i, s[VECTORSIZE];
	VetSmallInt v_shifted;
	
	for (i=0; i<VECTORSIZE; i++){
		s[i] = vs_get(&v,i) >> n; // ja faz shift arimetico por padrao, pq x é signed 
	}
	
	v_shifted = vs_new(s);
	cleanOverflow(&v_shifted);
	
	return v_shifted;

}
/*
int vs_writeStringInBinary(VetSmallInt v, FILE *f) {
	int i;
	for (i=TOTALSIZE; i>=0; i--){
		if (fprintf(f, "%d", (v >> i) & 1) < 0) {
			return -1;
		}
	}
	return 0;
}

int vs_writeString(VetSmallInt v, FILE *f) {
	if (fprintf(f, "%u", v) != 0) {
		return -1;
	}
	return 0;
}

VetSmallInt vs_readString(FILE *f) {
	int x;
	VetSmallInt a;
	if (fscanf(f, "%u", &a) != 0) {
		return (unsigned)-1;
	}
	return a;
}
*/

int vs_write(VetSmallInt v, FILE *f) {
	int i, returned_value;
	VetSmallIntUnion a;
	a.inteiro = v;
	for (i=3; i>=0; i--) { // inverte para colocar em big-endian
		returned_value = fputc(a.character[i], f);
		if (returned_value == EOF){
			return -1;
		}
		/*if (returned_value != written) { // de acordo com o man do fputc, isso deveria funcionar, mas nao funcionou
			return -1;
		}*/
	}
	return 0;
}

VetSmallInt vs_read(FILE *f) {
	int i, returned_value;
	VetSmallIntUnion a;
	for (i=3; i>=0; i--) { // inverte para colocar de volta em little-endian
		returned_value = fgetc(f);
		if (returned_value == EOF) {
			return -1;
		}
		a.character[i] = returned_value;
	}
	if (fgetc(f) != EOF){ // se o arquivo tiver mais de 4 bytes é porque está errado
		return -1;
	}
	return a.inteiro;
}