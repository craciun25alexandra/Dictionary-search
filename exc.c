/*CRACIUN Alexandra-Georgiana- 311CB*/

#include <string.h>
#include "nou.h"
#define NR_ALFA 26   // sunt 26 de litere in alfabet; aloc tabela hash
#define MAX 1000    //am presupus dimensiunea unei linii din cerinta de 1000 de caractere

TCuv* CreateCuv() { //creaza variabila de tipul TCuv
    TCuv* cuv = malloc(sizeof(TCuv));
    cuv->s = malloc(NR_LIT);
    cuv->nr_ap = 0;
    return cuv;
}
List* CreateList()
{
    List* list = (List*)malloc(sizeof(struct _List)); ///aloc memorie pentru o lista
    list->head = NULL; ///marchez faptul ca este goala punand capul de lista NNULL
    list->size = 0; ///marchez dimensiunea 0 (o lista goala nu are elemente)
    return list;
}

HashTable* CreateTable(int size, int (*hashCode)(void*))
{
    HashTable* table = malloc(sizeof(HashTable)); 
    table->size = size;
    table->hashCode = hashCode;

    table->lists = malloc(size*sizeof(List*));
    for(int i = 0; i < size; i++)
        table->lists[i] = CreateList();

    return table;
}
int lungime (void* cuv ){   //subprogram ce returneaza lungimea unui cuvant

    TCuv *s1 = (TCuv*)cuv;
    return strlen(s1->s); 
    
}
int cmp_lexic(void*a1, void*a2){    //subprogram ce compara lexicografic 2 cuvinte ce sunt de tipul void *

    TCuv *A1 = (TCuv*)a1;
    TCuv *A2 = (TCuv*)a2;
    return strcmp(A1->s, A2->s);

}

int cmp_ap(void* a, void *b){ //subprogram ce compara dupa aparitii 2 cuvinte ce sunt de tipul void *

    TCuv *A = (TCuv*)a;
    TCuv *B = (TCuv*)b;
    if (A->nr_ap > B->nr_ap) return 1;
    else if (A->nr_ap < B->nr_ap) return -1;
    return 0;
}
int cmp_len (void*a, void *b){  //subprogram ce compara dupa lungime 2 cuvinte ce sunt de tipul void *

    TElem*B = (TElem*)b;
    int A = *(int *)a;
    if(A > B->len) return 1;
    else if(A < B->len) return -1;
    return 0;
}

int HashCondition ( void* tcuv){ //conditia hash

    TCuv* a = (TCuv*) tcuv;
    if(a->s[0]>='A'&&a->s[0]<='Z') return a->s[0]-'A';
    return a->s[0]-'a'; //fie ca sunt cu litera mare sau mica, trebuie pusi in acelasi vector
}

void Increment(void * a){
                            // mareste cu o unitate nr de aparitii
    TCuv * A = (TCuv*) a;
    A->nr_ap ++;

}
int nrApInitial(void*a){    //returneaza nr de aparitii al unui void*

    TCuv * A = (TCuv*)a;
    return A->nr_ap;
}

List* IaSublista (void* a){ //intoarce sublista de la o adresa void*

    TElem* A = (TElem*) a;
    return A->cuvinte;
}

//AddSublista adauga in ordine cuvintele in listele secundare(subliste), dupa cele 2 criterii:
//intai dupa aparitii, apoi lexico-grafic

