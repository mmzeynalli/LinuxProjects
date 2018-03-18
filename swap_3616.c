#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 4096

int charToInt(char* num);

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    perror("Not sufficient number of arguments!");
    exit(1);
  }

  char* file = argv[1];
  int offset = charToInt(argv[2]);

  char buffBefore[offset + 1];
  char buffAfter[BUFFSIZE];


  int fdold = open(file, O_RDONLY);
  int fdnew = open("sysQuiz", O_CREAT | O_RDWR | O_TRUNC);
  int n;

  if (fdold < 0)
  {
    perror("Cannot open file!");
    exit(1);
  }

  if(n = read(fdold, buffBefore, offset) > offset)
  {
    perror("Out of bounds!");
    exit(1);
  }

  while((n = read(fdold, buffAfter, BUFFSIZE)) > 0)
  {
    if (write(fdnew, buffAfter, n) != n)
    {
      perror("writing failed");
      exit(1);
    }
  }  

  if (n < 0)
  {
    perror("reading failed");
    exit(1);
  }

  write(fdnew, buffBefore, offset);
  
  
  close(fdold);

  
  exit(0);
}
int charToInt(char* num)
{
  int n = 0, i = 0;

  for (i = strlen(num) - 1; i >= 0; i--)
  {
    n = (n * 10) + (num[i] - '0');
  }

  return n;
}
