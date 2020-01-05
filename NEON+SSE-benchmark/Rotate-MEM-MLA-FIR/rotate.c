#include "rotate.h"

void rotate(struct _cf32x4_t *base, const struct _cf32x4_t *phase) {
    for (int i=0; i<1000*1000/500; i++) {
        struct _cf32x4_t res = { {0., 0., 0., 0.}, {0., 0., 0., 0.} };
        for (int k=0; k<4; k++) {
            for (int j=0; j<125; j++) {
                float32x4_t b_re = *(float32x4_t*)(((float *)&base[i+j].re) + k);
                float32x4_t b_im = *(float32x4_t*)(((float *)&base[i+j].re) + k);
                res.re += b_re * phase[j].re - b_im * phase[j].im;
                res.im += b_re * phase[j].im + b_im * phase[j].re;
            }
            base[i].re[k] = res.re[0] + res.re[1] + res.re[2] + res.re[3];
            base[i].im[k] = res.im[0] + res.im[1] + res.im[2] + res.im[3];
        }
    }
}
