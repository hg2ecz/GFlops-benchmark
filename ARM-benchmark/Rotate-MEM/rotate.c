#include "rotate.h"

void rotate(struct _cf32x4_t *base, const struct _cf32x4_t *phase) {
    for (int i=0; i<1000*1000; i++) {
        float32x4_t base_tmpre = base[i].re;
        base[i].re = base_tmpre * phase[i].re - base[i].im * phase[i].im;
        base[i].im = base_tmpre * phase[i].im + base[i].im * phase[i].re;
    }
}
