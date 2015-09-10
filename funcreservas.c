#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estruturas.h"

ReservationPtr CreateReservationList(void){
	/*Criar lista de reserva*/
    ReservationPtr aux;
    aux = (ReservationPtr) malloc (sizeof(ReservationType));
    aux->utilizador = NULL;
    aux->sala = NULL;
    aux->prev = NULL;
    aux->next = NULL;
    aux->primeira = -1;
    aux->ultima = -1;    
    return aux;
}

ReservationPtr SearchReservationList (ReservationPtr lista, ReservationPtr chave){
	/*procura uma reserva numa dada lista*/
	ReservationPtr ant, aux;
	ant = lista;
	if (ant!=NULL){
		aux = ant->next;
		while ((aux!=NULL)&&(compara_slot(chave, aux)>0)) {
			ant = aux;
			aux = aux->next;
		}
	}
	/*Devolve ponteiro para anterior*/
	return ant;
}

void enqueue (ReservationPtr novo){
	/*Insere reserva na fila de espera*/
	ReservationPtr aux = novo->sala->prereservas;
	while (aux->next!=NULL) 
		aux=aux->next;
	aux->next = novo;
	novo->prev = aux;
	novo->next = NULL;
}

int InsertReservation(ReservationPtr novo, int flag){
	ReservationPtr ant;
	/*procura reserva anterior na lista*/
	ant = SearchReservationList(novo->sala->reservas,novo);
	/*verifica se a reserva coincide com algum slot de outra reserva*/
	if ((compara_data(ant->data,novo->data)==0)&&((ant->ultima)>=(novo->primeira))) {
		/*se for o mesmo utilizador devolve mensagem e nao insere reserva*/
		if (strcmp(ant->utilizador->nome, novo->utilizador->nome) == 0) 	return -2;
		return -1;
	}
	if ((ant->next!=NULL)&&(compara_data(ant->next->data,novo->data)==0)&&((ant->next->primeira)<=(novo->ultima))) {
		/*se for o mesmo utilizador devolve mensagem e nao insere reserva*/
		if (strcmp(ant->next->utilizador->nome, novo->utilizador->nome) == 0) 	return -2;
		return -1;
	}
	/*Insere reserva na lista*/
	novo->next = ant->next;
	novo->prev = ant;
	ant->next = novo;
	if (novo->next!=NULL) novo->next->prev = novo;
	/*se flag=0 entao estamos a por uma pre-reserva como reserva, logo nao e novamente inserida na lista de reservas do utilizador */
	if (flag!=0) InsertUserRes(novo); 
	return 0;
}

int CreateNewReservation (ReservationPtr novo){
	/*Tenta inserir a reserva na lista das reservas*/
	int flag = InsertReservation(novo,1);
	if (flag==0){
		printf("Reserva inserida com sucesso!\n");
		return 1;
	}
	else if (flag == -1){ /*slot(s) ocupado(s) por outro utilizador*/
		printf("Pelo menos um dos slots está ocupado. Deseja fazer pre-reserva? (y/n) ");
		char option = getchar();
		while ((option!='y')&&(option!='n')) 
			option = getchar();
		if (option=='y') { /*Quer fazer pre-reserva: insere na fila e na lista do user*/
			enqueue(novo);
			InsertUserRes(novo);
			printf("Prereserva inserida\n");
			return 0;
		}
		free(novo);
	}
	else {
		printf("O utilizador ja fez uma reserva ou pre-reserva para pelo menos uma das slots\n");
		free(novo);
		return -1;
	}
}

int RemoveReservation (ReservationPtr item) {
	ReservationPtr aux;
	RoomPtr s;
	
	/*verificar se item nao e nulo e se sala existe*/
	if ((item==NULL)||(item->sala==NULL))
		return -1;
	s = SearchRoomList(item->sala->numero);
	if ((s==NULL) || (s->next==NULL) || (s->next->numero!=item->sala->numero))
		return -1;
	s = s->next;
	
	aux = SearchReservationList(s->reservas,item)->next;
	if (!((aux!=NULL)&&(strcmp(aux->utilizador->nome,item->utilizador->nome)==0)&&(compara_slot(aux, item)==0)&&(aux->ultima == item->ultima))){
		/*Nao encontrou nas reservas. Procurar nas pre-reservas*/
		
		aux = SearchReservationList(s->reservas,item)->next;
		if (!((aux!=NULL)&&(strcmp(aux->utilizador->nome,item->utilizador->nome)==0)&&(compara_slot(aux, item)==0)&&(aux->ultima == item->ultima))){
			/*Nao encontrou em reservas nem pre-reservas*/
			return -1;
		}
	}
	
	/*Encontrou a reserva, apaga-a da lista do utilizador*/
	DeleteUserRes(item);
	if (aux->prev !=NULL)
		aux->prev->next = aux->next;
	if (aux->next!=NULL)
		aux->next->prev = aux->prev;
	free(aux);
	return 1;
}

int	ReplaceReservation(RoomPtr s){
	/*procura na lista das pre-reservas*/
	ReservationPtr temprev, tempnext, aux = s->prereservas->next;
	int flag;
	while (aux!=NULL) {
		tempnext = aux->next;
		temprev = aux->prev;
		flag = InsertReservation(aux,0);
		if (flag==0) { /*Inseriu nas reservas*/
			if (temprev !=NULL)
				temprev->next = tempnext;
			if (tempnext!=NULL)
				tempnext->prev = temprev;
		}
		aux = tempnext;
	}
}

void PrintReservation (ReservationPtr a){
	if ((a!=NULL)&&(a->sala!=NULL)&&(a->utilizador!=NULL))	{
		printf("Sala: %3i\tData: %02i/%02i/%02i\tSlots: %2i a %2i\tUtilizador: %s\n", a->sala->numero, a->data.dia, a->data.mes, a->data.ano, a->primeira, a->ultima, a->utilizador->nome);
	}
}

void DestroyReservationList (ReservationPtr lista){
	/*Destroi a lista de reservas */
	ReservationPtr temp;
    while (lista!=NULL) {
        temp = lista;
        lista = lista->next;
        free(temp);
    }
    free(lista);
}
