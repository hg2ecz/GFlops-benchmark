#include <math.h>
#include <malloc.h>

#include "test.h"

#define DATALEN (20*1000*1024)
#define COEFLEN 512
#define DECIM   20

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
    input->coeff = malloc(COEFLEN*sizeof(float));
    input->clen = COEFLEN;
    for (int i=0; i<input->dlen; i++) {
	input->data_real[i] = 1.0*sin(2*M_PI/1001*i);
	input->data_imag[i] = 1.1*sin(2.131*M_PI/1001*i);
    }
    for (int i=0; i<input->clen; i++) {
	input->coeff[i] = 0.9+0.1/(i+1.);
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
    int decim=DECIM;
    int k=0;
    for (int i=0; i <= input->dlen - input->clen-decim+1; i+=decim) {
	float res_re = 0;
	float res_im = 0;
	for (int j=0; j<input->clen; j++) {
	    res_re += input->data_real[i+j] * input->coeff[j];
	    res_im += input->data_imag[i+j] * input->coeff[j];
	}
	result->data_real[k] = res_re;
	result->data_imag[k] = res_im;
	k++;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test2(struct _result *result, const struct _input *input) {
    int decim=DECIM;
    int k=0;
    for (int i=0; i <= input->dlen - input->clen-decim-4+1; i+=decim) {
	VECTYPE res_re = {0., 0., 0., 0.};
	VECTYPE res_im = {0., 0., 0., 0.};
	for (int j=0; j <= input->clen-4; j+=4) {
	    VECTYPE data_re = LOADSIMD_UNALIGNED(&input->data_real[i+j]);
	    VECTYPE data_im = LOADSIMD_UNALIGNED(&input->data_imag[i+j]);
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]);
	    res_re += data_re * coeff;
	    res_im += data_im * coeff;
	}
	result->data_real[k] = res_re[0] + res_re[1] + res_re[2] + res_re[3];
	result->data_imag[k] = res_im[0] + res_im[1] + res_im[2] + res_im[3];
	k++;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test3(struct _result *result, const struct _input *input) {
    int decim=DECIM;
    int k=0;
    for (int i=0; i <= input->dlen - input->clen-3*decim+1; i+=3*decim) {
	VECTYPE res_re0 = {0., 0., 0., 0.}, res_im0 = {0., 0., 0., 0.};
	VECTYPE res_re1 = {0., 0., 0., 0.}, res_im1 = {0., 0., 0., 0.};
	VECTYPE res_re2 = {0., 0., 0., 0.}, res_im2 = {0., 0., 0., 0.};
	//VECTYPE res_re3 = {0., 0., 0., 0.}, res_im3 = {0., 0., 0., 0.}; -- optimize for NEON

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data;
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]); // 1 x load coeff

	    int r = i+j;
	    data = LOADSIMD(&input->data_real[r]); res_re0 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im0 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re1 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im1 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re2 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im2 += data * coeff;
	    //r += decim;
	    //data = LOADSIMD(&input->data_real[r]); res_re3 += data * coeff;
	    //data = LOADSIMD(&input->data_imag[r]); res_im3 += data * coeff;
	}
	result->data_real[k]   = res_re0[0] + res_re0[1] + res_re1[2] + res_re0[3];
	result->data_real[k+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3];
	result->data_real[k+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3];
	//result->data_real[k+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3];

	result->data_imag[k]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3];
	result->data_imag[k+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3];
	result->data_imag[k+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3];
	//result->data_imag[k+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3];
	k+=3;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test4(struct _result *result, const struct _input *input) {
    int decim=DECIM;
    int k=0;
    for (int i=0; i <= input->dlen - input->clen-7*decim+1; i+=7*decim) {
	VECTYPE res_re0 = {0., 0., 0., 0.}, res_im0 = {0., 0., 0., 0.};
	VECTYPE res_re1 = {0., 0., 0., 0.}, res_im1 = {0., 0., 0., 0.};
	VECTYPE res_re2 = {0., 0., 0., 0.}, res_im2 = {0., 0., 0., 0.};
	VECTYPE res_re3 = {0., 0., 0., 0.}, res_im3 = {0., 0., 0., 0.};
	VECTYPE res_re4 = {0., 0., 0., 0.}, res_im4 = {0., 0., 0., 0.};
	VECTYPE res_re5 = {0., 0., 0., 0.}, res_im5 = {0., 0., 0., 0.};
	VECTYPE res_re6 = {0., 0., 0., 0.}, res_im6 = {0., 0., 0., 0.};
	//VECTYPE res_re7 = {0., 0., 0., 0.}, res_im7 = {0., 0., 0., 0.}; -- optimalize for NEON regs

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data;
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]); // 1 x load coeff

	    int r = i+j;
	    data = LOADSIMD(&input->data_real[r]); res_re0 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im0 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re1 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im1 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re2 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im2 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re3 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im3 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re4 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im4 += data * coeff;
	    r += decim;
	    data = LOADSIMD_UNALIGNED(&input->data_real[r]); res_re5 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data_imag[r]); res_im5 += data * coeff;
	    r += decim;
	    data = LOADSIMD_UNALIGNED(&input->data_real[r]); res_re6 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data_imag[r]); res_im6 += data * coeff;
	    //r += decim;
	    //data = LOADSIMD_UNALIGNED(&input->data_real[r]); res_re7 += data * coeff;
	    //data = LOADSIMD_UNALIGNED(&input->data_imag[r]); res_im7 += data * coeff;
	}

        result->data_real[k]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3];
	result->data_real[k+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3];
	result->data_real[k+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3];
	result->data_real[k+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3];
	result->data_real[k+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3];
	result->data_real[k+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3];
	result->data_real[k+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3];
	//result->data_real[k+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3];

	result->data_imag[k]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3];
	result->data_imag[k+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3];
	result->data_imag[k+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3];
	result->data_imag[k+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3];
	result->data_imag[k+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3];
	result->data_imag[k+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3];
	result->data_imag[k+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3];
	//result->data_imag[k+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3];
	k+=7;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test5(struct _result *result, const struct _input *input) {
    int decim=DECIM;
    int k = 0;
    for (int i=0; i <= input->dlen - input->clen-15*decim+1; i+=15*decim) {
	VECTYPE res_re0 = {0., 0., 0., 0.}, res_im0 = {0., 0., 0., 0.};
	VECTYPE res_re1 = {0., 0., 0., 0.}, res_im1 = {0., 0., 0., 0.};
	VECTYPE res_re2 = {0., 0., 0., 0.}, res_im2 = {0., 0., 0., 0.};
	VECTYPE res_re3 = {0., 0., 0., 0.}, res_im3 = {0., 0., 0., 0.};
	VECTYPE res_re4 = {0., 0., 0., 0.}, res_im4 = {0., 0., 0., 0.};
	VECTYPE res_re5 = {0., 0., 0., 0.}, res_im5 = {0., 0., 0., 0.};
	VECTYPE res_re6 = {0., 0., 0., 0.}, res_im6 = {0., 0., 0., 0.};
	VECTYPE res_re7 = {0., 0., 0., 0.}, res_im7 = {0., 0., 0., 0.};

	VECTYPE res_re8 = {0., 0., 0., 0.}, res_im8 = {0., 0., 0., 0.};
	VECTYPE res_re9 = {0., 0., 0., 0.}, res_im9 = {0., 0., 0., 0.};
	VECTYPE res_re10 = {0., 0., 0., 0.}, res_im10 = {0., 0., 0., 0.};
	VECTYPE res_re11 = {0., 0., 0., 0.}, res_im11 = {0., 0., 0., 0.};
	VECTYPE res_re12 = {0., 0., 0., 0.}, res_im12 = {0., 0., 0., 0.};
	VECTYPE res_re13 = {0., 0., 0., 0.}, res_im13 = {0., 0., 0., 0.};
	VECTYPE res_re14 = {0., 0., 0., 0.}, res_im14 = {0., 0., 0., 0.};
	//VECTYPE res_re15 = {0., 0., 0., 0.}, res_im15 = {0., 0., 0., 0.}; -- optimalize for NEON regs

	for (int j=0; j<=input->clen-4; j+=4) {
	    VECTYPE data;
	    VECTYPE coeff = LOADSIMD(&input->coeff[j]); // 1 x load coeff

	    int r = i+j;
	    data = LOADSIMD(&input->data_real[r]); res_re0 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im0 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re1 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im1 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re2 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im2 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re3 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im3 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re4 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im4 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re5 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im5 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re6 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im6 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re7 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im7 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re8  += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im8  += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re9  += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im9  += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re10 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im10 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re11 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im11 += data * coeff;
	    r += decim;
	    data = LOADSIMD(&input->data_real[r]); res_re12 += data * coeff;
	    data = LOADSIMD(&input->data_imag[r]); res_im12 += data * coeff;
	    r += decim;
	    data = LOADSIMD_UNALIGNED(&input->data_real[r]); res_re13 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data_imag[r]); res_im13 += data * coeff;
	    r += decim;
	    data = LOADSIMD_UNALIGNED(&input->data_real[r]); res_re14 += data * coeff;
	    data = LOADSIMD_UNALIGNED(&input->data_imag[r]); res_im14 += data * coeff;
	    //r += decim;
	    //data = LOADSIMD_UNALIGNED(&input->data_real[r]); res_re15 += data * coeff;
	    //data = LOADSIMD_UNALIGNED(&input->data_imag[r]); res_im15 += data * coeff;
	}

	result->data_real[k]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3];
	result->data_real[k+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3];
	result->data_real[k+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3];
	result->data_real[k+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3];

	result->data_real[k+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3];
	result->data_real[k+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3];
	result->data_real[k+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3];
	result->data_real[k+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3];

	result->data_real[k+8]  = res_re8[0]  + res_re8[1] + res_re8[2] + res_re8[3];
	result->data_real[k+9]  = res_re9[0]  + res_re9[1] + res_re9[2] + res_re9[3];
	result->data_real[k+10] = res_re10[0] + res_re10[1] + res_re10[2] + res_re10[3];
	result->data_real[k+11] = res_re11[0] + res_re11[1] + res_re11[2] + res_re11[3];

	result->data_real[k+12] = res_re12[0] + res_re12[1] + res_re12[2] + res_re12[3];
	result->data_real[k+13] = res_re13[0] + res_re13[1] + res_re13[2] + res_re13[3];
	result->data_real[k+14] = res_re14[0] + res_re14[1] + res_re14[2] + res_re14[3];
	//result->data_real[k+15] = res_re15[0] + res_re15[1] + res_re15[2] + res_re15[3];

	result->data_imag[k]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3];
	result->data_imag[k+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3];
	result->data_imag[k+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3];
	result->data_imag[k+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3];

	result->data_imag[k+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3];
	result->data_imag[k+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3];
	result->data_imag[k+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3];
	result->data_imag[k+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3];

	result->data_imag[k+8]  = res_im8[0]  + res_im8[1] + res_im8[2] + res_im8[3];
	result->data_imag[k+9]  = res_im9[0]  + res_im9[1] + res_im9[2] + res_im9[3];
	result->data_imag[k+10] = res_im10[0] + res_im10[1] + res_im10[2] + res_im10[3];
	result->data_imag[k+11] = res_im11[0] + res_im11[1] + res_im11[2] + res_im11[3];

	result->data_imag[k+12] = res_im12[0] + res_im12[1] + res_im12[2] + res_im12[3];
	result->data_imag[k+13] = res_im13[0] + res_im13[1] + res_im13[2] + res_im13[3];
	result->data_imag[k+14] = res_im14[0] + res_im14[1] + res_im14[2] + res_im14[3];
	//result->data_imag[k+15] = res_im15[0] + res_im15[1] + res_im15[2] + res_im15[3];
	k+=15;
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
    int decim=DECIM;
    int k=0;
    for (int i=0; i <= input->dlen - input->clen-7*decim+1; i+=7*decim) {
	AVXTYPE res_re0 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re1 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re2 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re3 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im3 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re4 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re5 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re6 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	//AVXTYPE res_re7 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im7 = {0., 0., 0., 0., 0., 0., 0., 0.}; -- optimize for AVXregs

	for (int j=0; j<=input->clen-8; j+=8) {
	    AVXTYPE data;
	    AVXTYPE coeff = LOADAVX(&input->coeff[j]); // 1 x load coeff

	    int r = i+j;
	    data = LOADAVX(&input->data_real[r]); res_re0 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im0 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re1 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im1 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re2 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im2 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re3 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im3 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re4 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im4 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re5 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im5 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re6 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im6 += data * coeff;
	    //r += decim;
	    //data = LOADAVX(&input->data_real[r]); res_re7 += data * coeff;
	    //data = LOADAVX(&input->data_imag[r]); res_im7 += data * coeff;
	}

	result->data_real[k]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3] + res_re0[4] + res_re0[5] + res_re0[6] + res_re0[7];
	result->data_real[k+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3] + res_re1[4] + res_re1[5] + res_re1[6] + res_re1[7];
	result->data_real[k+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3] + res_re2[4] + res_re2[5] + res_re2[6] + res_re2[7];
	result->data_real[k+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3] + res_re3[4] + res_re3[5] + res_re3[6] + res_re3[7];

	result->data_real[k+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3] + res_re4[4] + res_re4[5] + res_re4[6] + res_re4[7];
	result->data_real[k+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3] + res_re5[4] + res_re5[5] + res_re5[6] + res_re5[7];
	result->data_real[k+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3] + res_re6[4] + res_re6[5] + res_re6[6] + res_re6[7];
	//result->data_real[i+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3] + res_re7[4] + res_re7[5] + res_re7[6] + res_re7[7];


	result->data_imag[k]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3] + res_im0[4] + res_im0[5] + res_im0[6] + res_im0[7];
	result->data_imag[k+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3] + res_im1[4] + res_im1[5] + res_im1[6] + res_im1[7];
	result->data_imag[k+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3] + res_im2[4] + res_im2[5] + res_im2[6] + res_im2[7];
	result->data_imag[k+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3] + res_im3[4] + res_im3[5] + res_im3[6] + res_im3[7];

	result->data_imag[k+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3] + res_im4[4] + res_im4[5] + res_im4[6] + res_im4[7];
	result->data_imag[k+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3] + res_im5[4] + res_im5[5] + res_im5[6] + res_im5[7];
	result->data_imag[k+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3] + res_im6[4] + res_im6[5] + res_im6[6] + res_im6[7];
	//result->data_imag[i+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3] + res_im7[4] + res_im7[5] + res_im7[6] + res_im7[7];
	k+=7;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}

