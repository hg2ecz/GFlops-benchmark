float gtest_float(const float *fvec, int len);
float gtest_float4(const float *fvec, int len);
float gtest_float16(const float *fvec, int len);
float gtest_float32(const float *fvec, int len);
float gtest_float_vec(const float *fvec, int len);

#if defined(__AVX__)
  float gtest_float_avx(const float *fvec, int len);
#endif

#if defined(__AVX512F__)
  float gtest_float_avx512f(const float *fvec, int len);
#endif
