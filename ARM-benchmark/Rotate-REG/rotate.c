#include "rotate.h"

struct _cf32x4_t rotate(struct _cf32x4_t base, struct _cf32x4_t phase) {
    for (int i=0; i<1000*1000; i++) {
        float32x4_t base_tmpre = base.re;
        base.re = base_tmpre * phase.re - base.im * phase.im;
        base.im = base_tmpre * phase.im + base.im * phase.re;
    }
    return base;
}
