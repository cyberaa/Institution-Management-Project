#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estruturas.h"

RoomPtr CreateRoomList(void){
	/*Cria uma sala*/
    RoomPtr aux;
    aux = (RoomPtr) malloc (sizeof(RoomType));
	aux -> next = NULL;
	/*Inicializa lista de reservas da sala*/
    aux -> reservas = CreateReservationList();
    aux -> reservas -> sala = aux;
	/*Inicializa lista de pre-reservas da sala*/
    aux -> prereservas = CreateReservationList();
    aux -> prereservas -> sala = aux;
    return aux;
}

RoomPtr SearchRoomList (int chave) {
	/*Procura lugar onde sala deve ser inserida*/
	RoomPtr aux = salas;
	while ((aux!=NULL)&&(aux->next!=NULL)&&(chave > aux->next->numero)) 	aux=aux->next;
	return aux;
}

void InsertRoom (int novo){
	/*Insere sala na lista*/
	RoomPtr ant, aux;
	/*Procura sala*/
	ant = SearchRoomList (novo);
	aux = CreateRoomList();
	/*Insere sala*/
	aux->numero = novo;
	aux->next = ant->next;
	ant->next = aux;
}


void DestroyRoomList(void){
	/*Destroi a lista de salas*/
	RoomPtr temp;
    while (salas!=NULL){
        temp = salas;
        salas = salas->next;
		/*Destroi listas de reservas e pre-reservas*/
		DestroyReservationList(temp->reservas);
		DestroyReservationList(temp->prereservas);
        free(temp);
    }
}
