#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estruturas.h"

int LoadFile (void) {
	/* 1ª linha - Utilizador, 2ª - sala, dia, mes, ano, primeira, ultima*/
	FILE *DadosRes;
	char us[MAX];
	int s,d,m,a,p,u, erro;
	ReservationPtr aux = NULL;
	DadosRes = fopen("reservas.txt","r");
	if (DadosRes == NULL)
	{
		printf("Ficheiro reservas.txt nao existente.\n");
		return -1;
	}
	do {
		fgets(us, MAX, DadosRes);
	}
	while (strlen(us)<1); //atencao ao EOF
	if (us==NULL) return -1;
	//us[strlen(us)] = '\0';
	while ((us)&&(strlen(us)>1))	{
		aux = NULL;
		erro = fscanf(DadosRes, "%i %i %i %i %i %i\n", &s, &d, &m, &a, &p, &u);
		if (erro==6) {
			aux = ReadReservationFromFile(us, s, d, m, a, p, u);
		}
		if (aux!=NULL) {
			LoadReservation(aux);
		}
		us[0]='\0';
		fgets(us, MAX, DadosRes);
	}
	fclose(DadosRes);
}

ReservationPtr ReadReservationFromFile(char us[], int s, int d,int m,int a,int p,int u){
	ReservationPtr aux;
	aux = (ReservationPtr) malloc(sizeof(ReservationType));
	
	aux->utilizador = LoadUser(us);
	if (aux->utilizador==NULL) {
		free(aux);
		return NULL;
	}
	
	aux->sala = LoadRoom(s);
	if (aux->sala == NULL) {
		free(aux);
		return NULL;
	}
	
	aux->data = LoadDate(d,m,a);
	if (compara_data(aux->data, hoje)==-1) {
		free(aux);
		return NULL;
	}
	
	if ((p>=0)&&(p<=27))
		aux->primeira = p;
	else {
		free(aux);
		return NULL;
	}
	
	if ((u>=0)&&(u<=27)&&u>=p)
		aux->ultima = u;
	else {
		free(aux);
		return NULL;
	}
	
	return aux;
}

Date LoadDate(int dia, int mes, int ano){
	/*Verifica se o valor inserido para a data e valido*/
    int mes31, mes30, fev, anob;
    Date aux;
	
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

RoomPtr LoadRoom (int aux){
	RoomPtr aux2;
	aux2 = SearchRoomList(aux);
	if ((aux2!=NULL) && (aux2->next!=NULL) && (aux2->next->numero == aux))
		return aux2->next;
	return NULL;
}

UserPtr LoadUser (char *us){
	UserPtr aux2, aux;
	aux = CreateUserList();
	us[strlen(us)] ='\0';
	strcpy(aux->nome,us);
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
	else {
		InsertUser(aux);
		return aux;
	}
}

int LoadReservation (ReservationPtr novo){
	/*Tenta inserir a reserva na lista das reservas*/
	int flag = InsertReservation(novo,1);
	if (flag==-1){
		enqueue(novo);
		InsertUserRes(novo);
		return 0;
	}
	else if (flag==-2) free(novo);
}

int SaveFile (void) {
	FILE *DadosRes;
	DadosRes = fopen("reservas.txt","w");
	if (DadosRes == NULL) return -1;
	
	RoomPtr lista = salas;
	ReservationPtr aux;
	while ((lista!=NULL)&&(lista->next!=NULL))	{
		lista = lista->next;
		if ((lista->reservas!= NULL)&&(lista->reservas->next!=NULL)) {
			aux = lista->reservas;
			while ((aux!=NULL)&&(aux->next!=NULL)) {
				aux = aux->next;
				fprintf(DadosRes, "%s\n",aux->utilizador->nome);
				fprintf(DadosRes, "%i %i %i %i %i %i\n",aux->sala->numero, aux->data.dia, aux->data.mes, aux->data.ano, aux->primeira, aux->ultima);
			}
		}
		if ((lista->prereservas!= NULL)&&(lista->prereservas->next!=NULL)) {
			aux = lista->prereservas;
			while ((aux!=NULL)&&(aux->next!=NULL)) { 
				aux = aux->next;
				fprintf(DadosRes, "%s\n",aux->utilizador->nome);
				fprintf(DadosRes, "%i %i %i %i %i %i\n",aux->sala->numero, aux->data.dia, aux->data.mes, aux->data.ano, aux->primeira, aux->ultima);
			}
		}
	}
	fclose(DadosRes);
}

int LoadRooms (void) {
    FILE *DadosRoom;
    int num;
    DadosRoom = fopen("salas.txt","r");
    if (DadosRoom == NULL)
    {
        printf("Ficheiro salas.txt nao existente. A carregar salas por default.\n");
		return -1;
    }
    while (fscanf(DadosRoom, "%i", &num) != EOF)
    {
        InsertRoom (num);
    }
    fclose (DadosRoom);
    return 0;
}	
	
