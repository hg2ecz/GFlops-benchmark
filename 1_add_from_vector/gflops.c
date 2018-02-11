#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include "gflops-test.h"

#define REPEAT (1000*1000)
#define FVECLEN 4096

float fvec[FVECLEN];
VECTYPE fvec32x4[FVECLEN/4];

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
    printf("%s --> eltime: %3.6f ms, GFlops: %3.6f\n\n", testname, eltime_ms, (double)REPEAT*FVECLEN/eltime_ms/1000./1000.);
}

int main() {
    gtest(gtest_float, "Float");
    gtest(gtest_float4, "Float4");
    gtest(gtest_float16, "Float16");
    gtest(gtest_float32, "Float32");
    gtest(gtest_float_vec, "Float32x4");
    return 0;
}
