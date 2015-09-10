#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estruturas.h"

int compara_slot (ReservationPtr a, ReservationPtr b){
	/*Compara data e primeira slot da reserva*/
	int num = compara_data(a->data,b->data);
	if (num!=0) return num;
	if (a->primeira>b->primeira) return 1;
	if (a->primeira<b->primeira) return -1;
	return 0;
}

int compara_data (Date a, Date b){
	/*Compara data*/
	if (a.ano>b.ano) return 1;
	if (a.ano<b.ano) return -1;
	if (a.mes>b.mes) return 1;
	if (a.mes<b.mes) return -1;
	if (a.dia>b.dia) return 1;
	if (a.dia<b.dia) return -1;
	return 0;
}


int ReadSlot(void){
	/*Verifica se o valor inserido para a slot e valido*/
	int new, erro;
	erro = scanf("%i",&new);
	if ((erro==0)||(new<0)||(new>27)) return -1;
	return new;
}
	
Date ReadDate(void){
	/*Verifica se o valor inserido para a data e valido*/
    int mes31, mes30, fev, anob, dia, mes, ano;
    Date aux;
    scanf("%d%d%d", &dia, &mes, &ano);
	
    mes31 = ((mes==1)||(mes==3)||(mes==5)||(mes==7)||(mes==8)||(mes==10)||(mes==12))&&(dia >= 32);
    mes30 = ((mes==4)||(mes==6)||(mes==9)||(mes==11))&&(dia >= 31);
    anob = ((ano%4==0)&&(ano%100!=0)||(ano%400==0));
    fev = (mes == 2)&& ((dia >= 29 && anob == 0)||(dia >= 30 && anob == 1));
	
    aux.dia = dia;
    aux.mes = mes;
    aux.ano = ano;
    if (mes31||mes30||fev||mes>13||dia<1||mes<1)
		aux.ano = -1;
    return aux;
}

RoomPtr ReadRoom (void){
	/*Verifica se o valor inserido para a sala e valido*/
	int aux;
	RoomPtr aux2;
	scanf("%i", &aux);
	/*procura na lista de salas se a sala existe*/
	aux2 = SearchRoomList(aux);
	if ((aux2!=NULL) && (aux2->next!=NULL) && (aux2->next->numero == aux))
		return aux2->next;
	return NULL;
}

UserPtr ReadUser (void){
	/*Le utilizador e valida*/
	UserPtr aux2, aux;
	char option;
	aux = CreateUserList();
	gets(aux->nome);
	/*Procura utilizador*/
	aux2 = SearchUserList(aux);
	if ((aux2!=NULL)&&(aux2->next!=NULL)&&(strcmp(aux2->next->nome,aux->nome)==0)){
		/*Encontrou utilizador*/
		if ((aux!=NULL)&&(aux->reservas!=NULL))
			free(aux->reservas);
		if (aux!=NULL)
			free(aux);
		return aux2->next;
	}
	/*Nao encontrou o utilizador - pergunta se queremos criar um novo*/
	printf("Utilizador nao encontrado. Pretende criar um novo utilizador? (y/n) ");
	option = getchar();
	while ((option!='y')&&(option!='n')) 
		option = getchar();
	if (option=='y') {
		InsertUser(aux);
		return aux;
	}
	else { /*Nao quer criar novo utilizador - liberta memoria alocada*/
		if ((aux!=NULL)&&(aux->reservas!=NULL))
			free(aux->reservas);
		if (aux!=NULL)
			free(aux);
		return NULL;
	}
}

ReservationPtr ReadReservation(){
	/*
	* Le dado
	* Valida
	* Liberta memoria alocada se nao introducao nao e valida
	*/
	ReservationPtr aux;
	aux = (ReservationPtr) malloc(sizeof(ReservationType));
	
	printf("Nome: ");
	aux->utilizador = ReadUser();
	if (aux->utilizador==NULL) {
		printf("Utilizador invalido. ");
		free(aux);
		return NULL;
	}
	
	printf("Sala: ");
	aux->sala = ReadRoom();
	if (aux->sala == NULL) {
		printf("Sala nao existente. ");
		free(aux);
		return NULL;
	}
	
	printf("Data: ");
	aux->data = ReadDate();
	if (compara_data(aux->data, hoje)==-1) {
		printf("Data invalida. ");
		free(aux);
		return NULL;
	}
	
	printf("Primeira Slot: ");
	aux->primeira = ReadSlot();
	if (aux->primeira==-1) {
		printf("Slot invalida. Tem de estar entre 0 e 27. ");
		free(aux);
		return NULL;
	}
	
	printf("Ultima Slot: ");
	aux->ultima = ReadSlot();
	if ((aux->ultima==-1)||(aux->ultima<aux->primeira)) {
		printf("Slot invalida. Tem de estar entre 0 e 27 e de ser maior ou igual que a primeira. ");
		free(aux);
		return NULL;
	}
	
	return aux;
}

void ListbyRoom (){
	RoomPtr lista = salas;
	ReservationPtr aux;
	while ((lista!=NULL)&&(lista->next!=NULL))	{
		lista = lista->next;
		
		if ((lista->reservas!= NULL)&&(lista->reservas->next!=NULL)) {
			aux = lista->reservas;
			printf("\n------------------- Sala %i: RESERVAS-------------------\n", lista->numero);
			while ((aux!=NULL)&&(aux->next!=NULL)) {
				aux = aux->next;
				PrintReservation(aux);
			}
		}
		if ((lista->prereservas!= NULL)&&(lista->prereservas->next!=NULL)) {
			aux = lista->prereservas;
			printf("\n----------------- Sala %i: PRE-RESERVAS-----------------\n", lista->numero);
			while ((aux!=NULL)&&(aux->next!=NULL)) { 
				aux = aux->next;
				PrintReservation(aux);
			}
		}
	}
    printf("\n\n");
}

void ListbyUser (void){
	UserPtr lista = users;
	UserResPtr aux;
	int cabecalho;
	while ((lista!=NULL)&&(lista->next!=NULL)){
		cabecalho = 0;
		lista = lista->next;
		aux = lista->reservas;
		while (aux!=NULL){
			if (aux->reserva!=NULL) {
				if (cabecalho == 0) printf("\n******** Utilizador: %s *********\n",lista->nome);
				PrintReservation(aux->reserva);
				cabecalho = 1;
			}
			aux = aux->next;
		}
	}
}

void ListRooms(void) {
	RoomPtr lista = salas;
	while ((lista !=NULL)&&(lista->next!=NULL)) 	{
		printf("%i, ",lista->numero);
		lista = lista->next;
	}
	printf("%i\n",lista->numero);
}
