#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 1
char *buff = NULL;

int main(int argc, char** argv)
{
  if (argc < 4)
  {
    perror("Not enough arguments!");
    exit(1);
  }

  int fid = open(argv[1], O_RDWR);
  int n;

  if (fid < 0)
  {
    perror("Cannot open file!");
    exit(1);
  }

  while((n = read(fid, buff, BUFFSIZE)) != (argv[2] - '0'))
  {
    printf("%s", buff);
  }

  lseek(fid, argv[3] - '0', SEEK_CUR);

  while((n = read(fid, buff, BUFFSIZE)) > 0)
  {
    printf("%s", buff);
  }
}