void AddSublista (List* list, void* value, int (*compare1)(void*, void*), int (*compare2)(void*, void*))
{
    Node* n = list->head;
    if(list->head == NULL) {    //daca trebuie adaugat primul element din sublista
        list->head = (Node*)malloc(sizeof(struct _Node));
        list->head->next = NULL;
        list->head->value = value;
        return;
    }
    //daca trebuie schimbat headul listei, cu cele 2 posibilitati:
    //val curenta are mai multe aparitii, sau are acelasi nr de aparitii dar este lexicografic inainte
    if(compare1(n->value, value) < 0 || (compare1(n->value, value)==0 && compare2(n->value, value)>0)){
        Node* aux = (Node*)malloc(sizeof(struct _Node));
        aux->value = value;
        aux->next = list->head;
        list->head = aux;
        return;
    }
    //altfel, parcurge lista pana la pozitia unde trebuie inserat si adauga-l in sublista
    while (n->next != NULL && (compare1(n->next->value, value) > 0 || 
    ((compare1(n->next->value, value)==0 && compare2(n->next->value, value)<0)))) {
        n = n->next;
    }
        Node* aux = (Node*)malloc(sizeof(struct _Node));
        aux->value = value;
        aux->next = n->next;
        n->next = aux;
}

//Addlist adauga in ordine, intr-o lista generica cu ajutorul castului,
//elementele de tipul TElem (sublista +len) si le salveaza ca void *
//ordinea se face dupa dimensiunea cuvantului

void AddList(List* list, void* value1, int (*compare1)(void*, void*), int (*compare2)(void*, void*),
int(*compare3)(void*, void*))
{
    Node* n =list->head;
    int * len_cuv =(int*)malloc(sizeof(int));
    *len_cuv = lungime(value1);
    if(!list->head){  //daca este primul element al listei
     
        list->head = (Node*)calloc(sizeof(struct _Node), 1); //aloca-l
        list->head->next =  NULL; //urmatorul este NULL
        TElem * Elaux = (TElem*)malloc(sizeof(TElem)); //variabila auxiliara pentru a pune informatia de tipul TElem
        Elaux->cuvinte =(List*) malloc(sizeof(List)); //aici se va crea sublista
        Elaux->cuvinte->head = NULL;
        AddSublista(Elaux->cuvinte, value1, compare2, compare3); //creeaza sublista cu criteriile specifice
        Elaux->len = *len_cuv;  //adauga si strlen-ul
        list->head->value = (void*) Elaux; //cast la tipul void
        free(len_cuv); //eliberarea pointerului pt int
        return;
    }
    else if(compare1((void*)len_cuv, n->value) <0){
        //daca trebuie modificat headul
        Node* aux = (Node*)malloc(sizeof(struct _Node)); //aloca celula pt head
        TElem * Elaux = (TElem*)malloc(sizeof(TElem));//variabila auxiliara pentru a pune informatia de tipul TElem
        Elaux->cuvinte =(List*) malloc(sizeof(List));//aici se va crea sublista
        Elaux->cuvinte->head = NULL;
        Elaux->len = *len_cuv; //se pune strlen-ul
        AddSublista(Elaux->cuvinte, value1, compare2, compare3); //creeaza sublista cu criteriile specifice
        aux->value = (void*)Elaux;  //castare la void*
        aux->next = list->head; //se fac legaturile
        list->head = aux;
        free(len_cuv);return;
       
    }
    else{//cautare pozitie
        while (n->next!=NULL && compare1((void*)len_cuv, n->next->value)>0){ 
            //cat timp strlen-ul valorii date e mai mare, avanseaza
            n = n->next;}
        if(n->next && compare1((void*)len_cuv, n->next->value)<0 && compare1((void*)len_cuv, n->value)!=0){ 
            //daca trebuie creat un nou TElem intre 2 existenti
            Node* aux = (Node*)malloc(sizeof(struct _Node)); //aloca celula
            TElem * Elaux = (TElem*)malloc(sizeof(TElem)); //aloca auxiliar pt informatia din celula de tip TElem
            Elaux->cuvinte =(List*) malloc(sizeof(List));
            Elaux->cuvinte->head = NULL;
            Elaux->len = *len_cuv;
            AddSublista(Elaux->cuvinte, value1, compare2, compare3); //pune in sublista
            aux->value = (void*)Elaux; //cast
            aux->next = n->next; //legaturile
            n->next = aux;
            }
            else if(n->next == NULL && compare1((void*)len_cuv, n->value)!=0){
            // daca trebuie creat un nou TElem la final 
            Node* aux = (Node*)malloc(sizeof(struct _Node));
            aux->next = NULL;
            TElem * Elaux = (TElem*)malloc(sizeof(TElem));
            Elaux->cuvinte =(List*) malloc(sizeof(List));
            Elaux->cuvinte->head = NULL;
            Elaux->len = *len_cuv;
            AddSublista(Elaux->cuvinte, value1, compare2, compare3);
            aux->value = (void*)Elaux;
            n->next = aux;  
      
        }
        else {
            //daca elementul trebuie pus intr-o lista existenta
            //adica exista o lista de cuvinte cu aceeasi litera de inceput si acelasi nr de litere
            while(compare1((void*)len_cuv, n->value)!=0) //cauta lista
            n=n->next;
            List * l = IaSublista(n->value); //adauga-l in sublista( in valoarea nodului din lista principala)
            AddSublista(l, value1, compare2, compare3);

        }
        free(len_cuv);

    }

}
void AddToTable( HashTable* t, void* value){//adaugare in tabela hash

    int poz = t->hashCode(value);  //ia codul hash
    AddList(t->lists[poz], value, cmp_len, cmp_ap, cmp_lexic);//adauga valoarea in lista de cod specifica
}
void freeCuv( void* a){    //eliberarea memoriei pt un TCuv dat ca void*, functie utila pt eliminare si dealocare

    TCuv* A = (TCuv*)a;
    free(A->s);
    free(A); 
}
//functia FindValueInTable cauta stringul dat ca void* in tabela hash si returneaza 0 daca nu exista in nicio sublista,
//-1 daca valoarea este gasita ca si head al unei subliste sau numarul de aparitii al elementului

