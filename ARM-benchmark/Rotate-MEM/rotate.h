#include <arm_neon.h>

struct _cf32x4_t {
    float32x4_t re;
    float32x4_t im;
};

void rotate(struct _cf32x4_t *base, const struct _cf32x4_t *phase);
