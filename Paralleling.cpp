#include "5_3_reverse.h"
#include <pthread.h>
#include "5_3_time.h"
#include <stdio.h>
typedef struct _ARGS
{
	double* matrix1;
	double* matrix2;
	double* res;
	int n;
	int thread_number;
	int total_threads;
} ARGS;

static long int threads_total_time = 0;
static pthread_mutex_t threads_total_time_mutex = PTHREAD_MUTEX_INITIALIZER;

#define N_TESTS 1
void Delta(int i, int j)
{
	if (i== j)
		return 1
	else
		return 0
}

void error(double* a, double*b, double* res, int size, int k, int total)
{
	int part_size = (int)(size*size/total);
	int start = (int)(size*size*k/total);
	for(int i = (int)(k * size/total), i < (int)((k+1) * size/total) + 1, i++)
	{
		for(int j = 0; j < size; j++){
			if(i*size + j >= start && i*size + j < start + part_size )
			{
				double sum = 0;
				for(int l = 0; l < size; l++)
					sum+= a[i*size + l]*b[l*size + j]
				sum-=Delta(i,j);
				res[0]+=sum*sum;
			}	
		}
	}	
}

void* error_threaded(void *pa)
{
	ARGS *pargs = (ARGS*)pa;
	long int t;
	int i;
	
	t = get_time();
	for(i = 0; i < N_TESTS; i++)
	{
		error(pargs->matrix1, pargs->matrix2, pargs->res,
		pargs->n, pargs->thread_number, pargs->total_threads);
	}
	t = get_time() - t;
	pthread_mutex_lock(&threads_total_time_mutex);
	threads_total_time+=t;
	pthread_mutex_unlock(&threads_total_time_mutex);
	return 0;
}

double Matrix::ErrorValue_threads(double* M){
	int nthreads;
	std::cout<<"Threads number"<<std::endl;
	std::cin>>nthreads;
	pthread_t* treads = (pthread_t*)malloc(nthreads * sizeof(pthread_t));
	ARGS* args = (ARGS*)malloc(nthreads * sizeof(ARGS));
	double* result = new double[1];
	for(int i = 0; i < nthreads; i++)
	{
		args[i].matrix1 = this->Check;
		args[i].matrix2 = M;
		args[i].res = result;
		args[i].n = this->size;
		args[i].thread_number = i;
		args[i].total_threads = nthreads;
	}
	t_full = get_full_time();
	for(int i = 0; i < nthreads; i++)
	{
		pthread_create(threads + i, 0, error_threaded, args+i)
	}
	t_full = get_full_time() - t_full;
	//result
	double res = result[0];
	std::cout<<"Total full time: "<<t_full<<"total threads time "<<threads_total_time<<"per thread"<<threads_total_time/nthreads<<std::endl;
	free(threads);
	free(args);
	delete[] result;
	return res;
}

