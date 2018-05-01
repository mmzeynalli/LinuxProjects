/**
 * written by Miradil Zeynalli
 * as Final Project from
 * CSCI 3614 - Systems Programming
 *
 * 17.05.2017
 *
 */

/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>


#include <signal.h>
#include <sys/types.h>
#include <errno.h>
/* END OF INCLUDES */

/* DEFINES */
#define LINE_LEN 100
#define MAX_COMMANDS 20
#define MAX_ARGS 30
#define DELIMITERS " \r\n\0\t"

/* END OF DEFINES */

/* STRUCTURES */
typedef struct
{
  char** line;
  char** how;
}command_t;

/* END OF STRUCTURES */

/* GLOBAL VARIABLES */
command_t* cmd;
/* END OF GLOBAL VARIABLES */

/* FUNCTION PROTOTYPES */

void initStruct(); //initialize struct
void printPrompt(); //print prompt message
void readLine(char* buff); //get input line
void parseLine(char* line); //parse line into command and arguments

char* lookupPath(char **, char **);
int parsePath(char **);
void exec();
int executeFileInCommand(char *, char **, char *);
int executeFileOutCommand(char *, char **, char *);
void executePipedCommand(char **, char **, char *, char *);
/* END OF FUNCTION PROTOTYPES */

int main()
{
  char* line = calloc(LINE_LEN, sizeof(char));

  while(1)
  {
    initStruct();
    printPrompt();

    readLine(line);
    parseLine(line);
    exec(0);
  }

  exit(0);
}

void initStruct()
{
  cmd = calloc(MAX_COMMANDS, sizeof(*cmd));

  for(int i = 0; i < MAX_COMMANDS; i++)
  {
    cmd[i].line = calloc(MAX_ARGS, sizeof(char*));
    cmd[i].how = calloc(2, sizeof(char));
  }

}

void printPrompt()
{
  printf("in the nut shell > ");
}

void readLine(char* buff)
{
  fgets(buff, LINE_LEN, stdin);

  if(!(strcmp(buff, "exit")) || (!strcmp(buff, "quit")))
      exit(0);
}

void parseLine(char* line)
{
  //printf("parsed one is = %s\n", line);
  int i = 0, j = 0;
  char* temp;

  cmd[i].line[j] = strtok(line, DELIMITERS);

  while(cmd[i].line[j] != NULL)
  {

    temp = strtok(NULL, DELIMITERS);

    if(strcmp(temp, "&") || strcmp(temp, "&&") || strcmp(temp, "||") || strcmp(temp, "|")
       || strcmp(temp, ">") || strcmp(temp, ">>")) cmd[i].line[++j] = temp;
    else
      cmd[i++].how = temp;
  }


}

void exec(int in)
{


  pid_t pid;
  int st; //status

  //printf("command is = %s\n", *cmd[in].line);


  if((pid = fork()) < 0)
  {
    perror("Fork failed!\n");
    exit(1);
  }
  else if(!pid)
  {
    if(execvp(*(cmd[in].line), cmd[in].line) < 0)
    {
      perror("Execution Failed!\n");
      exit(1);
    }
  }
  else wait(&st);

}

