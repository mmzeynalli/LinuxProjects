#include <stdio.h>
#include <stdlib.h>

void addToHead();
int removeFromHead();
void printList();

struct LinkedList
{
  int data;
  struct LinkedList *next;  

}; 

struct LinkedList *l;
int op;
int n;
int cnt;

int main()
{
  printf("Use any option:\n");
  printf("(1) add, (2) delete, (3) print (0) exit: ");

  scanf("%d", &op);

  while(op)
  {
    if(op == 1)
    {
      printf("Add a number to head: ");
      scanf("%d", &n);
      addToHead(n);
    }
    if(op == 2)
      removeFromHead();
    if(op == 3)
      printList();
  
    printf("(1) add, (2) delete, (3) print (0) exit: ");

    scanf("%d", &op);

  }

  exit(0);
}

void addToHead(int a)
{
  struct LinkedList* prev = l;
  l = malloc(sizeof(struct LinkedList));
  l->data = a;
  l->next = prev;

}

int removeFromHead()
{
  int removedData = l->data;

  l->data = l->next->data;
  l->next = l->next->next;

  return removedData;
}

void printList()
{
  struct LinkedList* i = l;

  while(i != NULL)
  { 
    printf("%d ", i->data);
    i = i->next;
  }
  printf("\n");
}
