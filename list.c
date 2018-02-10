#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/* DEFINES */

#define DIR_CUR "."
#define DIR_PREV ".."

#define COLOR_GREEN "\x1b[1;32m" //color for executables
#define COLOR_LIGHT_BLUE "\x1b[1;94m" //color for directories
#define COLOR_CYAN "\x1b[1;96m"
#define COLOR_BACKG_RED "\x1b[0;41m"
#define COLOR_RESET "\x1b[0m"

/* END OF DEFINES */

/* FUNCTION PROTOTYPES */

int getNumberOfFilesInDIR(); // number of files in current directory
void storeAndSortFileNames(char*** buff); // store filenames in buffer and sort them
int cmp(const void* s1, const void* s2); // comporator for sorting
char* strlwr(const char* s); // makes string to be lower cased
void ls(); // lists all in the current directory
char* permColor(char* file); // depending on permissions, output color of file name changes
int isHiddenFile(char* file); // checks if file is hidden or not
void printExtension(char* COLOR); // depending on permission, prints extension
int inode(char* file); // finds i-node of file

/* END OF FUNCTION PROTOTYPES */


/* GLOBAL VARIABLES */

DIR* dir;
struct dirent* file;
struct stat st;
int i;
int cnt = 0;
char** buff = NULL; // for storing filenames
int toSort = 1; // 0 - not sort, 1 - sort

/* END OF GLOBAL VARIABLES */

int main(int argc, char** argv)
{

  if (argc < 1)
  {
    perror("Not enough arguments!");
    return strerror(errno);
  }
//----------------------------------------
  if (argc == 1) //correct swap file and sorting (- is not considered)
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      if(strcmp(buff[i], DIR_CUR) && strcmp(buff[i], DIR_PREV) && !isHiddenFile(buff[i]))
      	printf("%s%s%s  ", permColor(buff[i]), buff[i], COLOR_RESET);
    
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
      	printf("%s%s%s  ", permColor(buff[i]), buff[i], COLOR_RESET);
    
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
      	printf("%s%s%s  ", permColor(buff[i]), buff[i], COLOR_RESET);
    
    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }

//-----------------------------------------  
  if(!strcmp(argv[1], "-d")) //understand this argument
  {
    printf(COLOR_LIGHT_BLUE ".\n");
  }
//-----------------------------------------
  if(!strcmp(argv[1], "-F"))
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      if(strcmp(buff[i], DIR_CUR) && strcmp(buff[i], DIR_PREV) && !isHiddenFile(buff[i]))
      {
	printf("%s%s%s", permColor(buff[i]), buff[i], COLOR_RESET);
	printExtension(permColor(buff[i]));
        
      }

    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }

  if(!strcmp(argv[1], "-f"))
  {
    toSort = 0;
    ls();
    
    for(i = 0; i < cnt; i++)
      	printf("%s  ",buff[i]);
    
    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }
//--------------------------------------------------

  if (!strcmp(argv[1], "-i")) //correct swap file and sorting (- is not considered)
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      if(strcmp(buff[i], DIR_CUR) && strcmp(buff[i], DIR_PREV) && !isHiddenFile(buff[i]))
      	printf("%d %s%s%s  ", inode(buff[i]), permColor(buff[i]), buff[i], COLOR_RESET);
    
    printf("\n");

    free(buff);
    closedir(dir);

    return 0;
  }

  if(!strcmp(argv[1], "-1"))
  {
    ls();
    
    for(i = 0; i < cnt; i++)
      if(strcmp(buff[i], DIR_CUR) && strcmp(buff[i], DIR_PREV) && !isHiddenFile(buff[i]))
      	printf("%s%s%s\n", permColor(buff[i]), buff[i], COLOR_RESET);

    free(buff);
    closedir(dir);

    return 0;
  }
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

  if(toSort) qsort(*buff, cnt, sizeof(char *), cmp);
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

char* permColor(char *file)
{

  if(stat(file, &st) == 0)
  {
    mode_t perm = st.st_mode;

    if(perm & S_IFDIR) return COLOR_LIGHT_BLUE;
    if(perm & S_IXUSR) return COLOR_GREEN;
//    if(perm & S_IFLNK) return COLOR_CYAN;
	
    return COLOR_RESET;
  }
  else
  {
    perror("Could not get permissions of file");
    return strerror(errno);
  }   
}

int inode(char *file)
{
  if(stat(file, &st) == 0) return st.st_ino;

  perror("Could not get inode of the file"); 
}

int isHiddenFile(char* file)
{
  return (file[0] == '.') ?  1 : 0;
}

void printExtension(char* COLOR)
{
  if(COLOR == COLOR_GREEN) printf("*  ");    	  
  else if(COLOR == COLOR_LIGHT_BLUE) printf("/  ");
  else if(COLOR == COLOR_CYAN) printf("@  ");
}

/* END OF FUNCTIONS */

