#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


void remove_spaces(char *str)
{
    const char *d = str;
    do
    {
        while (*d == ' ')
        {
            ++d;
        }
    } while (*str++ = *d++);
}


void error(const char *msg)
{
   perror(msg);
   exit(1);
}


void parse(char *parsed[10], char *line)
{

   int i;

   char *word;
   word = strtok(line, " ");
   i = 0;
   while (word != NULL)
   {
       parsed[i] = word;
       word = strtok(NULL, " ");
       i++;
   }
   parsed[i] = NULL;
   int k = 0;
  
}



int parse_pipe(char input[1024],char*result[80])
{
   char *command;
   int i = 0;
   /* get the first token */
   command = strtok(input, "|");

   /* walk through other tokens */
   while (command != NULL)
   {
       result[i] = command;

       command = strtok(NULL, "|");
       i++;
   }
   return i;
}


void parse_redirect_output(char input[2000], char* parsed[80]){
   char *token;
   int i = 0;
   token = strtok(input, ">");

   while (token != NULL){
       parsed[i] = token;

       token = strtok(NULL,">");
       i++;
   }
}


void parse_redirect_input(char input[2000], char* parsed[80]){
   char *token;
   int i = 0;
   token = strtok(input, "<");

   while (token != NULL){
       parsed[i] = token;

       token = strtok(NULL,"<");
       i++;
   }
}

void run_pipe(int pfd[], char input[1024])
{
   int pid,p2;

   char *command1[10], *command2[10];
   char *parsed_input[80];

   int count = parse_pipe(input, parsed_input);
   fprintf(stderr, "number of commands: %d\n", count);

   parse(command1, parsed_input[0]);
   parse(command2, parsed_input[1]);

   switch (pid = fork())
   {

   case 0: //child
       dup2(pfd[0], 0);
       close(pfd[1]);
       execvp(command2[0], command2);
       perror(command2[0]);
   default:  //parent
       dup2(pfd[1], 1);
       close(pfd[0]);
       execvp(command1[0], command1);
       perror(command1[0]);

   }
}



void mpipe(char *input)        
{
    int fd[2], pid, status;
    pipe(fd);
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(1);
    case 0: /* child */
        run_pipe(fd, input);
        exit(0);
    default: /* parent */
        while ((pid = wait(&status)) != -1)
        {
            fprintf(stderr, "process %d exits with %d\n",
                    pid, WEXITSTATUS(status));
        }
    }
    close(fd[0]);
    close(fd[1]);
    close(fd[2]);
}





void execute(char *input){

    char *parsed_input[10];

    parse(parsed_input, input);
    int pid, ppid, status;
    printf("%s\n", parsed_input[0]);

    
    switch (pid = fork()) 
    {
    case -1: /*check for error*/
        perror("fork");
        exit(1);

    case 0: /*The child process*/
        execvp(parsed_input[0], parsed_input);
        fprintf(stdout, "exec: %s\n", strerror(errno));
        exit(0);

    default: /*The parent process*/
        ppid = getpid();
        while ((pid = wait(&status)) != -1)
        {
            fprintf(stderr, "process %d exits with %d\n",
                    pid, WEXITSTATUS(status));
        }
    }
}

void redirect_output(char str[2000]){

   char *parsed_input[80];

   parse_redirect_output(str,parsed_input);
   remove_spaces(parsed_input[1]);
   int filefd = open(parsed_input[1], O_WRONLY | O_CREAT, 0660);
   dup2(filefd, 1);
   execute(parsed_input[0]);
   fdatasync(filefd);
   if (close(filefd) != 0)
   {
       fprintf(stderr, "error closing file descriptor \n");
   }


}

void redirect_input(char str[2000]){

    char *parsed_input[80];
    parse_redirect_input(str, parsed_input);
    remove_spaces(parsed_input[1]);
    int filefd = open(parsed_input[1], O_RDONLY,0);
    dup2(filefd, STDIN_FILENO);
    execute(parsed_input[0]);
    if (close(filefd) != 0)
    {
        fprintf(stderr, "error closing file descriptor \n");
   }
   fflush(STDIN_FILENO);


}

void history(char* file)
{
    char ch;

    FILE *fp = fopen(file, "r"); 
    while ((ch = fgetc(fp)) != EOF)
        printf("%c", ch);
    fclose(fp);
}
