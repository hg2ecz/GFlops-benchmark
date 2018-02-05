#include <arm_neon.h>

float gtest_float(const float *fvec, int len);
float gtest_float4(const float *fvec, int len);
float gtest_float16(const float *fvec, int len);
float gtest_float32(const float *fvec, int len);
float gtest_float32x4(float32x4_t *fvec, int len);
