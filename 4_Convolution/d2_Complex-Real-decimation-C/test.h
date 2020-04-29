#include <stddef.h>

struct _input {
    float *data_real;
    float *data_imag;
    size_t dlen;
    float *coeff;
    size_t clen;
} __attribute__ ((aligned (32)));

struct _result {
    float *data_real;
    float *data_imag;
    size_t dlen;
} __attribute__ ((aligned (32)));

void test_init(struct _result *, struct _input *);
double test_check(const struct _result *);

int test1(struct _result *, const struct _input *);
int test2(struct _result *, const struct _input *);
int test3(struct _result *, const struct _input *);
int test4(struct _result *, const struct _input *);
int test5(struct _result *, const struct _input *);
int test6(struct _result *, const struct _input *);
int test7(struct _result *, const struct _input *);
