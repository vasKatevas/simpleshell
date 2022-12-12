#include "myshell_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
   int flag, pid, fd[2];
   char user_input[2000];

   char history_file[100];    
   getcwd(history_file,100);
   strcat(history_file,"/myshell.history");


   while (1)
   {
       flag = 0;
       memset(user_input,0,sizeof(user_input));
       fprintf(stderr, "\x1b[01;34;1m[ shell ] -> \x1b[\033[00m");

       read(STDIN_FILENO, user_input, 255);

       FILE *history_fd = fopen(history_file, "a");

       char *parsed_input[10];

       int len = strlen(user_input);
      
       if(len == 1) //when user presses ENTER (len = 1) it gives Segmentation fault
       {
         continue;
       }
       fputs(user_input, history_fd);
       user_input[len - 1] = '\0';
       fprintf(stderr, "Running command: %s\n", user_input);

/*  ##########  redirection (>)  ##########  */
       if (strchr(user_input,'>') != NULL)
       {
           redirect_output(user_input);
           flag = 1;
       }

       if (strchr(user_input, '<') != NULL)
       {
           redirect_input(user_input);
           flag = 1;
       }

/*  ##########  pipe  #############  */
       if (strchr(user_input, '|') != NULL)
       {
           mpipe(user_input);
           flag = 1;
       }

       char tmp[2000];
       strcpy(tmp, user_input);
       parse(parsed_input, tmp);


<<<<<<< HEAD
       if (strcmp(parsed_input[0], ">>") == 0)
       {
           redirect_output_append(user_input);
           flag = 1;
       }

=======
>>>>>>> 9139d2f443ef930b6ece524add74d69fea3324f1
       /*  ##########  cd  #############  */
       if (strcmp(parsed_input[0], "cd") == 0)
       {
           chdir(parsed_input[1]);
           flag = 1;
       }
/*  #########  EXIT  #############  */
       if (strcmp(parsed_input[0], "exit") == 0)
       {
           remove(history_file); 
           exit(0);
       }
/*  ##########  history  #############  */
       if (strcmp(parsed_input[0], "history") == 0)
       {
           fclose(history_fd);
           history(history_file);

           flag = 1;
       }

/*  ##########  EXECUTE  #############  */

       if (flag == 0)
       {
           execute(user_input);
       }



       fclose(history_fd);
   }
}
