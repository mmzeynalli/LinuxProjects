#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

/* DEFINES */

#define DIR_CUR "."
#define DIR_PREV ".."

#define COLOR_GREEN "\x1b[1;32m" // color for executables
#define COLOR_LIGHT_BLUE "\x1b[1;94m" // color for directories
#define COLOR_CYAN "\x1b[1;96m" // color for links
#define COLOR_BACKG_RED "\x1b[0;41m"
#define COLOR_RESET "\x1b[0m" // reset color settings

#define SIZE 10000
/* END OF DEFINES */

/* STRUCTURES */
typedef struct {

  char* name; // name of the file
  char* permission; // permission of the file
  char* color; // color due to type
  char exe; // type of file (directory, executable etc.)

  char* userName;  // username of the owner of the file
  char* groupName; // group name of the file owner

  time_t mtime; // modification time
  time_t ctime; // file status change time
  time_t atime; // access time

  off_t size; // size of file (in bytes)
  int inode;  // inode of the file
  int nlink;  // number of links
  int blocks; // number of blocks

  int userID;  // user id of the owner of the file
  int groupID; // group id of the owner of the file

  int isDir; // shows either file is directory or not


} fileInfo;

typedef struct 
{
  int show_hidden;
  int show_inode;
  int show_idNumbers;
  int show_inDIR;
  int show_Recursive;
  int show_insideOfDir;
  int show_CUR_PREV_DIR;
  int show_extensions;
  int show_longList;
  int show_readableB;
  int show_readableK;
  int show_color;
  int show_multiCol;
  int show_perLine;

  int no_sort;
  int sort_name;
  int sort_reverse;
  int sort_size;
  int sort_mtime; // modification time
  int sort_ctime; // status change time
  int sort_atime; // access time		

} Options;

/* END OF STRUCTURES */

/* GLOBAL VARIABLES */

DIR* dir;
struct dirent* file;
struct stat st; 
struct tm Date;
struct group* grp;
struct passwd* pwd;

int i; // for loop iterator
int j = 0; // iterator for dirsAndFiles array of strings
int cnt = 0; // count of files in directory
char* dirsAndFiles[SIZE]; // list of non-option of argv

Options op;
fileInfo *entry;

/* END OF GLOBAL VARIABLES */

/* FUNCTION PROTOTYPES */

void ls(); // lists all in the current directory
void initOptions(char* str); // initialize arguments
void initStruct(); // initializes Options struct
void storeFileInfo(); // store filenames in buffer and sort them
void sort(); // sort according to given options
void printAll(); // print file list with given options

int getNumOfFilesInDIR(); // number of files in current directory
int getNumOfHLinks(); // returns the number of hard links
int get_iNode(char* file); // finds i-node of file
int get_aTime(char* file); // access time
int get_cTime(char* file); // status change time
int get_mTime(char* file); // modification time
int getUserID(char* file); // id of user
int getGroupID(char* file); // id of group
char getExtension(char* COLOR); // depending on permission, prints extension
char* getPermission(char* file); // returns permissions of file
char* getColor(char* file); // depending on permissions, output color of file name changes
char* getUserName(char* file); // gets username depending on user id
char* getGroupName(char* file); // gets group name depending on group id

int cmpName(const void* s1, const void* s2); // comparator for sorting by alphebetical order
int cmpTime(const void* s1, const void* s2); // comparator for sorting by time
int cmpSize(const void* s1, const void* s2); // comparator for sorting by file size
char* strlwr(const char* s); // makes string to be lower cased

int isHidden(char* file); // checks if file is hidden or not

/* END OF FUNCTION PROTOTYPES */

int main(int argc, char** argv)
{

  if (argc < 1)
  {
    perror("Not enough arguments!\n");
    exit(1);
  }

  initStruct();
 
  dirsAndFiles[0] = DIR_CUR; // by default non-optional argument is current directory

  for (i = 1; i < argc; i++)
  {
    if(argv[i][0] == '-') initOptions(argv[i]);
    else dirsAndFiles[j++] = argv[i];
  }

  if(!op.show_Recursive) ls();

  

//---------------------------------------
    // Date = gmtime(&entry[i].mtime);
    // Date->tm_hour;
  
  closedir(dir);
  free(entry);

  exit(0);
  
//---------------------------------------
  
}

/* FUNCTIONS */

void initStruct()
{
  op.show_hidden = 0;
  op.show_inode = 0;
  op.show_idNumbers = 0;
  op.show_inDIR = 1;
  op.show_Recursive = 0;
  op.show_insideOfDir = 1;
  op.show_CUR_PREV_DIR = 0;
  op.show_extensions = 0;
  op.show_longList = 0;
  op.show_readableB = 0;
  op.show_readableK = 0;
  op.show_color = 1;
  op.show_multiCol = 1;
  op.show_perLine = 0;

  op.no_sort = 0;
  op.sort_name = 1;
  op.sort_reverse = 0;
  op.sort_size = 0;
  op.sort_mtime = 0;
  op.sort_ctime = 0;
  op.sort_atime = 0;

}

