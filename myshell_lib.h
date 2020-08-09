#ifndef _MYSHELL_LIB_H_
	#define _MYSHELL_LIB_H
	void mpipe(char*); // handles first FDs calls forks and calls run pipe
	void error(const char *msg);

	void remove_spaces(char *str);
	void parse(char *vector[10], char *line); //takes a takes array of chars splits it to words
	int parse_pipe(char str[1024],char*vector[80]); // creates an array of commands
	void parse_redirect_output(char str[2000], char* vector[80]);
	void parse_redirect_input(char str[2000], char* vector[80]);

	void run_pipe(int pfd[], char str[1024]);
	void execute(char *msg);
	void redirect_output(char str[2000]);
	void redirect_input(char str[2000]);
	void history(char* cwd);
#endif


//				  NOTES


//				  TODO
//	+implimentation of multiple pipes



//		For multiple pipes IDEA
//
//runpipe runing half pipe then calling it shelf giving an fd, the next command 
//and (if it has next command)?


//				COMPILE

// $ gcc -c myshell_lib.c -o myshell_lib.o
// $ gcc -c shell.c -o shell.o
// $ gcc shell.o myshell_lib.o -o shell
