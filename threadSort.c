#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000
#define MINSIZE 1000

typedef struct
{
	int* arr;
	int l, r;
} sortArgs;

void sort(int* arr, int l, int r);
void mergeArray(int* arr, int l, int m, int r);
void* thread_sort(void* args);
void swap(int* a, int* b);

pthread_t tid[SIZE];
int t = 0;

int main()
{
	int *a = calloc(SIZE, sizeof(int)); //initialize array
	srand(time(NULL)); //initialize random generator
	clock_t t1, t2;

	for(int i = 0; i < SIZE; i++)
		a[i] = rand() % 500; //fill array with random numbers


	t1 = clock();

	sort(a, 0, SIZE - 1); //start sorting with threads

	t2 = clock();

	printf ("Total time = %f seconds\n", (double) (t2 - t1) / CLOCKS_PER_SEC);

	//difference is 0.07 sec

	exit(0);

}

void sort(int* arr, int l, int r)
{
	int middle = (l + r) / 2;

	if(r == l) return;

	if(r - l == 1)
	{
		if(arr[l] > arr[r]) swap(&arr[l], &arr[r]);
	}
	else if(r - l <= MINSIZE) //if there is less than MINSIZE elements
		// better to use manual version over threads
	{
		sort(arr, l, middle);
		sort(arr, middle + 1, r);
		mergeArray(arr, l, middle, r);
	}
	else
	{
		sortArgs args;

		args.l = l;
		args.r = r;
		args.arr = arr;
		pthread_create(&tid[t], NULL, thread_sort, (void*) &args);
		pthread_join(tid[t++], NULL);
	}
}

void mergeArray(int* arr, int l, int m, int r)
{

	int n1 = m - l + 1;
	int n2 = r - m;


	int L[n1], R[n2];

	for (int i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (int j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];


	int i, j;
	i = j = 0;
	int k = l;

	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
			arr[k++] = L[i++];
		else
			arr[k++] = R[j++];
	}


	while (i < n1)
		arr[k++] = L[i++];

	while (j < n2)
		arr[k++] = R[j++];

}

void* thread_sort(void* arg)
{
	sortArgs* t = (sortArgs*)(arg);
	int middle = (t->l + t->r) / 2;

	sort(t->arr, t->l, middle);
	sort(t->arr, middle + 1, t->r);
	mergeArray(t->arr, t->l, middle, t->r);

	pthread_exit(0);
}

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

