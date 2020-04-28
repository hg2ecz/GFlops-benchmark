#include <math.h>
#include <malloc.h>

#include "test.h"

#define DATALEN (200*1024)
#define COEFLEN 512

#if defined(__arm__) || defined(__aarch64__)
  #include <arm_neon.h>
  typedef float32x4_t VECTYPE;
  #define LOADSIMD(x) *((VECTYPE *)x)
  #define LOADSIMD_UNALIGNED(x) *((VECTYPE *)x)
#else
  #include <smmintrin.h>
  typedef __v4sf VECTYPE;
  #define LOADSIMD(x) *((VECTYPE *)x)
  #define LOADSIMD_UNALIGNED(x) _mm_loadu_ps(x)
#endif



void test_init(struct _result *result, struct _input *input) {
    input->dlen = DATALEN;
    input->data = malloc(DATALEN*sizeof(float));
    input->coeff = malloc(COEFLEN*sizeof(float));
    input->clen = COEFLEN;
    for (int i=0; i<input->dlen; i++) {
	input->data[i] = 1.0*sin(2*M_PI/1001*i);
    }
    for (int i=0; i<input->clen; i++) {
	input->coeff[i] = 0.9+0.1/(i+1.);
    }
    result->data = malloc(DATALEN*sizeof(float));
}

double test_check(const struct _result *result) {
    double res = 0.;
    for (int i=0; i<result->dlen; i++) {
	res += result->data[i];
    }
    return res;
}


