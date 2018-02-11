#if defined(__arm__) || defined(__aarch64__)
  #include <arm_neon.h>
  typedef float32x4_t VECTYPE;
#else
  #include <smmintrin.h>
  typedef __v4sf VECTYPE;
#endif

float gtest_float(const float *fvec, int len);
float gtest_float4(const float *fvec, int len);
float gtest_float16(const float *fvec, int len);
float gtest_float32(const float *fvec, int len);
float gtest_float_vec(const float *fvec, int len);
