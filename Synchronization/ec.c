#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int circle = 0;
int point_per_thread;

pthread_mutex_t t = PTHREAD_MUTEX_INITIALIZER;

void* count(void* arg)
{
	long name = (long)arg;
	int thread_circle = 0;
	srand((double)time(NULL));
	double x, y;

	for(int i=0 ; i< point_per_thread; i++)
	{
		x = (double)rand()/(double)RAND_MAX;
		y = (double)rand()/(double)RAND_MAX;

		if((x*x) + (y*y) <= 1)
			thread_circle++;
	}

	printf("thread %ld: point_in_circle: %d point_total: %d\n", name+1, thread_circle, point_per_thread);
	pthread_mutex_lock(&t);
	circle += thread_circle;
	pthread_mutex_unlock(&t);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int num_thread = atoi(argv[1]);
	point_per_thread = atoi(argv[2]);
	int point_total = num_thread * point_per_thread;
	double pi = 0;
	pthread_t* pid = (pthread_t*)malloc(sizeof(pthread_t)*num_thread);
	
	for(long i =0; i<num_thread; i++)
		pthread_create(&pid[i], NULL, count, (void*)i);

	for(int i=0; i<num_thread; i++)
		pthread_join(pid[i], NULL);

	pthread_mutex_destroy(&t);

	pi = (double)circle/(double)point_total;
	printf("Total: point_in_circle: %d point_total: %d pi: %lf\n", circle, point_total, 4*pi);
	free(pid);
	return 0;
}
