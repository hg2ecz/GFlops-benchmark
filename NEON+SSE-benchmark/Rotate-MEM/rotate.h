#if defined(__arm__) || defined(__aarch64__)
  #include <arm_neon.h>
  typedef float32x4_t VECTYPE;
#else
  #include <smmintrin.h>
  typedef __v4sf VECTYPE;
#endif

struct _cf32x4_t {
    VECTYPE re;
    VECTYPE im;
};

void rotate(struct _cf32x4_t *base, const struct _cf32x4_t *phase);
