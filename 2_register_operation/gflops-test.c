#include "gflops-test.h"

float gtest_fmla_a(const float *fvec, int len) {
    float res = 0;
    for (int i=0; i<=len-8; i+=8) {
	res+=fvec[i+0]*fvec[i+1];
	res+=fvec[i+1]*fvec[i+2];
	res+=fvec[i+2]*fvec[i+3];
	res+=fvec[i+3]*fvec[i+4];
	res+=fvec[i+4]*fvec[i+5];
	res+=fvec[i+5]*fvec[i+6];
	res+=fvec[i+6]*fvec[i+7];
	res+=fvec[i+7]*fvec[i+0];

	res+=fvec[i+0]*fvec[i+2];
	res+=fvec[i+1]*fvec[i+3];
	res+=fvec[i+2]*fvec[i+4];
	res+=fvec[i+3]*fvec[i+5];
	res+=fvec[i+4]*fvec[i+6];
	res+=fvec[i+5]*fvec[i+7];
	res+=fvec[i+6]*fvec[i+0];
	res+=fvec[i+7]*fvec[i+1];

	res+=fvec[i+0]*fvec[i+3];
	res+=fvec[i+1]*fvec[i+4];
	res+=fvec[i+2]*fvec[i+5];
	res+=fvec[i+3]*fvec[i+6];
	res+=fvec[i+4]*fvec[i+7];
	res+=fvec[i+5]*fvec[i+0];
	res+=fvec[i+6]*fvec[i+1];
	res+=fvec[i+7]*fvec[i+2];

	res+=fvec[i+0]*fvec[i+4];
	res+=fvec[i+1]*fvec[i+5];
	res+=fvec[i+2]*fvec[i+6];
	res+=fvec[i+3]*fvec[i+7];
	res+=fvec[i+4]*fvec[i+0];
	res+=fvec[i+5]*fvec[i+1];
	res+=fvec[i+6]*fvec[i+2];
	res+=fvec[i+7]*fvec[i+3];
    }
    return res;
}

float gtest_fmla_b(const float *fvec, int len) {
    float res[4] = {0, 0, 0, 0};
    for (int i=0; i<=len-8; i+=8) {
	res[0]+=fvec[i+0]*fvec[i+1];
	res[0]+=fvec[i+1]*fvec[i+2];
	res[1]+=fvec[i+2]*fvec[i+3];
	res[1]+=fvec[i+3]*fvec[i+4];
	res[2]+=fvec[i+4]*fvec[i+5];
	res[2]+=fvec[i+5]*fvec[i+6];
	res[3]+=fvec[i+6]*fvec[i+7];
	res[3]+=fvec[i+7]*fvec[i+0];

	res[0]+=fvec[i+0]*fvec[i+2];
	res[0]+=fvec[i+1]*fvec[i+3];
	res[1]+=fvec[i+2]*fvec[i+4];
	res[1]+=fvec[i+3]*fvec[i+5];
	res[2]+=fvec[i+4]*fvec[i+6];
	res[2]+=fvec[i+5]*fvec[i+7];
	res[3]+=fvec[i+6]*fvec[i+0];
	res[3]+=fvec[i+7]*fvec[i+1];

	res[0]+=fvec[i+0]*fvec[i+3];
	res[0]+=fvec[i+1]*fvec[i+4];
	res[1]+=fvec[i+2]*fvec[i+5];
	res[1]+=fvec[i+3]*fvec[i+6];
	res[2]+=fvec[i+4]*fvec[i+7];
	res[2]+=fvec[i+5]*fvec[i+0];
	res[3]+=fvec[i+6]*fvec[i+1];
	res[3]+=fvec[i+7]*fvec[i+2];

	res[0]+=fvec[i+0]*fvec[i+4];
	res[0]+=fvec[i+1]*fvec[i+5];
	res[1]+=fvec[i+2]*fvec[i+6];
	res[1]+=fvec[i+3]*fvec[i+7];
	res[2]+=fvec[i+4]*fvec[i+0];
	res[2]+=fvec[i+5]*fvec[i+1];
	res[3]+=fvec[i+6]*fvec[i+2];
	res[3]+=fvec[i+7]*fvec[i+3];
    }
    return res[0]+res[1]+res[2]+res[3];
}

