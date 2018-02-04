#include <arm_neon.h>

#define THREADNUM 4

float gtest_float(float *fvec, int len);
float gtest_float4(float *fvec, int len);
float gtest_float32x4(float32x4_t *fvec, int len);
