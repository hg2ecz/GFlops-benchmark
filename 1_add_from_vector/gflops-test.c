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

float gtest_float32x4(float32x4_t *fvec, int len) {
    float32x4_t res = {0, 0, 0, 0};
    for (int i=0; i<len; i++) {
	res+=fvec[i];
    }
    return res[0]+res[1]+res[2]+res[3];
}
