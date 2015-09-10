#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estruturas.h"

#define MAXCMD 16



int main(void) {
	salas = CreateRoomList();
	users = CreateUserList();
	
	system("clear");
	printf("\nBem vindo ao Instituto João Jesus!\n");
	printf("Insira a data de hoje... ");
	hoje = ReadDate();
	if (hoje.ano == -1) {
		hoje.dia = 0;
		hoje.mes = 0;
		hoje.ano = 0;
	}
	
	/*Carregar salas do ficheiro salas.txt*/
	if (LoadRooms()==-1) {
		InsertRoom(1);
		InsertRoom(2);
		InsertRoom(3);
	}
	
	/*Carregar reservas do ficheiro reservas.txt*/
	LoadFile();
	
	printf("\n***********Comandos disponiveis:******************\n");
	printf("*\"adicionar\"\tadicionar uma reserva/pre-reserva*\n");
	printf("*\"remover\"\tremover uma reserva/pre-reserva\t *\n");
	printf("*\"listar\"\tlistar reservas e pre-reservas\t *\n");
	printf("*\"guardar\"\tguardar dados num ficheiro\t *\n");
	printf("*\"info\"\t\tinformacoes sobre salas e slots\t *\n");
	printf("*\"sair\"\t\tsair\t\t\t\t *\n");
	printf("*\"ajuda\"\tpara ver esta lista novamente\t *\n");
	printf("**************************************************\n\n");
	char option, cmd[MAXCMD];

	while (1)
	{
		do {
			printf(">>>");
			gets(cmd);
		} while (strlen(cmd)<1);
		
		if (strcmp(cmd,"sair")==0) break;
			
		else if (strcmp(cmd,"ajuda")==0) {
			system("clear");
			printf("\n***********Comandos disponiveis:******************\n");
			printf("*\"adicionar\"\tadicionar uma reserva/pre-reserva*\n");
			printf("*\"remover\"\tremover uma reserva/pre-reserva\t *\n");
			printf("*\"listar\"\tlistar reservas e pre-reservas\t *\n");
			printf("*\"guardar\"\tguardar dados num ficheiro\t *\n");
			printf("*\"info\"\t\tinformacoes sobre salas e slots\t *\n");
			printf("*\"sair\"\t\tsair\t\t\t\t *\n");
			printf("*\"ajuda\"\tpara ver esta lista novamente\t *\n");
			printf("**************************************************\n\n");
		}
		else if (strcmp(cmd,"listar")==0) {
			printf("Quer listar por utilizador ou por sala? (u/s)\n");
			option = getchar();
			while ((option!='u')&&(option!='s')) option = getchar();
			if (option == 'u') ListbyUser();
			else ListbyRoom();
		}
		else if (strcmp(cmd,"adicionar")==0) {
			ReservationPtr nova;
			nova = ReadReservation();
			if (nova == NULL){
				printf("Reserva nao inserida.\n");
			}
			else{
				CreateNewReservation(nova);
			}
			if (nova!=NULL) PrintReservation(nova);
			getchar();
		}
		else if (strcmp(cmd,"remover")==0){
			ReservationPtr nova;
			int erro;
			nova = ReadReservation();
			erro = RemoveReservation(nova);
			if ((nova == NULL)||(erro==-1))
				printf("Reserva nao existente.\n");
			else{
				ReplaceReservation(nova->sala);
				printf("Reserva removida. Pré-reservas actualizadas.\n");
			}
			free(nova);
			getchar();
		}
		else if (strcmp(cmd,"guardar")==0){
			SaveFile();
			printf("a salvar\n");
		}
		else if (strcmp(cmd,"info")==0){
			printf("Sabia que...\n");
			printf("\nHoje e dia %02i/%02i/%04i?\n",hoje.dia, hoje.mes, hoje.ano);
			printf("\nExistem estas salas? ");
			ListRooms();
			printf("\nSe o utilizador que quiser reservar a sala nao existir tera a hipotese de o adicionar?\n");
			printf("\nExistem 28 slots (numeradas de 0 a 27) com uma duracao de 30 mins cada entre as 10h e as 24h de cada dia?\n");
		}
		else
			printf("Comando inválido. Para obter uma lista de comandos, escreva \"ajuda\".\n");
	}
	printf("Está a sair. Deseja guardar? y/n\n");
	option = getchar();
	while ((option!='y')&&(option!='n')) option = getchar();
	if (option=='y') {
		printf("a salvar\n"); 
		SaveFile();
	}
	printf("Até à próxima!\n");
	DestroyRoomList();
	DestroyUserList();
	free(users);
}
