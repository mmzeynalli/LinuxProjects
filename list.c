#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

/* DEFINES */

/* END OF DEFINES */

/* FUNCTION PROTOTYPES */

/* END OF FUNCTION PROTOTYPES */


/* GLOBAL VARIABLES */

DIR* dir;
struct dirent* file;
int i;
int cnt = 0;

/* END OF GLOBAL VARIABLES */

int main(int argc, char** argv)
{

  if (argc < 1)
  {
    exit(1);
  }
//----------------------------------------
  if (argc == 1)
  {
    dir = opendir(".");
    char **buff;

    buff = calloc(100, sizeof(char *));
    
    while((file = readdir(dir)) != NULL)
    {
      sprintf(buff[cnt++], "%s", file->d_name);
      //buff[cnt++] = file->d_name;
    }
    
    for(i = 0; i < cnt; i++)
      printf("%s  ", buff[i]);

    printf("\n");

    return 0;
  }

  for (i = 0; i < argc; i++)
  {
    printf("%s", argv[i]);

    //check for arguments
  }

//-----------------------------------------  
	

  return 0;
}

/* FUNCTIONS */

/* END OF FUNCTIONS */

