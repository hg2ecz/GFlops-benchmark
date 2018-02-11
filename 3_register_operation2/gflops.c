#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include "gflops-test.h"

#define REPEAT (1000*1000)
#define FVECLEN 4096

float fvec[FVECLEN];

void gtest(float (*testfunc)(const float *, int), char *testname) {
    struct timeval tstart, tend;
    double eltime_ms;
    gettimeofday(&tstart, NULL);

    int nthreads, tid;
    #pragma omp parallel private (nthreads, tid)
    {
	nthreads = omp_get_num_threads(); // number of all thread
	tid = omp_get_thread_num(); // thread id
	if (tid == 0) printf("Number of threads = %d\n", nthreads);
	const int thread_veclen = FVECLEN/nthreads;

	for (int i=0; i<REPEAT; i++) {
	    (*testfunc)(&fvec[tid*thread_veclen], thread_veclen);
	}
    }

    gettimeofday(&tend, NULL);
    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("%s --> eltime: %3.6f ms, GFlops: %3.6f\n\n", testname, eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.*40./16.);
}

int main() {
    gtest(gtest_fmla_a, "Fmla_a");
    gtest(gtest_fmla_b, "Fmla_b");
    gtest(gtest_fmla_vec, "Fmla_vec");
    gtest(gtest_fmla_doublevec, "Fmla_doublevec");

#if defined (__AVX__)
    gtest(gtest_fmla_avx, "Fmla_AVX");
#endif

#if defined (__AVX512F__)
    gtest(gtest_fmla_avx512f, "Fmla_AVX512F");
#endif
    return 0;
}