float gtest_fmla_c(const float *fvec, int len) {
    float res0[4] = {0, 0, 0, 0};
    float res1[4] = {0, 0, 0, 0};
    float res2[4] = {0, 0, 0, 0};
    float res3[4] = {0, 0, 0, 0};

    for (int i=0; i<=len-8; i+=8) {
	res0[0]+=fvec[i+0]*fvec[i+1];
	res0[0]+=fvec[i+1]*fvec[i+2];
	res0[1]+=fvec[i+2]*fvec[i+3];
	res0[1]+=fvec[i+3]*fvec[i+4];
	res0[2]+=fvec[i+4]*fvec[i+5];
	res0[2]+=fvec[i+5]*fvec[i+6];
	res0[3]+=fvec[i+6]*fvec[i+7];
	res0[3]+=fvec[i+7]*fvec[i+0];

	res1[0]+=fvec[i+0]*fvec[i+2];
	res1[0]+=fvec[i+1]*fvec[i+3];
	res1[1]+=fvec[i+2]*fvec[i+4];
	res1[1]+=fvec[i+3]*fvec[i+5];
	res1[2]+=fvec[i+4]*fvec[i+6];
	res1[2]+=fvec[i+5]*fvec[i+7];
	res1[3]+=fvec[i+6]*fvec[i+0];
	res1[3]+=fvec[i+7]*fvec[i+1];

	res2[0]+=fvec[i+0]*fvec[i+3];
	res2[0]+=fvec[i+1]*fvec[i+4];
	res2[1]+=fvec[i+2]*fvec[i+5];
	res2[1]+=fvec[i+3]*fvec[i+6];
	res2[2]+=fvec[i+4]*fvec[i+7];
	res2[2]+=fvec[i+5]*fvec[i+0];
	res2[3]+=fvec[i+6]*fvec[i+1];
	res2[3]+=fvec[i+7]*fvec[i+2];

	res3[0]+=fvec[i+0]*fvec[i+4];
	res3[0]+=fvec[i+1]*fvec[i+5];
	res3[1]+=fvec[i+2]*fvec[i+6];
	res3[1]+=fvec[i+3]*fvec[i+7];
	res3[2]+=fvec[i+4]*fvec[i+0];
	res3[2]+=fvec[i+5]*fvec[i+1];
	res3[3]+=fvec[i+6]*fvec[i+2];
	res3[3]+=fvec[i+7]*fvec[i+3];
    }
    return res0[0]+res0[1]+res0[2]+res0[3] + res1[0]+res1[1]+res1[2]+res1[3] +
	   res2[0]+res2[1]+res2[2]+res2[3] + res3[0]+res3[1]+res3[2]+res3[3];
}

float gtest_fmla_c2(const float *fvec, int len) {
    float res0[4] = {0, 0, 0, 0};
    float res1[4] = {0, 0, 0, 0};
    float res2[4] = {0, 0, 0, 0};
    float res3[4] = {0, 0, 0, 0};

    for (int i=0; i<=len-8; i+=8) {
	float r0 = fvec[i+0];
	float r1 = fvec[i+1];
	float r2 = fvec[i+2];
	float r3 = fvec[i+3];
	float r4 = fvec[i+4];
	float r5 = fvec[i+5];
	float r6 = fvec[i+6];
	float r7 = fvec[i+7];

	res0[0]+=r0*r1;
	res0[0]+=r1*r2;
	res0[1]+=r2*r3;
	res0[1]+=r3*r4;
	res0[2]+=r4*r5;
	res0[2]+=r5*r6;
	res0[3]+=r6*r7;
	res0[3]+=r7*r0;

	res0[0]+=r0*r2;
	res0[0]+=r1*r3;
	res0[1]+=r2*r4;
	res0[1]+=r3*r5;
	res0[2]+=r4*r6;
	res0[2]+=r5*r7;
	res0[3]+=r6*r0;
	res0[3]+=r7*r1;

	res0[0]+=r0*r3;
	res0[0]+=r1*r4;
	res0[1]+=r2*r5;
	res0[1]+=r3*r6;
	res0[2]+=r4*r7;
	res0[2]+=r5*r0;
	res0[3]+=r6*r1;
	res0[3]+=r7*r2;

	res0[0]+=r0*r4;
	res0[0]+=r1*r5;
	res0[1]+=r2*r6;
	res0[1]+=r3*r7;
	res0[2]+=r4*r0;
	res0[2]+=r5*r1;
	res0[3]+=r6*r2;
	res0[3]+=r7*r3;
    }
    return res0[0]+res0[1]+res0[2]+res0[3] + res1[0]+res1[1]+res1[2]+res1[3] +
	   res2[0]+res2[1]+res2[2]+res2[3] + res3[0]+res3[1]+res3[2]+res3[3];
}
