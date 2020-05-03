#include <stdio.h>

/*
Rust:
-----
pub struct Input {
    data_real: Vec<f32>,
    data_imag: Vec<f32>,
    coeff: Vec<f32>,
}

pub struct Output {
    data_real: Vec<f32>,
    data_imag: Vec<f32>,
}

fn testfunc(output: *const Output, input: *const Input) -> i32;
*/

struct _float_vec {
    float *data;
    size_t capacity;
    size_t len;
};

struct _in_rust {
    struct _float_vec data_real;
    struct _float_vec data_imag;
    struct _float_vec coeff;
};

struct _out_rust {
    struct _float_vec data_real;
    struct _float_vec data_imag;
};

#include "test.h"
int testfn(int (*testfunc)(struct _result *, const struct _input *), void *result_raw, const void *input_raw) {
    struct _in_rust *inrust = (struct _in_rust *)input_raw;
    struct _out_rust *outrust = (struct _out_rust *)result_raw;

    // mapping to original C struct
    struct _input test_input = {
	.data_real = inrust->data_real.data,
	.data_imag = inrust->data_imag.data,
	.dlen = inrust->data_real.len,
	.coeff = inrust->coeff.data,
	.clen = inrust->coeff.len,
    };

    struct _result test_result = {
	.data_real = outrust->data_real.data,
	.data_imag = outrust->data_imag.data,
	.dlen = 0,
    };
    // Call original C function
    (*testfunc)(&test_result, &test_input);

    // Non pointertype result back to Rust struct
    outrust->data_real.len = test_result.dlen < outrust->data_real.capacity ? test_result.dlen : outrust->data_real.capacity;
    outrust->data_imag.len = test_result.dlen < outrust->data_imag.capacity ? test_result.dlen : outrust->data_imag.capacity;
    return 0;
}

// Call from Rust
// --------------
int testrust1(void *result, void *input) {
    return testfn(test1, result, input);
}

int testrust2(void *result, void *input) {
    return testfn(test2, result, input);
}

int testrust3(void *result, void *input) {
    return testfn(test3, result, input);
}

int testrust4(void *result, void *input) {
    return testfn(test4, result, input);
}

int testrust5(void *result, void *input) {
    return testfn(test5, result, input);
}

#if defined (__AVX__)
int testrust6(void *result, void *input) {
    return testfn(test6, result, input);
}

int testrust7(void *result, void *input) {
    return testfn(test7, result, input);
}
#endif
