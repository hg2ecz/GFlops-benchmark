#if defined(__arm__) || defined(__aarch64__)
  #include <arm_neon.h>
  typedef float32x4_t VECTYPE;
#else
  #include <smmintrin.h>
  typedef __v4sf VECTYPE;
#endif

float gtest_fmla_a(const float *fvec, int len);
float gtest_fmla_b(const float *fvec, int len);
float gtest_fmla_c(const float *fvec, int len);
float gtest_fmla_c2(const float *fvec, int len);
