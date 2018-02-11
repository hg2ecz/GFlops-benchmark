float gtest_fmla_a(const float *fvec, int len);
float gtest_fmla_b(const float *fvec, int len);
float gtest_fmla_vec(const float *fvec, int len);
float gtest_fmla_doublevec(const float *fvec, int len);

#if defined (__AVX__)
  float gtest_fmla_avx(const float *fvec, int len);
#endif

#if defined (__AVX512F__)
  float gtest_fmla_avx512f(const float *fvec, int len);
#endif

