#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include "rotate.h"
#include <pthread.h>

struct _thread_info {
   pthread_t thread_id;
   int       thread_num;
};

static void *comp_thread(void *arg) {
    struct _thread_info *tinfo = arg;

    struct _cf32x4_t *base = malloc(1000*1000*sizeof (struct _cf32x4_t));
    struct _cf32x4_t *phase = malloc(1000*1000*sizeof (struct _cf32x4_t));
    struct _cf32x4_t res = { {0., 0., 0., 0.}, {0., 0., 0., 0.} };

    base[0].re[0] = cos(0./180.*M_PI);
    base[0].im[0] = sin(0./180.*M_PI);
    base[0].re[1] = cos(15./180.*M_PI);
    base[0].im[1] = sin(15./180.*M_PI);
    base[0].re[2] = cos(30./180.*M_PI);
    base[0].im[2] = sin(30./180.*M_PI);
    base[0].re[3] = cos(45./180.*M_PI);
    base[0].im[3] = sin(45./180.*M_PI);

    phase[0].re[0] = phase[0].re[1] = phase[0].re[2] = phase[0].re[3] = cos(45./180.*M_PI);
    phase[0].im[0] = phase[0].im[1] = phase[0].im[2] = phase[0].im[3] = sin(45./180.*M_PI);

    for (int i=0; i<ITERNUM_MILL; i++) {
        struct _cf32x4_t restmp;
        restmp = rotate(base, phase);
        res.re += restmp.re;
        res.im += restmp.im;
    }
    printf("Thread#%d re: %f %f %f %f\n", tinfo->thread_num, res.re[0], res.re[1], res.re[2], res.re[3]);
    printf("Thread#%d im: %f %f %f %f\n", tinfo->thread_num, res.im[0], res.im[1], res.im[2], res.im[3]);

    return 0;
}

int main() {
    struct _thread_info *tinfo = calloc(THREADNUM, sizeof(struct _thread_info));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    printf("Rotate: %d million cf32 x THREADNUM = %d million cf32\n", 4*ITERNUM_MILL, THREADNUM * 4*ITERNUM_MILL);

    for (int i=0; i<THREADNUM; i++) {
	tinfo[i].thread_num = i;
        int s = pthread_create(&tinfo[i].thread_id, &attr, comp_thread, &tinfo[i]);
        if (s != 0) {
	    fprintf(stderr, "Error: %d\n", s);
	    perror("Thread hiba");
	}
    }
    void *retval;
    for (int i=0; i<THREADNUM; i++) pthread_join(tinfo[i].thread_id, &retval);
}