int test1(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-1+1; i++) {
	float res = 0;
	for (int j=0; j<input->clen; j++) {
	    res += input->data[i+j] * input->coeff[j];
	}
	result->data[i] = res;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test2(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-4+1; i++) {
	VECTYPE res = {0., 0., 0., 0.};
	for (int j=0; j <= input->clen-4; j+=4) {
	    VECTYPE data = LOADSIMD_UNALIGNED(&input->data[i+j]);
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]);
	    res += data * coeff;
	}
	result->data[i] = res[0] + res[1] + res[2] + res[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test3(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-8+1; i+=4) {
	VECTYPE res0 = {0., 0., 0., 0.};
	VECTYPE res1 = {0., 0., 0., 0.};
	VECTYPE res2 = {0., 0., 0., 0.};
	VECTYPE res3 = {0., 0., 0., 0.};

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data;
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]); // 1 x load coeff

	    data = LOADSIMD(&input->data[i+j]);   res0 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+1]); res1 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+2]); res2 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+3]); res3 += data * coeff;
	}

	result->data[i]   = res0[0] + res0[1] + res0[2] + res0[3];
	result->data[i+1] = res1[0] + res1[1] + res1[2] + res1[3];
	result->data[i+2] = res2[0] + res2[1] + res2[2] + res2[3];
	result->data[i+3] = res3[0] + res3[1] + res3[2] + res3[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test4(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-8+1; i+=8) {

	VECTYPE res0 = {0., 0., 0., 0.};
	VECTYPE res1 = {0., 0., 0., 0.};
	VECTYPE res2 = {0., 0., 0., 0.};
	VECTYPE res3 = {0., 0., 0., 0.};

	VECTYPE res4 = {0., 0., 0., 0.};
	VECTYPE res5 = {0., 0., 0., 0.};
	VECTYPE res6 = {0., 0., 0., 0.};
	VECTYPE res7 = {0., 0., 0., 0.};

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data;
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]); // 1 x load coeff

	    data = LOADSIMD(&input->data[i+j]);   res0 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+1]); res1 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+2]); res2 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+3]); res3 += data * coeff;

	    data = LOADSIMD(&input->data[i+j+4]); res4 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data[i+j+5]); res5 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data[i+j+6]); res6 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data[i+j+7]); res7 += data * coeff;
	}

	result->data[i]   = res0[0] + res0[1] + res0[2] + res0[3];
	result->data[i+1] = res1[0] + res1[1] + res1[2] + res1[3];
	result->data[i+2] = res2[0] + res2[1] + res2[2] + res2[3];
	result->data[i+3] = res3[0] + res3[1] + res3[2] + res3[3];

	result->data[i+4] = res4[0] + res4[1] + res4[2] + res4[3];
	result->data[i+5] = res5[0] + res5[1] + res5[2] + res5[3];
	result->data[i+6] = res6[0] + res6[1] + res6[2] + res6[3];
	result->data[i+7] = res7[0] + res7[1] + res7[2] + res7[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test5(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-16+1; i+=16) {

	VECTYPE res0 = {0., 0., 0., 0.};
	VECTYPE res1 = {0., 0., 0., 0.};
	VECTYPE res2 = {0., 0., 0., 0.};
	VECTYPE res3 = {0., 0., 0., 0.};

	VECTYPE res4 = {0., 0., 0., 0.};
	VECTYPE res5 = {0., 0., 0., 0.};
	VECTYPE res6 = {0., 0., 0., 0.};
	VECTYPE res7 = {0., 0., 0., 0.};

	VECTYPE res8 = {0., 0., 0., 0.};
	VECTYPE res9 = {0., 0., 0., 0.};
	VECTYPE res10 = {0., 0., 0., 0.};
	VECTYPE res11 = {0., 0., 0., 0.};

	VECTYPE res12 = {0., 0., 0., 0.};
	VECTYPE res13 = {0., 0., 0., 0.};
	VECTYPE res14 = {0., 0., 0., 0.};
	VECTYPE res15 = {0., 0., 0., 0.};

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data;
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]); // 1 x load coeff

	    data = LOADSIMD(&input->data[i+j]);   res0 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+1]); res1 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+2]); res2 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+3]); res3 += data * coeff;

	    data = LOADSIMD(&input->data[i+j+4]); res4 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+5]); res5 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+6]); res6 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+7]); res7 += data * coeff;

	    data = LOADSIMD(&input->data[i+j+ 8]); res8  += data * coeff;
	    data = LOADSIMD(&input->data[i+j+ 9]); res9  += data * coeff;
	    data = LOADSIMD(&input->data[i+j+10]); res10 += data * coeff;
	    data = LOADSIMD(&input->data[i+j+11]); res11 += data * coeff;

	    data = LOADSIMD(&input->data[i+j+12]); res12 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data[i+j+13]); res13 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data[i+j+14]); res14 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data[i+j+15]); res15 += data * coeff;
	}

	result->data[i]   = res0[0] + res0[1] + res0[2] + res0[3];
	result->data[i+1] = res1[0] + res1[1] + res1[2] + res1[3];
	result->data[i+2] = res2[0] + res2[1] + res2[2] + res2[3];
	result->data[i+3] = res3[0] + res3[1] + res3[2] + res3[3];

	result->data[i+4] = res4[0] + res4[1] + res4[2] + res4[3];
	result->data[i+5] = res5[0] + res5[1] + res5[2] + res5[3];
	result->data[i+6] = res6[0] + res6[1] + res6[2] + res6[3];
	result->data[i+7] = res7[0] + res7[1] + res7[2] + res7[3];

	result->data[i+8]  = res8[0]  + res8[1] + res8[2] + res8[3];
	result->data[i+9]  = res9[0]  + res9[1] + res9[2] + res9[3];
	result->data[i+10] = res10[0] + res10[1] + res10[2] + res10[3];
	result->data[i+11] = res11[0] + res11[1] + res11[2] + res11[3];

	result->data[i+12] = res12[0] + res12[1] + res12[2] + res12[3];
	result->data[i+13] = res13[0] + res13[1] + res13[2] + res13[3];
	result->data[i+14] = res14[0] + res14[1] + res14[2] + res14[3];
	result->data[i+15] = res15[0] + res15[1] + res15[2] + res15[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

#if defined (__AVX__)
#include <immintrin.h>
  typedef __v8sf AVXTYPE;
  #define LOADAVX(x) *((AVXTYPE *)x)
  #ifdef __clang__
     #define LOADAVX_UNALIGNED(x) *((AVXTYPE *)x)
  #elif __GNUC__
     #define LOADAVX_UNALIGNED(x) _mm256_loadu_ps(x)
  #else
     #error "CLANG or GCC? Other!"
  #endif

int test6(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-16+1; i+=16) {
	AVXTYPE res0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res3 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res7 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res8  = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res9  = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res10 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res11 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res12 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res13 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res14 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res15 = {0., 0., 0., 0., 0., 0., 0., 0.};

	for (int j=0; j<=input->clen-8; j+=8) {
	    AVXTYPE data;
	    AVXTYPE coeff = LOADAVX(&input->coeff[j]); // 1 x load coeff

	    data = LOADAVX(&input->data[i+j]);   res0 += data * coeff;
	    data = LOADAVX(&input->data[i+j+1]); res1 += data * coeff;
	    data = LOADAVX(&input->data[i+j+2]); res2 += data * coeff;
	    data = LOADAVX(&input->data[i+j+3]); res3 += data * coeff;

	    data = LOADAVX(&input->data[i+j+4]); res4 += data * coeff;
	    data = LOADAVX(&input->data[i+j+5]); res5 += data * coeff;
	    data = LOADAVX(&input->data[i+j+6]); res6 += data * coeff;
	    data = LOADAVX(&input->data[i+j+7]); res7 += data * coeff;

	    data = LOADAVX_UNALIGNED(&input->data[i+j+ 8]); res8  += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data[i+j+ 9]); res9  += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data[i+j+10]); res10 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data[i+j+11]); res11 += data * coeff;

	    data = LOADAVX(&input->data[i+j+12]); res12 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data[i+j+13]); res13 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data[i+j+14]); res14 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data[i+j+15]); res15 += data * coeff;
	}

	result->data[i]   = res0[0] + res0[1] + res0[2] + res0[3] + res0[4] + res0[5] + res0[6] + res0[7];
	result->data[i+1] = res1[0] + res1[1] + res1[2] + res1[3] + res1[4] + res1[5] + res1[6] + res1[7];
	result->data[i+2] = res2[0] + res2[1] + res2[2] + res2[3] + res2[4] + res2[5] + res2[6] + res2[7];
	result->data[i+3] = res3[0] + res3[1] + res3[2] + res3[3] + res3[4] + res3[5] + res3[6] + res3[7];

	result->data[i+4] = res4[0] + res4[1] + res4[2] + res4[3] + res4[4] + res4[5] + res4[6] + res4[7];
	result->data[i+5] = res5[0] + res5[1] + res5[2] + res5[3] + res5[4] + res5[5] + res5[6] + res5[7];
	result->data[i+6] = res6[0] + res6[1] + res6[2] + res6[3] + res6[4] + res6[5] + res6[6] + res6[7];
	result->data[i+7] = res7[0] + res7[1] + res7[2] + res7[3] + res7[4] + res7[5] + res7[6] + res7[7];

	result->data[i+8]  = res8[0]  + res8[1] + res8[2] + res8[3] + res8[4] + res8[5] + res8[6] + res8[7];
	result->data[i+9]  = res9[0]  + res9[1] + res9[2] + res9[3] + res9[4] + res9[5] + res9[6] + res9[7];
	result->data[i+10] = res10[0] + res10[1] + res10[2] + res10[3] + res10[4] + res10[5] + res10[6] + res10[7];
	result->data[i+11] = res11[0] + res11[1] + res11[2] + res11[3] + res11[4] + res11[5] + res11[6] + res11[7];

	result->data[i+12] = res12[0] + res12[1] + res12[2] + res12[3] + res12[4] + res12[5] + res12[6] + res12[7];
	result->data[i+13] = res13[0] + res13[1] + res13[2] + res13[3] + res13[4] + res13[5] + res13[6] + res13[7];
	result->data[i+14] = res14[0] + res14[1] + res14[2] + res14[3] + res14[4] + res14[5] + res14[6] + res14[7];
	result->data[i+15] = res15[0] + res15[1] + res15[2] + res15[3] + res15[4] + res15[5] + res15[6] + res15[7];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}
#endif
