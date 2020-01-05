#include "rotate.h"

struct _cf32x4_t rotate(const struct _cf32x4_t *base, const struct _cf32x4_t *phase) {
    struct _cf32x4_t res = { {0., 0., 0., 0.}, {0., 0., 0., 0.} };
    for (int i=0; i<1000*1000; i++) {
        res.re += base[i].re * phase[i].re - base[i].im * phase[i].im;
        res.im += base[i].re * phase[i].im + base[i].im * phase[i].re;
    }
    return res;
}
