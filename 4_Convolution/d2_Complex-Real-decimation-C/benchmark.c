#include <stdio.h>
#include <sys/time.h>
#include "test.h"

#define REPEAT 1

void testfn(int (*testfunc)(struct _result *, const struct _input *), struct _result *result, struct _input *input, const char *testname) {
    struct timeval tstart, tend;
    double eltime_ms;

    gettimeofday(&tstart, NULL);
    for (int i=0; i<REPEAT; i++) {
	(*testfunc)(result, input);
    }
    gettimeofday(&tend, NULL);

    eltime_ms = 1000.*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec)/1000.;
    printf("%s --> eltime: %3.6f ms, result: %f\n", testname, eltime_ms, test_check(result));
}

int main() {
    struct _input input __attribute__ ((aligned (32)));
    struct _result result __attribute__ ((aligned (32)));
    test_init(&result, &input);

    testfn(test1, &result, &input, "test1");
    testfn(test2, &result, &input, "test2");
    testfn(test3, &result, &input, "test3");
    testfn(test4, &result, &input, "test4");
    testfn(test5, &result, &input, "test5");
#if defined (__AVX__)
    testfn(test6, &result, &input, "t3AVX");
    testfn(test7, &result, &input, "t4AVX");
#endif
    puts("");
    return 0;
}
