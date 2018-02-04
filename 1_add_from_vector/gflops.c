#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include "gflops-test.h"

#define REPEAT (1000*1000)
#define FVECLEN 4000

float fvec[FVECLEN];
float32x4_t fvec32x4[FVECLEN/4];

void gfloat() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_float(&fvec[tid*thread_veclen], thread_veclen);
	}
    }
}

void gfloat4() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_float4(&fvec[tid*thread_veclen], thread_veclen);
	}
    }
}

void gfloat32x4() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/4/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_float32x4(&fvec32x4[tid*thread_veclen], thread_veclen);
	}
    }
}

void gfloat32x4_1thread() {
    printf("Number of threads = 1\n");
    for (int i=0; i<REPEAT; i++) {
	gtest_float32x4(fvec32x4, FVECLEN/4);
    }
}

int main() {
    struct timeval tstart, tend;
    double eltime_ms;

    gettimeofday(&tstart, NULL); gfloat(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Float --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.);

    gettimeofday(&tstart, NULL); gfloat4(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Float4 --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.);

    gettimeofday(&tstart, NULL); gfloat32x4(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Float32x4 --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.);

    gettimeofday(&tstart, NULL); gfloat32x4_1thread(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Float32x4 1thread --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.);

    return 0;
}