int FindValueInTable(HashTable *t, void* value, int (*compare1)(void*, void*),int (*compare2)(void*, void*)){
    int poz1 = t->hashCode(value);
    int  len_cuv = lungime(value); //lungimea cuvantului dat
    Node* n;
    if(t->lists[poz1])
        n = t->lists[poz1]->head;//daca exista o lista de cuvinte care sa inceapa cu aceeasi lit, porneste de la head
    else return 0; //daca nu exista, cert nu se afla in vreo sublista
    while( n!=NULL && compare1((void*)&len_cuv, n->value)!=0)
        n=n->next; //avanseaza cat timp nu gaseste o lista de cuvinte cu acelasi nr de litere ca valoarea data
    if (!n) return 0; //a ajuns la NULL si nu a gasit
    else{//a gasit o lista de cuvinte cu acelasi nr de litere ca valoarea data
        List* l = IaSublista(n->value); //ii ia sublista
        Node* m = l->head;
        if(compare2(value, m->value)==0){//daca valoarea este head, atunci ii incrementeaza nr de aparitii
           Increment(m->value);
           return -1;}
        else{//daca nu, o cauta in sublista
            while( m->next!=NULL && compare2(value, m->next->value)!=0)
                m = m->next;
            if(m->next && compare2(value, m->next->value)==0){//daca o gaseste, ii pastreaza nr de aparitii intr-un int
            int nr_aparitii= nrApInitial(m->next->value);       //pe care il va returna la final
            Node *aux = m->next->next;                  //elimina celula ce era in lista
            freeCuv(m->next->value);
            free(m->next);
            m->next = aux;
            return nr_aparitii;}
            return 0;} //nu gaseste valoarea in sublista
        
        }
}

void printCuv (void* a){//printarea elementului de tip TCuv 

    TCuv * A= (TCuv*)a;
    printf("%s/%d", A->s, A->nr_ap);
}

void printElement(void*a, void (*printElement2)(void*)){    //printarea unui element TElem

    TElem* aux = (TElem *) a;
    printf("(%d:", aux->len);    //printarea lungimii nr
    Node * n = aux->cuvinte->head;
    while(n){       //printarea sublistei
        printElement2(n->value);
        n=n->next; 
        if(n) printf(", "); 
    }
    printf(")");
}

