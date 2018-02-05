#include "gflops-test.h"

float gtest_fmla_a(const float *fvec, int len) {
    float res = 0;
    for (int i=0; i<=len-16; i+=16) {
	res+=fvec[i+0]*fvec[i+4]; // r0*r1
	res+=fvec[i+1]*fvec[i+5];
	res+=fvec[i+2]*fvec[i+6];
	res+=fvec[i+3]*fvec[i+7];

	res+=fvec[i+4]*fvec[i+8]; // r1*r2
	res+=fvec[i+5]*fvec[i+9];
	res+=fvec[i+6]*fvec[i+10];
	res+=fvec[i+7]*fvec[i+11];

	res+=fvec[i+8]*fvec[i+12]; // r2*r3
	res+=fvec[i+9]*fvec[i+13];
	res+=fvec[i+10]*fvec[i+14];
	res+=fvec[i+11]*fvec[i+15];

	res+=fvec[i+12]*fvec[i+0]; // r3*r0
	res+=fvec[i+13]*fvec[i+1];
	res+=fvec[i+14]*fvec[i+2];
	res+=fvec[i+15]*fvec[i+3];
    }
    return res;
}

float gtest_fmla_b(const float *fvec, int len) {
    float res[4] = {0, 0, 0, 0};
    for (int i=0; i<=len-16; i+=16) {
	res[0]+=fvec[i+0]*fvec[i+4]; // r0*r1
	res[1]+=fvec[i+1]*fvec[i+5];
	res[2]+=fvec[i+2]*fvec[i+6];
	res[3]+=fvec[i+3]*fvec[i+7];

	res[0]+=fvec[i+4]*fvec[i+8]; // r1*r2
	res[1]+=fvec[i+5]*fvec[i+9];
	res[2]+=fvec[i+6]*fvec[i+10];
	res[3]+=fvec[i+7]*fvec[i+11];

	res[0]+=fvec[i+8]*fvec[i+12]; // r2*r3
	res[1]+=fvec[i+9]*fvec[i+13];
	res[2]+=fvec[i+10]*fvec[i+14];
	res[3]+=fvec[i+11]*fvec[i+15];

	res[0]+=fvec[i+12]*fvec[i+0]; // r3*r0
	res[1]+=fvec[i+13]*fvec[i+1];
	res[2]+=fvec[i+14]*fvec[i+2];
	res[3]+=fvec[i+15]*fvec[i+3];
    }
    return res[0]+res[1]+res[2]+res[3];
}

float gtest_fmla_c(const float *fvec, int len) {
    float res0[4] = {0, 0, 0, 0};
    float res1[4] = {0, 0, 0, 0};
    float res2[4] = {0, 0, 0, 0};
    float res3[4] = {0, 0, 0, 0};

    for (int i=0; i<=len-16; i+=16) {
	res0[0]+=fvec[i+0]*fvec[i+4]; // r0*r1
	res0[1]+=fvec[i+1]*fvec[i+5];
	res0[2]+=fvec[i+2]*fvec[i+6];
	res0[3]+=fvec[i+3]*fvec[i+7];

	res1[0]+=fvec[i+4]*fvec[i+8]; // r1*r2
	res1[1]+=fvec[i+5]*fvec[i+9];
	res1[2]+=fvec[i+6]*fvec[i+10];
	res1[3]+=fvec[i+7]*fvec[i+11];

	res2[0]+=fvec[i+8]*fvec[i+12]; // r2*r3
	res2[1]+=fvec[i+9]*fvec[i+13];
	res2[2]+=fvec[i+10]*fvec[i+14];
	res2[3]+=fvec[i+11]*fvec[i+15];

	res3[0]+=fvec[i+12]*fvec[i+0]; // r3*r0
	res3[1]+=fvec[i+13]*fvec[i+1];
	res3[2]+=fvec[i+14]*fvec[i+2];
	res3[3]+=fvec[i+15]*fvec[i+3];
    }
    return res0[0]+res0[1]+res0[2]+res0[3] + res1[0]+res1[1]+res1[2]+res1[3] +
	   res2[0]+res2[1]+res2[2]+res2[3] + res3[0]+res3[1]+res3[2]+res3[3];
}

float gtest_fmla_c2(const float *fvec, int len) {
    float32x4_t res = {0, 0, 0, 0};

    for (int i=0; i<=len-16; i+=16) {
	float32x4_t r0 = *((const float32x4_t *)&fvec[i+0]);
	float32x4_t r1 = *((const float32x4_t *)&fvec[i+4]);
	float32x4_t r2 = *((const float32x4_t *)&fvec[i+8]);
	float32x4_t r3 = *((const float32x4_t *)&fvec[i+12]);

	res+=r0*r1;
	res+=r1*r2;
	res+=r2*r3;
	res+=r3*r0;
    }
    return res[0]+res[1]+res[2]+res[3];
}