void initOptions(char* str)
{
  for(i = 1; i < strlen(str); i++)
  {
    switch(str[i])
    {
      case 'A': op.show_hidden = 1;
		op.show_CUR_PREV_DIR = 0; 
		break;
      case 'a': op.show_hidden = 1; 
		op.show_CUR_PREV_DIR = 1;
		break;
//      case 'C': op.show_multiCol = 0;
//		op.show_perLine = 0;
//		break;
      case 'c': op.sort_ctime = 1;
		op.sort_size = 0;
		op.sort_mtime = 0;
		op.sort_atime = 0;
		break;
      case 'd': op.show_insideOfDir = 0; 
		break;
      case 'F': op.show_extensions = 1; 
		break;
      case 'f': op.no_sort = 1;
		op.show_color = 0;
		op.show_hidden = 1;
		op.show_CUR_PREV_DIR = 1;
		break;
      case 'h': op.show_readableK = 1;
		op.show_readableB = 0; 
		break;
      case 'i': op.show_inode = 1; 
		break;
      case 'k': op.show_readableB = 1;
		break;
      case 'l': op.show_longList = 1; 
		op.show_perLine = 1;
		op.show_multiCol = 0;
		break;
      case 'n': op.show_longList = 1;
		op.show_idNumbers = 1; 
		op.show_perLine = 1;
		op.show_multiCol = 0;
		break;
      case 'R': op.show_Recursive = 1;
		break;
      case 'r': op.sort_reverse = 1;
		break;
      case 'S': op.sort_size = 1;
		op.sort_name = 0;
		op.sort_atime = 0;
		op.sort_ctime = 0;
		op.sort_mtime = 0;
		break;
      case 't': op.sort_mtime = 1;
		op.sort_name = 0;
		op.sort_size = 0;
		op.sort_atime = 0;
		op.sort_ctime = 0;
		break;
      case 'u': op.sort_atime = 1;
		op.sort_name = 0;
		op.sort_size = 0;
		op.sort_mtime = 0;
		op.sort_ctime = 0;
		break;
//      case 'x': op.show_multiCol = 0;
//		op.show_perLine = 0;
//		break;
        case '1': op.show_perLine = 1;
		op.show_multiCol = 0;
		break;

    }
  }
}

void ls()
{
  for(i = 0; i < j; i++)
  {
    cnt = getNumOfFilesInDIR();

    entry = calloc(cnt, sizeof(*entry));
  
    storeFileInfo();

    sort();
  
    printAll();
  }
}

void storeFileInfo()
{

  i = 0;

  while((file = readdir(dir)) != NULL)
  {
    char* name = file->d_name;

    entry[i].name = name;
    
    entry[i].permission = getPermission(name);
    if(entry[i].permission[0] == 'd') entry[i].isDir = 1;

    entry[i].color = getColor(name);
    entry[i].exe = getExtension(getColor(name));
    entry[i].atime = get_aTime(name);
    entry[i].ctime = get_cTime(name);
    entry[i].mtime = get_mTime(name);
    entry[i].nlink = getNumOfHLinks(name);
    entry[i].userID = getUserID(name);
    entry[i].groupID = getGroupID(name);
    entry[i].userName = getUserName(name);
    entry[i].groupName = getGroupName(name);
    entry[i].inode = get_iNode(name);

    i++;
  }

}

void sort()
{
  if(!op.no_sort)
  {
    if(op.sort_name) qsort(entry, cnt, sizeof(*entry), cmpName);
    else if(op.sort_size) qsort(entry, cnt, sizeof(*entry), cmpSize);
    else qsort(entry, cnt, sizeof(*entry), cmpTime);
  }
}

void printAll()
{
  for(i = 0; i < cnt; i++)
  {
    if (isHidden(entry[i].name) && !op.show_hidden) continue;
    if ((!strcmp(entry[i].name, DIR_CUR) || !strcmp(entry[i].name, DIR_PREV)) && !op.show_CUR_PREV_DIR) continue;
    if (op.show_inode) printf("%d ", entry[i].inode);

    if (op.show_longList)
    {
      printf("%s %d ", entry[i].permission, entry[i].nlink);

      if(op.show_idNumbers) printf("%d %d ", entry[i].userID, entry[i].groupID);
      else printf("%s %s", entry[i].userName, entry[i].groupName);

      printf("%lu ", entry[i].size);

      if(op.show_color)
      {
	printf("%s", entry[i].color);
      }

      printf("%s%s", entry[i].name, COLOR_RESET);
    }
  }
}
/* GET FUNCTIONS */

int getNumOfFilesInDIR()
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

int getNumOfHLinks(char* file)
{
  if(lstat(file, &st) == 0) return st.st_nlink;

  perror("Could not get number of hard links of the file");
  exit(1);
}