//printarea tabelei hash
void printTable(HashTable* table, void (*printElement1)(void*, void(*) (void*)), void (*printElement2)(void*))
{
    for (int i = 0; i < table->size; i++) {
        if (table->lists[i]->head != NULL) {
            printf("pos %d: ", i);
            for (Node* iter = table->lists[i]->head; iter != NULL; iter = iter->next)
                printElement1(iter->value, printElement2);  //printeaza elementele din lista principala
            printf("\n");                                   //si trece la urm nod din lista principala
        }
    }
}

void print2(HashTable*t, int n){

    int i, ok1, ok2,ok3, numar_ap;
    Node *m = NULL, *aux;
    for(i = 0; i< 26; i++){   
        
        ok1 = 0;    //inca nu s a afisat nimic cu litera corespunzatoare lui i
        if(t->lists[i]){
            m = t->lists[i]->head;}    //daca exista lista, incepe de la head
        while(m){

            ok2 = 0;    //nu s a afisat nicio sublista
            List *Laux = IaSublista(m->value);
            aux = Laux->head;
            while(aux){    //cauta in sublista
                
                numar_ap = nrApInitial(aux->value);
                if(numar_ap <=n && numar_ap >0) //daca a aparut de cel mult n ori, afiseaza-l
                {                    
                    if(ok1==0){
                        printf("pos%d: ",i ); //daca e primul nod afisat, indica litera
                        ok1=1;}    //indica ca ai afisat nivelul principal
                        
                    if(ok2==0) {    //daca e primul element din sublista, indica strlen-ul
                        printf("(%d: ", lungime(aux->value));
                        ok2=1;}
                    else printf(", ");    //daca nu e primul, desparte printr-o virgula
                
                printCuv(aux->value);    //printeaza cuvantul
                }    
                aux=aux->next;    //continua cautarea in sublista
               }
            
            if(ok2)  printf(")");    //daca s-a afisat vreo sublista, marcheaza sfarsitul ei
            m=m->next;    //continua cautarea in lista
}
                if(ok1)printf("\n");

        }
    }


void print1(List *l, int*nr_dat){   //printarea elementelor cu litera si nr date

        Node *n = l->head;
        while( n!=NULL && cmp_len(nr_dat, n->value)!=0)
            n = n->next;    //avanseaza pana gasesti nodul din lista principala care are acelasi strlen
        if(n){
            printElement(n->value, printCuv); //daca exista, printeaza elementul de tip TElem prin apelarea functiei
        printf("\n");}

}

//freeSublist elibereaza sublista, apelandu-se cu subprogramul freeCuv(are elemente de tip TCuv)
void freeSublist(List**list, void(*freeValue)(void*)){
    Node* current = (*list)->head; ///luam nodul de inceput
    Node* next;
    
    while(current != NULL) ///cat timp mai avem elemente
    {
        next = current->next; ///salvam nodul urmator
        freeValue(current->value); ///eliberam memoria alocata elementului
        free(current); ///eliberam nodul curent
        current = next; ///trecem mai departe
    }
    if (*list != NULL)
        free(*list); ///eliberam memoria alocata listei
    *list = NULL;
}

//freeBigList elibereaza memoria pt elementele din lista principala(TElem uri)
void freeBigList(List** list, void (*freeValue)(void*))
{
    Node* current = (*list)->head; ///luam nodul de inceput
    Node* next;

    while(current != NULL) ///cat timp mai avem elemente
    {
        next = current->next; ///salvam nodul urmator
        List *l = IaSublista(current->value);
        freeSublist(&l, freeValue); ///eliberam memoria alocata sublistei din value
        free(current->value); //eliberam memoria alocata valorii, in afara sublistei(TElem* = ...)
        free(current); ///eliberam nodul curent
        current = next; ///trecem mai departe
    }
    if (*list != NULL)
        free(*list); ///eliberam memoria alocata listei
    *list = NULL;
}
void freeTable(HashTable** table, void (*freeValue)(void*))    //elibereaza memoria tabelei hash
{
    for(int i = 0; i < (*table)->size; i++)
        freeBigList(&((*table)->lists[i]), freeValue);
    free((*table)->lists);
    free(*table);
    *table = NULL;
}

