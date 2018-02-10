#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

/* DEFINES */

#define DIR_CUR "."
#define DIR_PREV ".."

#define COLOR_GREEN "\x1b[1;32m" //color for executables
#define COLOR_TURQUOISE "\x1b[1;96m" //color for directories
#define COLOR_BACKG_RED "\x1b[0;41m"
#define COLOR_RESET "\x1b[0m"

/* END OF DEFINES */

/* FUNCTION PROTOTYPES */

int getNumberOfFilesInDIR(); //number of files in current directory
void storeAndSortFileNames(char*** buff); //store filenames in buffer and sort them
int cmp(const void* s1, const void* s2); //comporator for sorting
char* strlwr(const char* s); //makes string to be lower cased
void ls(); //lists all in the current directory

/* END OF FUNCTION PROTOTYPES */


/* GLOBAL VARIABLES */

DIR* dir;
struct dirent* file;
int i;
int cnt = 0;
char** buff = NULL; // for storing filenames

/* END OF GLOBAL VARIABLES */

int main(int argc, char** argv)
{

  if (argc < 1)
  {
    exit(1);
  }
//----------------------------------------
  if (argc == 1) //correct swap file and sorting (- is not considered)
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      if(strcmp(buff[i], DIR_CUR) && strcmp(buff[i], DIR_PREV))
      	printf("%s%s%s  ", COLOR_TURQUOISE, buff[i], COLOR_RESET);
    
    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }
//---------------------------------------
  if(!strcmp(argv[1], "-A"))
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      if(strcmp(buff[i], DIR_CUR) && strcmp(buff[i], DIR_PREV))
      	printf("%s  ", buff[i]);
    
    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }
//--------------------------------------
  if(!strcmp(argv[1], "-a"))
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      	printf("%s  ", buff[i]);
    
    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }

//-----------------------------------------  
	

  return 0;
}

/* FUNCTIONS */

void ls()
{

  cnt = getNumberOfFilesInDIR(); 
  buff = calloc(cnt, sizeof(char*));
    
  cnt = 0; //reset value for next iterations

  storeAndSortFileNames(&buff);

}

int getNumberOfFilesInDIR()
{
  int n = 0;
  dir = opendir(DIR_CUR);
  
  while((file = readdir(dir)) != NULL)
  { 
    n++;
  }

  rewinddir(dir);

  return n; //all files + 2 directories (. & ..)
}

void storeAndSortFileNames(char*** buff)
{

  while((file = readdir(dir)) != NULL)
  { 
    (*buff)[cnt++] = file->d_name;
  }

  qsort(*buff, cnt, sizeof(char *), cmp);
}

int cmp(const void* s1, const void* s2)
{
  const char* a = *(const char**) s1;
  const char* b = *(const char**) s2;

  return strcmp(strlwr(a), strlwr(b));
}

char* strlwr(const char* s)
{
  char* temp;

  temp = calloc(strlen(s), sizeof(char));
  
  for(i = 0; i < strlen(s); i++)
  {
    temp[i] = tolower(s[i]);
  }
  
  return temp;
}

/* END OF FUNCTIONS */

