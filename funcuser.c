#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"

UserPtr CreateUserList (void){
	/*Inicializar lista de utilizadores*/
    UserPtr aux;
    aux = (UserPtr) malloc (sizeof(UserType));
    aux->reservas = CreateUserRes(); /*Inicializa lista de reservas do user*/
    aux->next = NULL;
    return aux;
}

void DestroyUserList (void){
    UserPtr temp;
    while (users!=NULL)    {
        temp = users;
        users = users->next;
        if (temp->reservas!=NULL) 
			DestroyUserRes (temp->reservas); /*Destruir lista de reservas do utilizador*/
        free(temp);
    }
}

int DeleteUser (UserPtr chave){
	/*Procura o utilizador*/
    UserPtr ant,aux;
	ant = SearchUser(chave);
    if (ant==NULL) 
		return -1;
		
	/*Destroi as reservas*/
    aux = ant->next;
    if (aux->reservas != NULL) 
		DestroyUserRes(aux->reservas);
    
	/*Destroi utilizador*/
	ant->next = aux->next;
    free(aux);
    return 0;
}

UserPtr SearchUserList (UserPtr chave){
	/*
	* Procura sitio onde inserir utilizador chave.
	* ant sera o ponteiro para o elemento anterior
	* aux sera o ponteiro para o actual (para facilitar leitura do programa, nao estritamente necessario
	* o ponteiro ant é o ponteiro devolvido
	*/
	UserPtr ant, aux;
	ant = users;
	aux = users->next;
	while ((aux!=NULL)&&(strcmp(aux->nome, chave->nome))<0)	{
		ant = aux;
		aux = aux->next;
	}
	return ant;
}

UserPtr SearchUser (UserPtr chave){
	/*
	* usa a funcao SearchUserList para procurar o elemento anterior
	* se encontrar e o nome de utilizador coincidir com o elemento seguinte, devolve o ponteiro para o actual
	* senao devolve NULL
	*/
	UserPtr aux = SearchUserList (chave);
    if ((aux->next!=NULL) && (strcmp(aux->next->nome,chave->nome)==0))
		return aux;
    return NULL;
}

int InsertUser (UserPtr novo){
	/*Poe novo utilizador na lista por ordem alfabetica*/
    UserPtr ant = SearchUserList(novo);
    novo->next = ant->next;
    ant->next = novo;
    return 0;
}

UserResPtr CreateUserRes(void){
	/*Inicializa lista de ponteiros para reservas */
    UserResPtr aux;
    aux = (UserResPtr) malloc (sizeof(UserResType));
    aux->next = NULL;
    aux->reserva = NULL;
    return aux;
}

void InsertUserRes(ReservationPtr item){
	/*Insere um ponteiro para uma reserva na lista de reservas do user*/
	UserResPtr novo = CreateUserRes();
	novo->reserva = item;
	novo->next = item->utilizador->reservas->next;
    item->utilizador->reservas->next = novo;
}

int DeleteUserRes(ReservationPtr item){
	UserResPtr temp, aux;
	if (item->utilizador == NULL) 
		return -1;
	aux = item->utilizador->reservas;
	temp = NULL;
	while ((aux!=NULL)&&(aux->next!=NULL)&&(aux->next->reserva!=item)){
		temp = aux;
		aux = aux->next;
	}
	if (!((aux!=NULL)&&(aux->reserva!=NULL) &&(strcmp(aux->reserva->utilizador->nome,item->utilizador->nome)==0)&&(compara_slot(aux->reserva, item)==0)&&(aux->reserva->ultima == item->ultima))){
		return -1;
	}
	if ((temp!= NULL)&&(aux!=NULL)) 
		temp->next = aux->next;
	free(aux);
	if (item->utilizador->reservas == NULL) 
		DeleteUser(item->utilizador);
	return 0;
}

void DestroyUserRes (UserResPtr lista){
	/*Destroi lista de reservas do utilizador*/
    UserResPtr temp;
    while (lista!=NULL) {
        temp = lista;
        lista = lista->next;
		/*Apaga a reserva na lista da sala*/
        if (temp->reserva != NULL)
			RemoveReservation(temp->reserva);
        free(temp);
    }
}
