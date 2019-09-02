#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 4096

char* createAbsPath(char* dest);

int main(int argc, char **argv)
{
  if(argc < 2) 
  {
    perror("Not enough arguments");
    exit(1);
  }

  char *filename = argv[1];
  char *dest = argv[2];

  if(dest[0] != '/') dest = createAbsPath(dest); //Segmentation Fault, work on "mkdirs" command

  int fdold = open(filename, O_RDONLY);
  int fdnew = creat(dest, S_IRUSR | S_IWUSR);
  int n;
  char buff[BUFFSIZE];

  while((n = read(fdold, buff, BUFFSIZE)) > 0)
  {
    if (write(fdnew, buff, n) != n)
    {
      perror("writing failed");
    }
  }  

  if (n < 0)
    perror("reading failed");

  exit(0);
}


char* createAbsPath(char* dest)
{
  char *absPath = "/home/mmzeynalli";
  printf("%s", absPath);
  strncat(absPath, dest, strlen(dest));
  
  printf("%s", absPath);

  return absPath;
}
