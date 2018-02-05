#include "gflops-test.h"

float gtest_fmla_a(const float *fvec, int len) {
    float res = 0;
    for (int i=0; i<=len-16; i+=16) {
	res+=fvec[i+0]*fvec[i+4]; // r0*r1
	res+=fvec[i+1]*fvec[i+5];
	res+=fvec[i+2]*fvec[i+6];
	res+=fvec[i+3]*fvec[i+7];

	res+=fvec[i+0]*fvec[i+8]; // r0*r2
	res+=fvec[i+1]*fvec[i+9];
	res+=fvec[i+2]*fvec[i+10];
	res+=fvec[i+3]*fvec[i+11];

	res+=fvec[i+0]*fvec[i+12]; // r0*r3
	res+=fvec[i+1]*fvec[i+13];
	res+=fvec[i+2]*fvec[i+14];
	res+=fvec[i+3]*fvec[i+15];

	res+=fvec[i+4]*fvec[i+8]; // r1*r2
	res+=fvec[i+5]*fvec[i+9];
	res+=fvec[i+6]*fvec[i+10];
	res+=fvec[i+7]*fvec[i+11];

	res+=fvec[i+4]*fvec[i+12]; // r1*r3
	res+=fvec[i+5]*fvec[i+13];
	res+=fvec[i+6]*fvec[i+14];
	res+=fvec[i+7]*fvec[i+15];

	res+=fvec[i+8]*fvec[i+12]; // r2*r3
	res+=fvec[i+9]*fvec[i+13];
	res+=fvec[i+10]*fvec[i+14];
	res+=fvec[i+11]*fvec[i+15];
    }
    return res;
}

float gtest_fmla_b(const float *fvec, int len) {
    float res[4] = {0, 0, 0, 0};
    for (int i=0; i<=len-16; i+=16) {
	res[0]+=fvec[i+0]*fvec[i+0]; // r0*r0
	res[1]+=fvec[i+1]*fvec[i+1];
	res[2]+=fvec[i+2]*fvec[i+2];
	res[3]+=fvec[i+3]*fvec[i+3];

	res[0]+=fvec[i+0]*fvec[i+4]; // r0*r1
	res[1]+=fvec[i+1]*fvec[i+5];
	res[2]+=fvec[i+2]*fvec[i+6];
	res[3]+=fvec[i+3]*fvec[i+7];

	res[0]+=fvec[i+0]*fvec[i+8]; // r0*r2
	res[1]+=fvec[i+1]*fvec[i+9];
	res[2]+=fvec[i+2]*fvec[i+10];
	res[3]+=fvec[i+3]*fvec[i+11];

	res[0]+=fvec[i+0]*fvec[i+12]; // r0*r3
	res[1]+=fvec[i+1]*fvec[i+13];
	res[2]+=fvec[i+2]*fvec[i+14];
	res[3]+=fvec[i+3]*fvec[i+15];


	res[0]+=fvec[i+4]*fvec[i+4]; // r1*r1
	res[1]+=fvec[i+5]*fvec[i+5];
	res[2]+=fvec[i+6]*fvec[i+6];
	res[3]+=fvec[i+7]*fvec[i+7];

	res[0]+=fvec[i+4]*fvec[i+8]; // r1*r2
	res[1]+=fvec[i+5]*fvec[i+9];
	res[2]+=fvec[i+6]*fvec[i+10];
	res[3]+=fvec[i+7]*fvec[i+11];

	res[0]+=fvec[i+4]*fvec[i+12]; // r1*r3
	res[1]+=fvec[i+5]*fvec[i+13];
	res[2]+=fvec[i+6]*fvec[i+14];
	res[3]+=fvec[i+7]*fvec[i+15];


	res[0]+=fvec[i+8]*fvec[i+8]; // r2*r2
	res[1]+=fvec[i+9]*fvec[i+9];
	res[2]+=fvec[i+10]*fvec[i+10];
	res[3]+=fvec[i+11]*fvec[i+11];

	res[0]+=fvec[i+8]*fvec[i+12]; // r2*r3
	res[1]+=fvec[i+9]*fvec[i+13];
	res[2]+=fvec[i+10]*fvec[i+14];
	res[3]+=fvec[i+11]*fvec[i+15];


	res[0]+=fvec[i+12]*fvec[i+12]; // r3*r3
	res[1]+=fvec[i+13]*fvec[i+13];
	res[2]+=fvec[i+14]*fvec[i+14];
	res[3]+=fvec[i+15]*fvec[i+15];
    }
    return res[0]+res[1]+res[2]+res[3];
}

float gtest_fmla_c(const float *fvec, int len) {
    VECTYPE res = {0, 0, 0, 0};

    for (int i=0; i<=len-16; i+=16) {
	VECTYPE r0 = *((const VECTYPE *)&fvec[i+0]);
	VECTYPE r1 = *((const VECTYPE *)&fvec[i+4]);
	VECTYPE r2 = *((const VECTYPE *)&fvec[i+8]);
	VECTYPE r3 = *((const VECTYPE *)&fvec[i+12]);

	res+=r0*r0;
	res+=r0*r1;
	res+=r0*r2;
	res+=r0*r3;

	res+=r1*r1;
	res+=r1*r2;
	res+=r1*r3;

	res+=r2*r2;
	res+=r2*r3;

	res+=r3*r3;
    }
    return res[0]+res[1]+res[2]+res[3];
}

float gtest_fmla_c2(const float *fvec, int len) {
    VECTYPE res0 = {0, 0, 0, 0};
    VECTYPE res1 = {0, 0, 0, 0};

    for (int i=0; i<=len-16; i+=16) {
	VECTYPE r0 = *((const VECTYPE *)&fvec[i+0]);
	VECTYPE r1 = *((const VECTYPE *)&fvec[i+4]);
	VECTYPE r2 = *((const VECTYPE *)&fvec[i+8]);
	VECTYPE r3 = *((const VECTYPE *)&fvec[i+12]);

	res0+=r0*r0;
	res1+=r0*r1;
	res0+=r0*r2;
	res1+=r0*r3;

	res0+=r1*r1;
	res1+=r1*r2;
	res0+=r1*r3;

	res1+=r2*r2;
	res0+=r2*r3;

	res1+=r3*r3;
    }
    res0+=res1;
    return res0[0]+res0[1]+res0[2]+res0[3];
}
