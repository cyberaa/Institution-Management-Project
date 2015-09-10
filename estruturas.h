#include <stdio.h>
#include <stdlib.h>

#define MAX 128

typedef struct date
{
	int dia, mes, ano;
}Date;


/*Declaracao de ponteiros*/
typedef struct user *UserPtr;
typedef struct reservation *ReservationPtr;
typedef struct room *RoomPtr;
typedef struct user_res *UserResPtr;


/*Utilizadores*/
typedef struct user
{
    UserResPtr reservas;
    char nome[MAX];
    UserPtr next;
}UserType;

/*Lista de reservas do utilizador*/
typedef struct user_res
{
    UserResPtr next;
    ReservationPtr reserva;
}UserResType;

/*Reservas*/
typedef struct reservation
{
    RoomPtr sala;
    Date data;
    int primeira, ultima;
    UserPtr utilizador;
    ReservationPtr next;
    ReservationPtr prev;
}ReservationType;

/*Salas*/
typedef struct room
{
    int numero;
    ReservationPtr reservas;
    ReservationPtr prereservas;
    RoomPtr next;
}RoomType;

/*Variaveis globais*/
Date hoje;
RoomPtr salas;
UserPtr users;


/*Manipulacao de estruturas - Utilizadores*/
UserPtr CreateUserList (void); 	/*Inicializar lista*/
void DestroyUserList (void); 	/*Destruir lista*/
UserPtr SearchUserList (UserPtr); /*Procurar lugar onde inserir novo utilizador*/
UserPtr SearchUser (UserPtr); 	/*Procurar utilizador por nome*/
int DeleteUser (UserPtr); 		/*Apagar utilizador*/
int InsertUser (UserPtr); 		/*Inserir novo utilizador*/

/*Funcoes para lista de reservas do utilizador*/
UserResPtr CreateUserRes (void); 	/*Inicializar lista*/
void InsertUserRes (ReservationPtr);/*Inserir ponteiro para reserva na lista*/
int DeleteUserRes (ReservationPtr); /*Apagar uma reserva da lista*/
void DestroyUserRes (UserResPtr); 	/*Apagar toda a lista*/

/*Funcoes para lista de salas */
RoomPtr CreateRoomList (void);	/*Inicializar lista*/
RoomPtr SearchRoomList (int);	/*Procurar sala por numero*/
void InsertRoom (int);			/*Inserir sala*/
void DestroyRoomList(void);		/*Destruir lista*/

/*Funcoes para reservas*/
ReservationPtr CreateReservationList (void);	/*Criar lista de reservas*/
ReservationPtr SearchReservationList (ReservationPtr,ReservationPtr); /*Procurar reserva numa lista*/
int InsertReservation (ReservationPtr, int); 	/*Inserir reserva*/
int CreateNewReservation (ReservationPtr);		/*Criar reserva*/
int RemoveReservation (ReservationPtr);			/*Apagar reserva*/
int ReplaceReservation (RoomPtr);				/*Por como reserva as pre-reservas de slots ja livres*/
void PrintReservation (ReservationPtr);			/*Imprimir uma reserva*/
void DestroyReservationList (ReservationPtr);	/*Destruir lista de reservas*/

/*Funcoes para comparacoes*/
int compara_slot (ReservationPtr, ReservationPtr); /*Comparar slots (data e primeira slot)*/
int compara_data (Date, Date);		/*Comparar data*/

/*Funcoes para leitura*/
int ReadSlot (void);		/*Ler slot inserida por utilizador*/
Date ReadDate (void);		/*Ler data inserida por utilizador*/
RoomPtr ReadRoom (void);	/*Ler sala inserida por utilizador*/
UserPtr ReadUser (void);	/*Ler utilizador inserida por utilizador*/
ReservationPtr ReadReservation(void);	/*Ler e criar reserva*/

/*Funcoes para listagem*/
void ListbyUser (void);	/*Listar reservas por utilizador*/
void ListbyRoom (void); /*Listar reservas por sala*/
void ListRooms(void); /*Listar salas existentes*/


/*Funcoes para manipulacao de ficheiros*/
ReservationPtr ReadReservationFromFile(char us[], int s, int d,int m,int a,int p,int u);
Date LoadDate(int dia, int mes, int ano);
RoomPtr LoadRoom (int aux);
UserPtr LoadUser (char us[]);
int LoadReservation (ReservationPtr novo);
int LoadFile (void);
int SaveFile (void);
int LoadRooms (void);
