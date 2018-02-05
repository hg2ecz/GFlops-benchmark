#include "gflops-test.h"

float gtest_float(const float *fvec, int len) {
    float res = 0;
    for (int i=0; i<len; i++) {
	res+=fvec[i];
    }
    return res;
}

float gtest_float4(const float *fvec, int len) {
    float res[4] = {0, 0, 0, 0};
    for (int i=0; i<=len-4; i+=4) {
	res[0]+=fvec[i];
	res[1]+=fvec[i+1];
	res[2]+=fvec[i+2];
	res[3]+=fvec[i+3];
    }
    return res[0]+res[1]+res[2]+res[3];
}

float gtest_float16(const float *fvec, int len) {
    float res0[4] = {0, 0, 0, 0};
    float res1[4] = {0, 0, 0, 0};
    float res2[4] = {0, 0, 0, 0};
    float res3[4] = {0, 0, 0, 0};

    for (int i=0; i<=len-16; i+=16) {
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

float gtest_float32(const float *fvec, int len) {
    float res0[4] = {0, 0, 0, 0};
    float res1[4] = {0, 0, 0, 0};
    float res2[4] = {0, 0, 0, 0};
    float res3[4] = {0, 0, 0, 0};
    float res4[4] = {0, 0, 0, 0};
    float res5[4] = {0, 0, 0, 0};
    float res6[4] = {0, 0, 0, 0};
    float res7[4] = {0, 0, 0, 0};

    for (int i=0; i<=len-32; i+=32) {
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

	res4[0]+=fvec[i+16];
	res4[1]+=fvec[i+17];
	res4[2]+=fvec[i+18];
	res4[3]+=fvec[i+19];

	res5[0]+=fvec[i+20];
	res5[1]+=fvec[i+21];
	res5[2]+=fvec[i+22];
	res5[3]+=fvec[i+23];

	res6[0]+=fvec[i+24];
	res6[1]+=fvec[i+25];
	res6[2]+=fvec[i+26];
	res6[3]+=fvec[i+27];

	res7[0]+=fvec[i+28];
	res7[1]+=fvec[i+29];
	res7[2]+=fvec[i+30];
	res7[3]+=fvec[i+31];
    }
    return res0[0]+res0[1]+res0[2]+res0[3] + res1[0]+res1[1]+res1[2]+res1[3] +
	   res2[0]+res2[1]+res2[2]+res2[3] + res3[0]+res3[1]+res3[2]+res3[3] +
	   res4[0]+res4[1]+res4[2]+res4[3] + res5[0]+res5[1]+res5[2]+res5[3] +
	   res6[0]+res6[1]+res6[2]+res6[3] + res7[0]+res7[1]+res7[2]+res7[3];
}


float gtest_float_vec(VECTYPE *fvec, int len) {
    VECTYPE res = {0, 0, 0, 0};
    for (int i=0; i<len; i++) {
	res+=fvec[i];
    }
    return res[0]+res[1]+res[2]+res[3];
}
