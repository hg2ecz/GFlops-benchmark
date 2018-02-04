#include "gflops-test.h"

float gtest_float(float *fvec, int len) {
    float res = 0;
    for (int i=0; i<len; i++) {
	res+=fvec[i];
    }
    return res;
}

float gtest_float4(float *fvec, int len) {
    float res[4] = {0, 0, 0, 0};
    for (int i=0; i<len; i+=4) {
	res[0]+=fvec[i];
	res[1]+=fvec[i+1];
	res[2]+=fvec[i+2];
	res[3]+=fvec[i+3];
    }
    return res[0]+res[1]+res[2]+res[3];
}

float gtest_float16(float *fvec, int len) {
    float res0[4] = {0, 0, 0, 0};
    float res1[4] = {0, 0, 0, 0};
    float res2[4] = {0, 0, 0, 0};
    float res3[4] = {0, 0, 0, 0};

    for (int i=0; i<len; i+=16) {
	res0[0]+=fvec[i+0];
	res0[1]+=fvec[i+1];
	res0[2]+=fvec[i+2];
	res0[3]+=fvec[i+3];

	res1[0]+=fvec[i+4];
	res1[1]+=fvec[i+5];
	res1[2]+=fvec[i+6];
	res1[3]+=fvec[i+7];

	res2[0]+=fvec[i+8];
	res2[1]+=fvec[i+9];
	res2[2]+=fvec[i+10];
	res2[3]+=fvec[i+11];

	res3[0]+=fvec[i+12];
	res3[1]+=fvec[i+13];
	res3[2]+=fvec[i+14];
	res3[3]+=fvec[i+15];
    }
    return res0[0]+res0[1]+res0[2]+res0[3] + res1[0]+res1[1]+res1[2]+res1[3] +
	   res2[0]+res2[1]+res2[2]+res2[3] + res3[0]+res3[1]+res3[2]+res3[3];
}

float gtest_float32x4(float32x4_t *fvec, int len) {
    float32x4_t res = {0, 0, 0, 0};
    for (int i=0; i<len; i++) {
	res+=fvec[i];
    }
    return res[0]+res[1]+res[2]+res[3];
}
