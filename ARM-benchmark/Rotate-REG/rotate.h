#include <arm_neon.h>

struct _cf32x4_t {
    float32x4_t re;
    float32x4_t im;
};

struct _cf32x4_t rotate(struct _cf32x4_t base, struct _cf32x4_t phase);
