all: PROJET EXCUTION
PROJET: main.c linked_list.c array.c fonction.h
	gcc -c -Wall main.c -Werror
	gcc -c -Wall linked_list.c -Werror
	gcc -c -Wall array.c -Werror
	gcc -o EXEC main.o linked_list.o array.o -lm
EXCUTION: EXEC
	./EXEC