int verif (char *s) {    //verifica existenta numerelor in cuvinte; in caz afirmativ, ele nu sunt eligibile,
                        //deci returneaza 0

    if(strchr(s,'0') || strchr(s,'1') || strchr(s,'2')|| strchr(s,'3')|| strchr(s,'4')|| 
    strchr(s,'5') || strchr(s,'6') || strchr(s,'7') || strchr(s,'8') || strchr(s,'9')) return 0;
    return 1;

}
int main(int argc, char*argv[]){

    HashTable* t = CreateTable(NR_ALFA, HashCondition);
    FILE * f = fopen(argv[1], "r");  //deschiderea fisierelor de input
    int aux, poz, p_urm2, cond; 
    char p_urm1;
    char *p;
    TCuv * c;
    char *cerinta = malloc(MAX);
    while(fgets(cerinta, MAX, f)){  //am presupus dimensiunea unei linii din cerinta de 1000 de caractere
       
        if(strchr(cerinta, '\r')) p = strtok(cerinta , "\r"); //pentru a exclude endline-ul
        if(strchr(cerinta, '\n')) p = strtok(cerinta , "\n");  //pentru a exclude caracterul de returnare
        p = strtok(cerinta , " ,.");
        if(strcmp(p, "insert")==0){
            p = strtok(NULL , " ,.");  //se sparg cuvintele ce trebuie puse in hash
            while(p){
        
                p[strlen(p)]='\0';  //se pune null la finalul cuvantului pt a evita caractere nedorite
                if (strlen(p) > 2 && verif(p)){  //daca respecta conditiile din enunt
                    c = CreateCuv();    //se creeaza o variabila cu string si nr de aparitii setat 1
                    c->nr_ap = 1;
                    strcpy(c->s, p);
                    aux = FindValueInTable(t,(void*)c,cmp_len, cmp_lexic);
                        if (aux>=0){       //daca nu este cap de lista principala
                                    c->nr_ap+=aux;  //se actualizeaza nr de aparitii
                                    AddToTable(t,(void*)c);}   // se punde in ordine in lista cu noul nr de aparitii
                        else{ 
                            free(c->s);   //daca este cap de lista, nr de aparitii a fost incrementat
                            free(c);}    //si eliberez variabila de tipul cuv in care am stocat cand am citit cuvantul
                    }
                p = strtok(NULL, " .,");
            }
        }
        else{
            p = strtok(NULL , " ,.");
            if(!p) //daca nu mai sunt caractere dupa print, se printeaza tabela hash 
                        //cu subprogramele specifice celor 2 niveluri
                printTable(t, printElement, printCuv);
            else{
                p_urm1 = *(char*)p;   //stochez intr-un char urmatoarea comanda
                p = strtok(NULL , " ,.");    
                if (!p) {   //daca nu mai este un caracter dupa, il transform in valoarea sa numerica
                    cond = p_urm1-'0';   
                    print2(t, cond);  //se apeleaza subprogramul de printare specific
                    
                }
                else {
                    p_urm2 = p[0]-'0'; //se transforma in val numerica
                    if(p_urm1 >='A' && p_urm1 <='Z') poz = (int)(p_urm1 -'A');
                    else poz = (int)(p_urm1 -'a'); // se verifica prima litera, pt a calcula codul hash
                    print1(t->lists[poz], &p_urm2);  //se apeleaza subprogramul de printare specific
                }

            }


        }
        
        }
    //la final, eliberez memoria
    free(cerinta);
    freeTable(&t, freeCuv);
    fclose(f);
}       

  