int test7(struct _result *result, const struct _input *input) {
    int decim=DECIM;
    int k=0;
    for (int i=0; i <= input->dlen - input->clen-15*decim+1; i+=15*decim) {
	AVXTYPE res_re0 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im0 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re1 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im1 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re2 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im2 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re3 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im3 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re4 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im4 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re5 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im5 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re6 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im6 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re7 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im7 = {0., 0., 0., 0., 0., 0., 0., 0.};

	AVXTYPE res_re8 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im8 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re9 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im9 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re10 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im10 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re11 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im11 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re12 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im12 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re13 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im13 = {0., 0., 0., 0., 0., 0., 0., 0.};
	AVXTYPE res_re14 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im14 = {0., 0., 0., 0., 0., 0., 0., 0.};
	//AVXTYPE res_re15 = {0., 0., 0., 0., 0., 0., 0., 0.}, res_im15 = {0., 0., 0., 0., 0., 0., 0., 0.}; -- optimize for AVXregs

	for (int j=0; j<=input->clen-8; j+=8) {
	    AVXTYPE data;
	    AVXTYPE coeff = LOADAVX(&input->coeff[j]); // 1 x load coeff

	    int r=i+j;
	    data = LOADAVX(&input->data_real[r]); res_re0 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im0 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re1 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im1 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re2 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im2 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re3 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im3 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re4 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im4 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re5 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im5 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re6 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im6 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re7 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im7 += data * coeff;

	    r += decim;
	    data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re8  += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im8  += data * coeff;
	    r += decim;
	    data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re9  += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im9  += data * coeff;
	    r += decim;
	    data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re10 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im10 += data * coeff;
	    r += decim;
	    data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re11 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im11 += data * coeff;
	    r += decim;
	    data = LOADAVX(&input->data_real[r]); res_re12 += data * coeff;
	    data = LOADAVX(&input->data_imag[r]); res_im12 += data * coeff;
	    r += decim;
	    data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re13 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im13 += data * coeff;
	    r += decim;
	    data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re14 += data * coeff;
	    data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im14 += data * coeff;
	    //r += decim;
	    //data = LOADAVX_UNALIGNED(&input->data_real[r]); res_re15 += data * coeff;
	    //data = LOADAVX_UNALIGNED(&input->data_imag[r]); res_im15 += data * coeff;
	}

	result->data_real[k]   = res_re0[0] + res_re0[1] + res_re0[2] + res_re0[3] + res_re0[4] + res_re0[5] + res_re0[6] + res_re0[7];
	result->data_real[k+1] = res_re1[0] + res_re1[1] + res_re1[2] + res_re1[3] + res_re1[4] + res_re1[5] + res_re1[6] + res_re1[7];
	result->data_real[k+2] = res_re2[0] + res_re2[1] + res_re2[2] + res_re2[3] + res_re2[4] + res_re2[5] + res_re2[6] + res_re2[7];
	result->data_real[k+3] = res_re3[0] + res_re3[1] + res_re3[2] + res_re3[3] + res_re3[4] + res_re3[5] + res_re3[6] + res_re3[7];

	result->data_real[k+4] = res_re4[0] + res_re4[1] + res_re4[2] + res_re4[3] + res_re4[4] + res_re4[5] + res_re4[6] + res_re4[7];
	result->data_real[k+5] = res_re5[0] + res_re5[1] + res_re5[2] + res_re5[3] + res_re5[4] + res_re5[5] + res_re5[6] + res_re5[7];
	result->data_real[k+6] = res_re6[0] + res_re6[1] + res_re6[2] + res_re6[3] + res_re6[4] + res_re6[5] + res_re6[6] + res_re6[7];
	result->data_real[k+7] = res_re7[0] + res_re7[1] + res_re7[2] + res_re7[3] + res_re7[4] + res_re7[5] + res_re7[6] + res_re7[7];

	result->data_real[k+8]  = res_re8[0]  + res_re8[1] + res_re8[2] + res_re8[3] + res_re8[4] + res_re8[5] + res_re8[6] + res_re8[7];
	result->data_real[k+9]  = res_re9[0]  + res_re9[1] + res_re9[2] + res_re9[3] + res_re9[4] + res_re9[5] + res_re9[6] + res_re9[7];
	result->data_real[k+10] = res_re10[0] + res_re10[1] + res_re10[2] + res_re10[3] + res_re10[4] + res_re10[5] + res_re10[6] + res_re10[7];
	result->data_real[k+11] = res_re11[0] + res_re11[1] + res_re11[2] + res_re11[3] + res_re11[4] + res_re11[5] + res_re11[6] + res_re11[7];

	result->data_real[k+12] = res_re12[0] + res_re12[1] + res_re12[2] + res_re12[3] + res_re12[4] + res_re12[5] + res_re12[6] + res_re12[7];
	result->data_real[k+13] = res_re13[0] + res_re13[1] + res_re13[2] + res_re13[3] + res_re13[4] + res_re13[5] + res_re13[6] + res_re13[7];
	result->data_real[k+14] = res_re14[0] + res_re14[1] + res_re14[2] + res_re14[3] + res_re14[4] + res_re14[5] + res_re14[6] + res_re14[7];
	//result->data_real[k+15] = res_re15[0] + res_re15[1] + res_re15[2] + res_re15[3] + res_re15[4] + res_re15[5] + res_re15[6] + res_re15[7];

	result->data_imag[k]   = res_im0[0] + res_im0[1] + res_im0[2] + res_im0[3] + res_im0[4] + res_im0[5] + res_im0[6] + res_im0[7];
	result->data_imag[k+1] = res_im1[0] + res_im1[1] + res_im1[2] + res_im1[3] + res_im1[4] + res_im1[5] + res_im1[6] + res_im1[7];
	result->data_imag[k+2] = res_im2[0] + res_im2[1] + res_im2[2] + res_im2[3] + res_im2[4] + res_im2[5] + res_im2[6] + res_im2[7];
	result->data_imag[k+3] = res_im3[0] + res_im3[1] + res_im3[2] + res_im3[3] + res_im3[4] + res_im3[5] + res_im3[6] + res_im3[7];

	result->data_imag[k+4] = res_im4[0] + res_im4[1] + res_im4[2] + res_im4[3] + res_im4[4] + res_im4[5] + res_im4[6] + res_im4[7];
	result->data_imag[k+5] = res_im5[0] + res_im5[1] + res_im5[2] + res_im5[3] + res_im5[4] + res_im5[5] + res_im5[6] + res_im5[7];
	result->data_imag[k+6] = res_im6[0] + res_im6[1] + res_im6[2] + res_im6[3] + res_im6[4] + res_im6[5] + res_im6[6] + res_im6[7];
	result->data_imag[k+7] = res_im7[0] + res_im7[1] + res_im7[2] + res_im7[3] + res_im7[4] + res_im7[5] + res_im7[6] + res_im7[7];

	result->data_imag[k+8]  = res_im8[0]  + res_im8[1] + res_im8[2] + res_im8[3] + res_im8[4] + res_im8[5] + res_im8[6] + res_im8[7];
	result->data_imag[k+9]  = res_im9[0]  + res_im9[1] + res_im9[2] + res_im9[3] + res_im9[4] + res_im9[5] + res_im9[6] + res_im9[7];
	result->data_imag[k+10] = res_im10[0] + res_im10[1] + res_im10[2] + res_im10[3] + res_im10[4] + res_im10[5] + res_im10[6] + res_im10[7];
	result->data_imag[k+11] = res_im11[0] + res_im11[1] + res_im11[2] + res_im11[3] + res_im11[4] + res_im11[5] + res_im11[6] + res_im11[7];

	result->data_imag[k+12] = res_im12[0] + res_im12[1] + res_im12[2] + res_im12[3] + res_im12[4] + res_im12[5] + res_im12[6] + res_im12[7];
	result->data_imag[k+13] = res_im13[0] + res_im13[1] + res_im13[2] + res_im13[3] + res_im13[4] + res_im13[5] + res_im13[6] + res_im13[7];
	result->data_imag[k+14] = res_im14[0] + res_im14[1] + res_im14[2] + res_im14[3] + res_im14[4] + res_im14[5] + res_im14[6] + res_im14[7];
	//result->data_imag[k+15] = res_im15[0] + res_im15[1] + res_im15[2] + res_im15[3] + res_im15[4] + res_im15[5] + res_im15[6] + res_im15[7];
	k+=15;
    }
    result->dlen = input->dlen - input->clen;
    return 0;
}
#endif
