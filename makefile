##====================================================
##
##
##	Autores:
##	Rosa Manuela Rodrigues Faria Nº 2005128014
##	Vasco Bruno dos Santos Gouveia Nº 2001106666
##
##====================================================

CC	= gcc -g
OBJS	=  menu.o files.o funcreservas.o funcsalas.o funcuser.o interface.o
PROG	= ppp

# GENERIC

all:		${PROG}

clean:
		rm ${OBJS} ${PROG}

${PROG}:	${OBJS}
		${CC}  ${OBJS} -o $@


.c.o:
		${CC} $< -c -o $@


###########################################


files.o: files.c estruturas.h

funcreservas.o: funcreservas.c

funcsalas.o: funcsalas.c

funcuser.o: funcuser.c

interface.o: interface.c

menu.o:	menu.c
