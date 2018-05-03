#include <stdio.h>
#include <pthread.h>

#define SIZE 100
#define MINSIZE 25

void sort(int* arr, int l, int r);
void thread_sort(struct* args);

int main()
{
	int *a = calloc(SIZE, sizeof(int));
	int err;
	pthread_t tid1, tid2;

	for(int i = 0; i < SIZE; i++)
		scanf("%d", a + i);
	
	sort(a, 0, SIZE);

	for(int i = 0; i < SIZE; i++)
		printf("%d ", a[i]);
	
}

void sort(int* arr, int l, int r)
{
	int middle = (l + r) / 2;

	if(r == l) return;

  if(r - l == 1)
	{
		if(arr[l] > arr[r]) swap(arr[l], arr[r]);
	}
	else if(r - l < MINSIZE)
	{
		sort(arr, l, middle);
		sort(arr, middle + 1, r);
	}
	else
	{
		pthread_t tid;
		struct *args;
		args->arr = arr;
		args->l = l;
		args->r = r;
		pthread_create(&tid, NULL, thread_sort, args);
	}
}

void thread_sort(struct* args)
{
	int middle = (l + r) / 2;

	sort(args->arr, args->l, middle);
	sort(args->arr, middle + 1, args->r);
}

