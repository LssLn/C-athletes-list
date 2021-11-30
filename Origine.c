/*
Quesito 2
Dato un archivio contenente le informazioni sugli atleti partecipanti ad una gara podistica
(numero di gara, cognome, nome, categoria, tempo in secondi),
implementare un programma in ANSI C che esponga le funzioni di seguito elencate.
Progettare la struttura dati adatta a mantenere in memoria centrale le informazioni utilizzando strutture
e puntatori per le liste.
Scrivere un opportuno main in cui sia previsto un menu di scelta delle operazioni richieste.
Tutti i valori necessari al funzionamento devono essere passati utilizzando parametri, non è permesso l’uso di variabili globali.
> 1.    Caricamento dati da file(vedi di seguito) in una lista semplice ordinata rispetto al cognome.
2.  Conteggio degli atleti appartenenti ad una categoria il cui valore è fornito come parametro alla funzione.
> 3.    Calcolo della classifica in base ad una categoria fornita come parametro,
    il risultato deve essere restituito in una seconda lista ordinata (che contiene, quindi, tutti e solo gli atleti che
    appartengono ad una specifica categoria) in modo crescente rispetto al tempo,
4.  Funzione che restituisce in un vettore di interi(passato come parametro) il numero di atleti iscritto in ogni categoria;
Esempio di file
100 BIANCHI     GIUSEPPE        SENIOR      654
12  ROSSI       CARMELO     JUNIOR      513
14  VERDI       GIOVANNI        JUNIOR      495
120 ARANGIO     AGATINO     SENIOR      712
25  AZZURRO     SALVATORE   JUNIOR      501
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ATLETA_DIM_NUMERO 10
#define ATLETA_DIM_STRINGA 64
// ADT Atleta
typedef struct {
    //    (numero di gara, cognome, nome, categoria, tempo in secondi),
    char numero[ATLETA_DIM_NUMERO];
    char cognome[ATLETA_DIM_STRINGA];
    char nome[ATLETA_DIM_STRINGA];
    char categoria[ATLETA_DIM_STRINGA];
    int tempo;
} t_atleta;
// ADT lista di atleti
typedef struct _s_nodo_lista_atleti {
    t_atleta dato;
    struct _s_nodo_lista_atleti* successivo;
} t_nodo_atleta, * t_lista_atleti;
int confronta_atleti_tempo(t_atleta a, t_atleta b) {
    return a.tempo - b.tempo;
}
int confronta_atleti_nome(t_atleta a, t_atleta b) {
    int risultato_confronto;
    risultato_confronto = strcmp(a.cognome, b.cognome);
    if (risultato_confronto == 0)
        risultato_confronto = strcmp(a.nome, b.nome);

    return risultato_confronto;
}
// f : lista x atleta -> t_lista
t_lista_atleti lista_aggiungi(t_lista_atleti lista, t_atleta atleta) {
    t_nodo_atleta* pt_aux = (t_nodo_atleta*)malloc(sizeof(t_nodo_atleta));
    if (pt_aux == 0)
        return lista;
    pt_aux->dato = atleta;
    pt_aux->successivo = lista;
    lista = pt_aux;
    return lista;
}
//t_lista_atleti lista_aggiungi_ordinato(t_lista_atleti lista, t_atleta atleta);
//t_lista_atleti lista_aggiungi_ordinato_generico(t_lista_atleti lista, t_atleta atleta, int (*confronta)(t_atleti a, t_atleta b)) {
    t_lista_atleti lista_aggiungi_ordinato_generico(t_lista_atleti lista, t_atleta atleta, int (*confronta)(t_atleta a, t_atleta b)) {

    if ((lista == NULL) || confronta(atleta, lista->dato) < 0)
        lista = lista_aggiungi(lista, atleta);
    else if (confronta(atleta, lista->dato) > 0)
        lista->successivo = lista_aggiungi_ordinato_generico(lista->successivo, atleta); //insufficienti arg nella call
    return lista;
}
t_lista_atleti lista_aggiungi_ordinato_nome(t_lista_atleti lista, t_atleta atleta) {
    if ((lista == NULL) || confronta_atleti_nome(atleta, lista->dato) < 0)
        lista = lista_aggiungi(lista, atleta);
    else if (confronta_atleti_nome(atleta, lista->dato) > 0)
        lista->successivo = lista_aggiungi_ordinato_nome(lista->successivo, atleta);
    return lista;
}
t_lista_atleti lista_aggiungi_ordinato_tempo(t_lista_atleti lista, t_atleta atleta) {
    if ((lista == NULL) || confronta_atleti_tempo(atleta, lista->dato) < 0)
        lista = lista_aggiungi(lista, atleta);
    else if (confronta_atleti_tempo(atleta, lista->dato) > 0)
        lista->successivo = lista_aggiungi_ordinato_tempo(lista->successivo, atleta);
    return lista;
}
t_lista_atleti lista_cancella_testa(t_lista_atleti lista) {
    t_lista_atleti aux = lista;
    if (lista == NULL)
        return NULL;
    lista = lista->successivo;
    free(aux);
    return lista;
}
t_lista_atleti lista_cancella_tutto(t_lista_atleti lista) {
    while (lista != NULL)
        lista = lista_cancella_testa(lista);
    return NULL;
}
void stampa_lista_atleti(t_lista_atleti lista, FILE* f) {
    while (lista != NULL) {
        fprintf(f, "%s %s %s %s %d\n", lista->dato.numero, lista->dato.cognome, lista->dato.nome, lista->dato.categoria, lista->dato.tempo);
        lista = lista->successivo;
    }
}
// funzioni di servizio al main
// f : lista x file -> lista
// 100  BIANCHI     GIUSEPPE        SENIOR      654
t_lista_atleti apri_file(t_lista_atleti lista, char nome_file[]) {
    t_atleta atleta;
    FILE* fp;
    fp = fopen(nome_file, "r");
    if (fp == NULL) {
        printf("errore aprendo il file %s\n ", nome_file);
        return lista;
    }
    if (fp == NULL)
        return lista;
    while (fscanf(fp, "%s%s%s%s%d", atleta.numero, atleta.cognome, atleta.nome, atleta.categoria, &atleta.tempo) == 5)
        lista = lista_aggiungi_ordinato_nome(lista, atleta);
    //lista = lista_aggiungi_ordinato_generico(lista, atleta, confronta_atleti_nome);
    fclose(fp);
    return lista;
}
// f : lista x categorie -> lista (ordinata)
t_lista_atleti calcola_classifica(t_lista_atleti lista, char* categoria) {
    t_atleta atleta;
    t_lista_atleti variabile_locale_classifica = NULL;
    while (lista != NULL) {
        // azione
        atleta = lista->dato;
        if (strcmp(categoria, atleta.categoria) == 0)
            variabile_locale_classifica = lista_aggiungi_ordinato_tempo(variabile_locale_classifica, atleta);
        lista = lista->successivo;
    }
    return variabile_locale_classifica;
}
int main(void) {
    int scelta;
    char buffer[128];
    t_lista_atleti archivio_atleti = NULL;
    t_lista_atleti classifica = NULL;
    do {
        printf("1. Apri file\n");
        printf("3. Calcola classifica\n");
        printf("0. Esci\n");
        printf(">> ");
        scanf("%d", &scelta);
        switch (scelta) {
        case 3: //calcola classifica
            printf("Inserisci la categoria: ");
            scanf("%s", buffer);
            if (classifica != NULL)
                classifica = lista_cancella_tutto(classifica);
            classifica = calcola_classifica(archivio_atleti, buffer);
            stampa_lista_atleti(classifica, stdout);
            break;
        case 1: // Apri file
            printf("Inserisci il nome del file: ");
            scanf("%s", buffer);
            archivio_atleti = apri_file(archivio_atleti, buffer);
            break;
        case 0:
            printf("fine programma\n");
            break;
        }
    } while (scelta != 0);
}