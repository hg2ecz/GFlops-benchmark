#if defined(__arm__) || defined(__aarch64__)
  #include <arm_neon.h>
  typedef float32x4_t VECTYPE;
#elif defined (__SSE__)
  #include <smmintrin.h>
  typedef __v4sf VECTYPE;
#endif

float gtest_fmla_a(const float *fvec, int len);
float gtest_fmla_b(const float *fvec, int len);
float gtest_fmla_vec(const float *fvec, int len);
float gtest_fmla_doublevec(const float *fvec, int len);

#if defined (__AVX__)
  #include <immintrin.h>
  float gtest_fmla_avx(const float *fvec, int len);
#endif

#if defined (__AVX512F__)
  #include <immintrin.h>
  float gtest_fmla_avx512f(const float *fvec, int len);
#endif

