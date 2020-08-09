CC		= gcc
RM		= rm -f *.o


default: shell

shell: main.c myshell_lib.c myshell_lib.h
		$(CC) -c main.c -o main.o
		$(CC) -c myshell_lib.c -o myshell_lib.o
		$(CC) main.o myshell_lib.o -o shell
		$(RM)

main.o: main.c
		$(CC) -c main.c -o main.o
		$(CC) main.o myshell_lib.o -o shell
		$(RM)

myshell_lib.o: myshell_lib.c myshell_lib.h
		$(CC) -c myshell_lib.c -o myshell_lib.o
		$(CC) main.o myshell_lib.o -o shell
		$(RM)

clean:	
		$(RM) shell
			