char* getPermission(char* file)
{
  char *modeVal = calloc(10, sizeof(char));
    
  if(lstat(file, &st) == 0)
  {
    mode_t perm = st.st_mode;
    
    if (S_ISREG(perm))  modeVal[0] = '-';
    if (S_ISDIR(perm))  modeVal[0] = 'd';
    if (S_ISLNK(perm))  modeVal[0] = 'l';
    if (S_ISCHR(perm))  modeVal[0] = 'c';
    if (S_ISBLK(perm))  modeVal[0] = 'b';
    if (S_ISSOCK(perm)) modeVal[0] = 's';
    if (S_ISFIFO(perm)) modeVal[0] = 'f';
    
    modeVal[1] = (perm & S_IRUSR) ? 'r' : '-';
    modeVal[2] = (perm & S_IWUSR) ? 'w' : '-';

    if (perm & S_IXUSR) modeVal[3] = 'x';
    else if (perm & S_ISUID) modeVal[3] = 's';
    else if (perm & (S_ISUID | S_IXUSR)) modeVal[3] = 'S';
    else modeVal[3] = '-';
    
    modeVal[4] = (perm & S_IRGRP) ? 'r' : '-';
    modeVal[5] = (perm & S_IWGRP) ? 'w' : '-';

    if (perm & S_IXGRP) modeVal[6] = 'x';
    else if (perm & S_ISGID) modeVal[6] = 's';
    else if (perm & (S_ISGID | S_IXGRP)) modeVal[6] = 'S';
    else modeVal[6] = '-';

    modeVal[6] = (perm & S_IXGRP) ? 'x' : '-';
    modeVal[7] = (perm & S_IROTH) ? 'r' : '-';
    modeVal[8] = (perm & S_IWOTH) ? 'w' : '-';
    modeVal[9] = (perm & S_IXOTH) ? 'x' : '-';
    modeVal[10] = '\0';

    return modeVal;     
  }
  else
  {
    return strerror(errno);
  }   

}

char* getColor(char *file)
{

  if(lstat(file, &st) == 0)
  {
    mode_t perm = st.st_mode;

    switch(perm & S_IFMT)
    {
      case S_IFLNK: return COLOR_CYAN;
      case S_IFDIR: return COLOR_LIGHT_BLUE;
      case S_IFSOCK: return COLOR_BACKG_RED;
      case S_IFREG: return COLOR_RESET;
    }
  }

  else
  {
    perror("Could not get permissions of file");
    return strerror(errno);
  }   
}

int getUserID(char* file)
{
  if(lstat(file, &st) == 0) return st.st_uid;

  perror("Could not user id of the file");
  exit(1);
}

int getGroupID(char* file)
{
  if(lstat(file, &st) == 0) return st.st_gid;

  perror("Could not get group id of the file");
  exit(1);
}

int get_iNode(char *file)
{
  if(lstat(file, &st) == 0) return st.st_ino;

  perror("Could not get inode of the file");
  exit(1);
}

int get_aTime(char *file)
{
  if(lstat(file, &st) == 0) return st.st_atime;

  perror("Could not get access time of the file");
  exit(1);
}

int get_cTime(char *file)
{
  if(lstat(file, &st) == 0) return st.st_ctime;

  perror("Could not get status change time of the file");
  exit(1);
}

int get_mTime(char *file)
{
  if(lstat(file, &st) == 0) return st.st_mtime;

  perror("Could not get modification of the file");
  exit(1);
}

char getExtension(char* COLOR)
{
  if(COLOR == COLOR_GREEN) return '*';    	  
  else if(COLOR == COLOR_LIGHT_BLUE) return '/';
  else if(COLOR == COLOR_CYAN) return '@';

}

char* getGroupName(char* file)
{
  grp = getgrgid(getGroupID(file));

  return grp->gr_name;

  perror("Could not get group name");
  exit(1);
}

char* getUserName(char* file)
{
  pwd = getpwuid(getUserID(file));

  return pwd->pw_name;
}

/* END OF GET FUNCTIONS */

/* COMPARATORS */
int cmpName(const void* a, const void* b)
{
  fileInfo* x = (fileInfo *) a;
  fileInfo* y = (fileInfo *) b;

  int returnValue = strcasecmp(x->name, y->name);

  return (!op.sort_reverse) ? returnValue : -returnValue;
}


int cmpTime(const void* a, const void* b)
{
  fileInfo* x = (fileInfo *) a;
  fileInfo* y = (fileInfo *) b;
  
  int returnValue;

  if(op.sort_atime) returnValue = x->atime - y->atime;
  if(op.sort_atime) returnValue = x->ctime - y->ctime;
  if(op.sort_atime) returnValue = x->mtime - y->mtime;

  return (!op.sort_reverse) ? returnValue : -returnValue;
}


int cmpSize(const void* a, const void* b)
{
  fileInfo* x = (fileInfo *) a;
  fileInfo* y = (fileInfo *) b;

  int returnValue = x->size - y->size;
  
  return (!op.sort_reverse) ? returnValue : -returnValue; 
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
/* END OF COMPARATORS */

int isHidden(char* file)
{
  return (file[0] == '.') ?  1 : 0;
}

/* END OF FUNCTIONS */
