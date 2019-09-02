#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZE 4096

char buff[BUFFSIZE]; //buffer for reading and writing
char* err; //error message

int main(int argc, char** argv){
    
  if (argc != 4)
  {
    perror("Not sufficient number of arguments!");
    exit(1);     
  }
    
  int fd1 = open(argv[1], O_RDONLY);
  int fd2 = open(argv[2], O_RDONLY);
  int fd3 = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    
  int n;
    
  if (fd1 < 0)
  {    
    sprintf(err, "%s %s", argv[1], "file does not exist!"); //it gives error in my local machine
							    // (segmentation fault: core dumped)
							    //most probably because of virtual machine
							    // but works well in mac
    perror(err);
    exit(1);
  }

  if (fd2 < 0)
  {
    sprintf(err, "%s %s", argv[2], "file does not exist!");
    perror(err);
    exit(1);
  }

  if (fd3 < 0)
  {
    sprintf(err, "%s %s", argv[3], "file does not exist!");
    perror(err);
    exit(1);
  }
   
  while((n = read(fd1, buff, BUFFSIZE)) > 0)
  {
    if(write(fd3, buff, n) != n)
    {
      perror("Write of the first file failed!");
      exit(1);
    }
  }

  if (n < 0)
  {
    perror("Could not read first file!");
    exit(1);
  }
     
     
  while ((n = read(fd2, buff, BUFFSIZE)) > 0)
  {
    if (write(fd3, buff, n) != n) 
    {
      perror("Write of the second file failed!");
      exit(1);     
    }
  }
 
  if (n < 0) 
  { 
    perror("Could not read second file!");
    exit(1);
  }

  close(fd1);
  close(fd2);
  close(fd3);
     
  exit(0);
}
