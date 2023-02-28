/*CRACIUN Alexandra-Georgiana- 311CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NR_LIT 36 //cel mai lung cuvant are 36 de litere *google reference

typedef struct _Node
{
    void* value; ///pointer catre valoarea stocata in nod
    struct _Node* next; ///pointer catre nodul urmator
}Node;
typedef struct _List
{
    struct _Node* head; ///pointer catre capul de lista
    unsigned int size; ///numarul de elemente din lista
}List;
typedef struct _hashTable
{
    int size;       //cate componente are vectorul
    int (*hashCode)(void*);
    List** lists;      //vectorul de liste
}HashTable;

typedef struct{     //o structura de tip element, ce contine un int- destinat pt stocarea numarului
                    // de caractere al unui cuvant- si o lista de cuvinte(sublistele)
    List* cuvinte; 
    int len;
}TElem;
typedef struct _TCuv
{                   //structura TCuv ce contine stringul dat si nr de aparitii
    char* s;    
    int nr_ap;  
}TCuv;

