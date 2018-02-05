#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include "gflops-test.h"

#define REPEAT (1000*1000)
#define FVECLEN 4000

float fvec[FVECLEN];

void gfmla_a() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_fmla_a(&fvec[tid*thread_veclen], thread_veclen);
	}
    }
}

void gfmla_b() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_fmla_b(&fvec[tid*thread_veclen], thread_veclen);
	}
    }
}

void gfmla_c() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_fmla_c(&fvec[tid*thread_veclen], thread_veclen);
	}
    }
}

void gfmla_c2() {
    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    gtest_fmla_c2(&fvec[tid*thread_veclen], thread_veclen);
	}
    }
}

int main() {
    struct timeval tstart, tend;
    double eltime_ms;

    gettimeofday(&tstart, NULL); gfmla_a(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Fmla_a --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.*4);

    gettimeofday(&tstart, NULL); gfmla_b(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Fmla_b --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.*4);

    gettimeofday(&tstart, NULL); gfmla_c(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Fmla_c --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.*4);

    gettimeofday(&tstart, NULL); gfmla_c2(); gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("Fmla_c2 --> eltime: %3.6f ms, GFlops: %3.6f\n\n", eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.*4);

    return 0;
}
