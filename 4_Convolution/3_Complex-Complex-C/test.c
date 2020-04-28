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
    input->data_real = malloc(DATALEN*sizeof(float));
    input->data_imag = malloc(DATALEN*sizeof(float));
    input->coeff_real = malloc(COEFLEN*sizeof(float));
    input->coeff_imag = malloc(COEFLEN*sizeof(float));
    input->clen = COEFLEN;
    for (int i=0; i<input->dlen; i++) {
	input->data_real[i] = 1.0*sin(2*M_PI/1001*i);
	input->data_imag[i] = 1.1*sin(2.131*M_PI/1001*i);
    }
    for (int i=0; i<input->clen; i++) {
	input->coeff_real[i] = 0.9+0.1/(i+1.);
	input->coeff_imag[i] = 0.8+0.11/(i+1.);
    }
    result->data_real = malloc(DATALEN*sizeof(float));
    result->data_imag = malloc(DATALEN*sizeof(float));
}

double test_check(const struct _result *result) {
    double res = 0.;
    for (int i=0; i<result->dlen; i++) {
	res += result->data_real[i] + result->data_imag[i];
    }
    return res;
}


int test1(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-1+1; i++) {
	float res_re = 0;
	float res_im = 0;
	for (int j=0; j<input->clen; j++) {
	    res_re += input->data_real[i+j] * input->coeff_real[j] - input->data_imag[i+j] * input->coeff_imag[j];
	    res_im += input->data_real[i+j] * input->coeff_imag[j] + input->data_imag[i+j] * input->coeff_real[j];
	}
	result->data_real[i] = res_re;
	result->data_imag[i] = res_im;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test2(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-4+1; i++) {
	VECTYPE res_re = {0., 0., 0., 0.};
	VECTYPE res_im = {0., 0., 0., 0.};
	for (int j=0; j <= input->clen-4; j+=4) {
	    VECTYPE coeff_real = LOADSIMD(&input->coeff_real[j]);
	    VECTYPE coeff_imag = LOADSIMD(&input->coeff_imag[j]);
	    VECTYPE data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j]);
	    VECTYPE data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j]);
	    res_re += data_real * coeff_real - data_imag * coeff_imag;
	    res_im += data_real * coeff_imag + data_imag * coeff_real;
	}
	result->data_real[i] = res_re[0] + res_re[1] + res_re[2] + res_re[3];
	result->data_imag[i] = res_im[0] + res_im[1] + res_im[2] + res_im[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test3(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-8+1; i+=4) {
	VECTYPE res_re0 = {0., 0., 0., 0.};
	VECTYPE res_re1 = {0., 0., 0., 0.};
	VECTYPE res_re2 = {0., 0., 0., 0.};
	VECTYPE res_re3 = {0., 0., 0., 0.};

	VECTYPE res_im0 = {0., 0., 0., 0.};
	VECTYPE res_im1 = {0., 0., 0., 0.};
	VECTYPE res_im2 = {0., 0., 0., 0.};
	VECTYPE res_im3 = {0., 0., 0., 0.};

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data_real;
	    VECTYPE data_imag;
	    VECTYPE coeff_real = LOADSIMD(&input->coeff_real[j]); // 1 x load coeff
	    VECTYPE coeff_imag = LOADSIMD(&input->coeff_imag[j]); // 1 x load coeff

	    data_real = LOADSIMD(&input->data_real[i+j]);
	    data_imag = LOADSIMD(&input->data_imag[i+j]);
	    res_re0 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im0 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+1]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+1]);
	    res_re1 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im1 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+2]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+2]);
	    res_re2 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im2 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+3]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+3]);
	    res_re3 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im3 += data_real * coeff_imag + data_imag * coeff_real;
	}

	result->data_real[i]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3];
	result->data_real[i+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3];
	result->data_real[i+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3];
	result->data_real[i+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3];

	result->data_imag[i]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3];
	result->data_imag[i+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3];
	result->data_imag[i+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3];
	result->data_imag[i+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test4(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-8+1; i+=8) {

	VECTYPE res_re0 = {0., 0., 0., 0.};
	VECTYPE res_re1 = {0., 0., 0., 0.};
	VECTYPE res_re2 = {0., 0., 0., 0.};
	VECTYPE res_re3 = {0., 0., 0., 0.};

	VECTYPE res_re4 = {0., 0., 0., 0.};
	VECTYPE res_re5 = {0., 0., 0., 0.};
	VECTYPE res_re6 = {0., 0., 0., 0.};
	VECTYPE res_re7 = {0., 0., 0., 0.};

	VECTYPE res_im0 = {0., 0., 0., 0.};
	VECTYPE res_im1 = {0., 0., 0., 0.};
	VECTYPE res_im2 = {0., 0., 0., 0.};
	VECTYPE res_im3 = {0., 0., 0., 0.};

	VECTYPE res_im4 = {0., 0., 0., 0.};
	VECTYPE res_im5 = {0., 0., 0., 0.};
	VECTYPE res_im6 = {0., 0., 0., 0.};
	VECTYPE res_im7 = {0., 0., 0., 0.};

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data_real;
	    VECTYPE data_imag;
	    VECTYPE coeff_real = LOADSIMD(&input->coeff_real[j]); // 1 x load coeff
	    VECTYPE coeff_imag = LOADSIMD(&input->coeff_imag[j]); // 1 x load coeff

	    data_real = LOADSIMD(&input->data_real[i+j]);
	    data_imag = LOADSIMD(&input->data_imag[i+j]);
	    res_re0 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im0 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD(&input->data_real[i+j+1]);
	    data_imag = LOADSIMD(&input->data_imag[i+j+1]);
	    res_re1 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im1 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD(&input->data_real[i+j+2]);
	    data_imag = LOADSIMD(&input->data_imag[i+j+2]);
	    res_re2 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im2 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD(&input->data_real[i+j+3]);
	    data_imag = LOADSIMD(&input->data_imag[i+j+3]);
	    res_re3 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im3 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD(&input->data_real[i+j+4]);
	    data_imag = LOADSIMD(&input->data_imag[i+j+4]);
	    res_re4 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im4 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+5]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+5]);
	    res_re5 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im5 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+6]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+6]);
	    res_re6 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im6 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+7]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+7]);
	    res_re7 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im7 += data_real * coeff_imag + data_imag * coeff_real;
	}

	result->data_real[i]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3];
	result->data_real[i+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3];
	result->data_real[i+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3];
	result->data_real[i+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3];

	result->data_real[i+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3];
	result->data_real[i+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3];
	result->data_real[i+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3];
	result->data_real[i+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3];

	result->data_imag[i]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3];
	result->data_imag[i+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3];
	result->data_imag[i+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3];
	result->data_imag[i+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3];

	result->data_imag[i+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3];
	result->data_imag[i+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3];
	result->data_imag[i+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3];
	result->data_imag[i+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test5(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-16+1; i+=16) {

	VECTYPE res_re0 = {0., 0., 0., 0.};
	VECTYPE res_re1 = {0., 0., 0., 0.};
	VECTYPE res_re2 = {0., 0., 0., 0.};
	VECTYPE res_re3 = {0., 0., 0., 0.};

	VECTYPE res_re4 = {0., 0., 0., 0.};
	VECTYPE res_re5 = {0., 0., 0., 0.};
	VECTYPE res_re6 = {0., 0., 0., 0.};
	VECTYPE res_re7 = {0., 0., 0., 0.};

	VECTYPE res_re8 = {0., 0., 0., 0.};
	VECTYPE res_re9 = {0., 0., 0., 0.};
	VECTYPE res_re10 = {0., 0., 0., 0.};
	VECTYPE res_re11 = {0., 0., 0., 0.};

	VECTYPE res_re12 = {0., 0., 0., 0.};
	VECTYPE res_re13 = {0., 0., 0., 0.};
	VECTYPE res_re14 = {0., 0., 0., 0.};
	VECTYPE res_re15 = {0., 0., 0., 0.};

	VECTYPE res_im0 = {0., 0., 0., 0.};
	VECTYPE res_im1 = {0., 0., 0., 0.};
	VECTYPE res_im2 = {0., 0., 0., 0.};
	VECTYPE res_im3 = {0., 0., 0., 0.};

	VECTYPE res_im4 = {0., 0., 0., 0.};
	VECTYPE res_im5 = {0., 0., 0., 0.};
	VECTYPE res_im6 = {0., 0., 0., 0.};
	VECTYPE res_im7 = {0., 0., 0., 0.};

	VECTYPE res_im8 = {0., 0., 0., 0.};
	VECTYPE res_im9 = {0., 0., 0., 0.};
	VECTYPE res_im10 = {0., 0., 0., 0.};
	VECTYPE res_im11 = {0., 0., 0., 0.};

	VECTYPE res_im12 = {0., 0., 0., 0.};
	VECTYPE res_im13 = {0., 0., 0., 0.};
	VECTYPE res_im14 = {0., 0., 0., 0.};
	VECTYPE res_im15 = {0., 0., 0., 0.};

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data_real;
	    VECTYPE data_imag;
	    VECTYPE coeff_real = LOADSIMD(&input->coeff_real[j]); // 1 x load coeff
	    VECTYPE coeff_imag = LOADSIMD(&input->coeff_imag[j]); // 1 x load coeff

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j]);
	    res_re0 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im0 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+1]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+1]);
	    res_re1 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im1 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+2]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+2]);
	    res_re2 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im2 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+3]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+3]);
	    res_re3 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im3 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+4]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+4]);
	    res_re4 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im4 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+5]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+5]);
	    res_re5 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im5 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+6]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+6]);
	    res_re6 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im6 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+7]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+7]);
	    res_re7 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im7 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+ 8]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+ 8]);
	    res_re8 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im8 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+ 9]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+ 9]);
	    res_re9 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im9 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+10]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+10]);
	    res_re10 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im10 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+11]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+11]);
	    res_re11 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im11 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+12]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+12]);
	    res_re12 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im12 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+13]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+13]);
	    res_re13 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im13 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+14]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+14]);
	    res_re14 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im14 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADSIMD_UNALIGNED(&input->data_real[i+j+15]);
	    data_imag = LOADSIMD_UNALIGNED(&input->data_imag[i+j+15]);
	    res_re15 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im15 += data_real * coeff_imag + data_imag * coeff_real;
	}

	result->data_real[i]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3];
	result->data_real[i+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3];
	result->data_real[i+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3];
	result->data_real[i+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3];

	result->data_real[i+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3];
	result->data_real[i+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3];
	result->data_real[i+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3];
	result->data_real[i+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3];

	result->data_real[i+8]  = res_re8[0]  + res_re8[1] + res_re8[2] + res_re8[3];
	result->data_real[i+9]  = res_re9[0]  + res_re9[1] + res_re9[2] + res_re9[3];
	result->data_real[i+10] = res_re10[0] + res_re10[1] + res_re10[2] + res_re10[3];
	result->data_real[i+11] = res_re11[0] + res_re11[1] + res_re11[2] + res_re11[3];

	result->data_real[i+12] = res_re12[0] + res_re12[1] + res_re12[2] + res_re12[3];
	result->data_real[i+13] = res_re13[0] + res_re13[1] + res_re13[2] + res_re13[3];
	result->data_real[i+14] = res_re14[0] + res_re14[1] + res_re14[2] + res_re14[3];
	result->data_real[i+15] = res_re15[0] + res_re15[1] + res_re15[2] + res_re15[3];

	result->data_imag[i]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3];
	result->data_imag[i+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3];
	result->data_imag[i+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3];
	result->data_imag[i+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3];

	result->data_imag[i+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3];
	result->data_imag[i+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3];
	result->data_imag[i+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3];
	result->data_imag[i+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3];

	result->data_imag[i+8]  = res_im8[0]  + res_im8[1] + res_im8[2] + res_im8[3];
	result->data_imag[i+9]  = res_im9[0]  + res_im9[1] + res_im9[2] + res_im9[3];
	result->data_imag[i+10] = res_im10[0] + res_im10[1] + res_im10[2] + res_im10[3];
	result->data_imag[i+11] = res_im11[0] + res_im11[1] + res_im11[2] + res_im11[3];

	result->data_imag[i+12] = res_im12[0] + res_im12[1] + res_im12[2] + res_im12[3];
	result->data_imag[i+13] = res_im13[0] + res_im13[1] + res_im13[2] + res_im13[3];
	result->data_imag[i+14] = res_im14[0] + res_im14[1] + res_im14[2] + res_im14[3];
	result->data_imag[i+15] = res_im15[0] + res_im15[1] + res_im15[2] + res_im15[3];
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
    for (int i=0; i <= input->dlen - input->clen-8+1; i+=8) {
	AVXTYPE res_re0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re3 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_re4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re7 = {0., 0., 0., 0., 0., 0., 0., 0.};


	AVXTYPE res_im0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im3 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_im4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im7 = {0., 0., 0., 0., 0., 0., 0., 0.};

	for (int j=0; j<=input->clen-8; j+=8) {
	    AVXTYPE data_real;
	    AVXTYPE data_imag;
	    AVXTYPE coeff_real = LOADAVX_UNALIGNED(&input->coeff_real[j]); // 1 x load coeff
	    AVXTYPE coeff_imag = LOADAVX_UNALIGNED(&input->coeff_imag[j]); // 1 x load coeff

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j]);
	    res_re0 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im0 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+1]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+1]);
	    res_re1 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im1 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+2]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+2]);
	    res_re2 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im2 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+3]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+3]);
	    res_re3 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im3 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+4]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+4]);
	    res_re4 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im4 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+5]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+5]);
	    res_re5 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im5 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+6]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+6]);
	    res_re6 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im6 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+7]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+7]);
	    res_re7 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im7 += data_real * coeff_imag + data_imag * coeff_real;
	}

	result->data_real[i]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3] + res_re0[4] + res_re0[5] + res_re0[6] + res_re0[7];
	result->data_real[i+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3] + res_re1[4] + res_re1[5] + res_re1[6] + res_re1[7];
	result->data_real[i+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3] + res_re2[4] + res_re2[5] + res_re2[6] + res_re2[7];
	result->data_real[i+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3] + res_re3[4] + res_re3[5] + res_re3[6] + res_re3[7];

	result->data_real[i+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3] + res_re4[4] + res_re4[5] + res_re4[6] + res_re4[7];
	result->data_real[i+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3] + res_re5[4] + res_re5[5] + res_re5[6] + res_re5[7];
	result->data_real[i+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3] + res_re6[4] + res_re6[5] + res_re6[6] + res_re6[7];
	result->data_real[i+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3] + res_re7[4] + res_re7[5] + res_re7[6] + res_re7[7];


	result->data_imag[i]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3] + res_im0[4] + res_im0[5] + res_im0[6] + res_im0[7];
	result->data_imag[i+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3] + res_im1[4] + res_im1[5] + res_im1[6] + res_im1[7];
	result->data_imag[i+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3] + res_im2[4] + res_im2[5] + res_im2[6] + res_im2[7];
	result->data_imag[i+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3] + res_im3[4] + res_im3[5] + res_im3[6] + res_im3[7];

	result->data_imag[i+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3] + res_im4[4] + res_im4[5] + res_im4[6] + res_im4[7];
	result->data_imag[i+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3] + res_im5[4] + res_im5[5] + res_im5[6] + res_im5[7];
	result->data_imag[i+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3] + res_im6[4] + res_im6[5] + res_im6[6] + res_im6[7];
	result->data_imag[i+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3] + res_im7[4] + res_im7[5] + res_im7[6] + res_im7[7];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test7(struct _result *result, const struct _input *input) {
    for (int i=0; i <= input->dlen - input->clen-16+1; i+=16) {
	AVXTYPE res_re0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re3 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_re4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re7 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_re8  = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re9  = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re10 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re11 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_re12 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re13 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re14 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re15 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_im0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im3 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_im4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im7 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_im8  = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im9  = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im10 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im11 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_im12 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im13 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im14 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_im15 = {0., 0., 0., 0., 0., 0., 0., 0.};

	for (int j=0; j<=input->clen-8; j+=8) {
	    AVXTYPE data_real;
	    AVXTYPE data_imag;
	    AVXTYPE coeff_real = LOADAVX_UNALIGNED(&input->coeff_real[j]); // 1 x load coeff
	    AVXTYPE coeff_imag = LOADAVX_UNALIGNED(&input->coeff_imag[j]); // 1 x load coeff

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j]);
	    res_re0 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im0 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+1]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+1]);
	    res_re1 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im1 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+2]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+2]);
	    res_re2 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im2 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+3]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+3]);
	    res_re3 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im3 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+4]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+4]);
	    res_re4 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im4 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+5]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+5]);
	    res_re5 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im5 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+6]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+6]);
	    res_re6 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im6 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+7]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+7]);
	    res_re7 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im7 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+ 8]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+ 8]);
	    res_re8 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im8 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+ 9]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+ 9]);
	    res_re9 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im9 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+10]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+10]);
	    res_re10 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im10 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+11]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+11]);
	    res_re11 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im11 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+12]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+12]);
	    res_re12 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im12 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+13]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+13]);
	    res_re13 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im13 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+14]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+14]);
	    res_re14 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im14 += data_real * coeff_imag + data_imag * coeff_real;

	    data_real = LOADAVX_UNALIGNED(&input->data_real[i+j+15]);
	    data_imag = LOADAVX_UNALIGNED(&input->data_imag[i+j+15]);
	    res_re15 += data_real * coeff_real - data_imag * coeff_imag;
	    res_im15 += data_real * coeff_imag + data_imag * coeff_real;
	}

	result->data_real[i]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3] + res_re0[4] + res_re0[5] + res_re0[6] + res_re0[7];
	result->data_real[i+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3] + res_re1[4] + res_re1[5] + res_re1[6] + res_re1[7];
	result->data_real[i+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3] + res_re2[4] + res_re2[5] + res_re2[6] + res_re2[7];
	result->data_real[i+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3] + res_re3[4] + res_re3[5] + res_re3[6] + res_re3[7];

	result->data_real[i+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3] + res_re4[4] + res_re4[5] + res_re4[6] + res_re4[7];
	result->data_real[i+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3] + res_re5[4] + res_re5[5] + res_re5[6] + res_re5[7];
	result->data_real[i+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3] + res_re6[4] + res_re6[5] + res_re6[6] + res_re6[7];
	result->data_real[i+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3] + res_re7[4] + res_re7[5] + res_re7[6] + res_re7[7];

	result->data_real[i+8]  = res_re8[0]  + res_re8[1] + res_re8[2] + res_re8[3] + res_re8[4] + res_re8[5] + res_re8[6] + res_re8[7];
	result->data_real[i+9]  = res_re9[0]  + res_re9[1] + res_re9[2] + res_re9[3] + res_re9[4] + res_re9[5] + res_re9[6] + res_re9[7];
	result->data_real[i+10] = res_re10[0] + res_re10[1] + res_re10[2] + res_re10[3] + res_re10[4] + res_re10[5] + res_re10[6] + res_re10[7];
	result->data_real[i+11] = res_re11[0] + res_re11[1] + res_re11[2] + res_re11[3] + res_re11[4] + res_re11[5] + res_re11[6] + res_re11[7];

	result->data_real[i+12] = res_re12[0] + res_re12[1] + res_re12[2] + res_re12[3] + res_re12[4] + res_re12[5] + res_re12[6] + res_re12[7];
	result->data_real[i+13] = res_re13[0] + res_re13[1] + res_re13[2] + res_re13[3] + res_re13[4] + res_re13[5] + res_re13[6] + res_re13[7];
	result->data_real[i+14] = res_re14[0] + res_re14[1] + res_re14[2] + res_re14[3] + res_re14[4] + res_re14[5] + res_re14[6] + res_re14[7];
	result->data_real[i+15] = res_re15[0] + res_re15[1] + res_re15[2] + res_re15[3] + res_re15[4] + res_re15[5] + res_re15[6] + res_re15[7];

	result->data_imag[i]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3] + res_im0[4] + res_im0[5] + res_im0[6] + res_im0[7];
	result->data_imag[i+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3] + res_im1[4] + res_im1[5] + res_im1[6] + res_im1[7];
	result->data_imag[i+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3] + res_im2[4] + res_im2[5] + res_im2[6] + res_im2[7];
	result->data_imag[i+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3] + res_im3[4] + res_im3[5] + res_im3[6] + res_im3[7];

	result->data_imag[i+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3] + res_im4[4] + res_im4[5] + res_im4[6] + res_im4[7];
	result->data_imag[i+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3] + res_im5[4] + res_im5[5] + res_im5[6] + res_im5[7];
	result->data_imag[i+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3] + res_im6[4] + res_im6[5] + res_im6[6] + res_im6[7];
	result->data_imag[i+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3] + res_im7[4] + res_im7[5] + res_im7[6] + res_im7[7];

	result->data_imag[i+8]  = res_im8[0]  + res_im8[1] + res_im8[2] + res_im8[3] + res_im8[4] + res_im8[5] + res_im8[6] + res_im8[7];
	result->data_imag[i+9]  = res_im9[0]  + res_im9[1] + res_im9[2] + res_im9[3] + res_im9[4] + res_im9[5] + res_im9[6] + res_im9[7];
	result->data_imag[i+10] = res_im10[0] + res_im10[1] + res_im10[2] + res_im10[3] + res_im10[4] + res_im10[5] + res_im10[6] + res_im10[7];
	result->data_imag[i+11] = res_im11[0] + res_im11[1] + res_im11[2] + res_im11[3] + res_im11[4] + res_im11[5] + res_im11[6] + res_im11[7];

	result->data_imag[i+12] = res_im12[0] + res_im12[1] + res_im12[2] + res_im12[3] + res_im12[4] + res_im12[5] + res_im12[6] + res_im12[7];
	result->data_imag[i+13] = res_im13[0] + res_im13[1] + res_im13[2] + res_im13[3] + res_im13[4] + res_im13[5] + res_im13[6] + res_im13[7];
	result->data_imag[i+14] = res_im14[0] + res_im14[1] + res_im14[2] + res_im14[3] + res_im14[4] + res_im14[5] + res_im14[6] + res_im14[7];
	result->data_imag[i+15] = res_im15[0] + res_im15[1] + res_im15[2] + res_im15[3] + res_im15[4] + res_im15[5] + res_im15[6] + res_im15[7];
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}
#endif
