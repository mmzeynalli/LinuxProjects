#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

struct passwd* pwd;
struct stat st;

char* getUserName(char* file);
int getUserID(char* file);

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    perror("Not enough arguments!");
    exit(1);
  }
  
  for (int i = 1; i < argc; i++)
  {
    printf("Owner of '%s' file is %s\n", argv[i], getUserName(argv[i]));
  }

  exit(0);
}

char* getUserName(char* file)
{
  pwd = getpwuid(getUserID(file));

  return pwd->pw_name;
}

int getUserID(char* file)
{
  if(lstat(file, &st) == 0) return st.st_uid;

  perror("Could not get inode of the file");
  exit(1);
}

