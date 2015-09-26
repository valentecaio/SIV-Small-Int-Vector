#include <stdio.h>

typedef unsigned VetSmallInt;

/* Cria um vetor de SmallInt a partir dos valores fornecidos */
VetSmallInt vs_new(int val[]);

/* Soma dois vetores de Small Int */
VetSmallInt vs_add(VetSmallInt v1, VetSmallInt v2);

/* Imprime um vetor de Small Int */
void vs_print(VetSmallInt v);

/* Shift lÃ³gico para a direita */
VetSmallInt vs_shr(VetSmallInt v, int n);

/* Shift aritmÃ©tico para a direita */
VetSmallInt vs_sar(VetSmallInt v, int n);

/* Shift para a esquerda */
VetSmallInt vs_shl(VetSmallInt v, int n);

/* LÃª um vetor de small ints */
VetSmallInt vs_read(FILE *f);

/* Grava um vetor de small ints */
int vs_write(VetSmallInt v, FILE *f);

/* Grava um vetor de small ints em binario */
int vs_writeInBinary(VetSmallInt v, FILE *f);

/* pega o small int do indice index e devolve como signed int */
int vs_get (VetSmallInt *v, int index);

/* coloca um signed int x em uma entrada index do VetSmallInt */
void vs_set (int index, int x, VetSmallInt *v);

/* printa um vetor de small int em binario */
void vs_printInBinary (int n) 
