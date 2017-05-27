#include <stdio.h>
#include <stdlib.h>
#include <common.h>
#include <pthread.h>

extern unsigned char in[MAX_WIDTH][MAX_HEIGHT];
extern unsigned char out[MAX_WIDTH][MAX_HEIGHT];
extern unsigned char correct_answer[MAX_WIDTH][MAX_HEIGHT];

static float tmp1[MAX_WIDTH][MAX_HEIGHT];
static float tmp2[MAX_WIDTH][MAX_HEIGHT];
static float tmp3[MAX_WIDTH][MAX_HEIGHT];
static float tmp4[MAX_WIDTH][MAX_HEIGHT];
static float tmp5[MAX_WIDTH][MAX_HEIGHT];

static unsigned int treatedL1 = 0;
static unsigned int treatedL2 = 0;
static unsigned int treatedL3 = 0;
static unsigned int treatedL4 = 0;
static unsigned int treatedL5 = 0;
static unsigned int treatedL6 = 0;

static pthread_mutex_t mutexL1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexL2 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexL3 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexL4 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexL5 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexL6 = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutex4[MAX_WIDTH][MAX_HEIGHT];
static pthread_mutex_t mutex5[MAX_WIDTH][MAX_HEIGHT];


//===================//
//===== DEFINES =====//
//===================//


#define A1 -0.188682f
#define A2 0.110209f
#define A3 -0.183682f
#define A5 -0.188682f
#define A6 0.110209f
#define A7 -0.183682f
#define A8 0.114441f
#define B1 0.840896f
#define B2 -0.606531f
#define C1 1
#define C2 1

//=======================//
//===== DECLARATION =====//
//=======================//


static void *L1(void *data);
static void *L2(void *data);
static void *L4(void *data);
static void *L5(void *data);
static void *L1_pool(void *data);
static void *L2_pool(void *data);
static void *L3_pool(void *data);
static void *L4_pool(void *data);
static void *L5_pool(void *data);
static void *L6_pool(void *data);

//===================//
//===== DERICHE =====//
//===================//


void deriche_float(int width, int height) {

    pthread_t pL1;
    pthread_t pL2;
    pthread_t pL4;
    pthread_t pL5;

    pthread_t *pool;

    unsigned int i;
    unsigned int sizeData[2];

    sizeData[0] = (unsigned int) width;
    sizeData[1] = (unsigned int) height;

    treatedL1 = 0;
    treatedL2 = 0;
    treatedL3 = 0;
    treatedL4 = 0;
    treatedL5 = 0;
    treatedL6 = 0;

    // L1 & L2

    pthread_create(&pL1, NULL, L1, sizeData);
    pthread_create(&pL2, NULL, L2, sizeData);

    // Let's use this time to do something useful

    pool = malloc(sizeof(pthread_t) * MAX_THREAD * 2); // For L3 & L6

    // Now, we are ready to wait

    pthread_join((pthread_t) pL1, NULL);
    pthread_join((pthread_t) pL2, NULL);

    // L3:

    for (i = MAX_THREAD * 2; i--;) pthread_create(&pool[i], NULL, L3_pool, sizeData);
    for (i = MAX_THREAD * 2; i--;) pthread_join(pool[i], NULL);


    // L4 & L5

    pthread_create(&pL4, NULL, L4, sizeData);
    pthread_create(&pL5, NULL, L5, sizeData);

    pthread_join((pthread_t) pL4, NULL);
    pthread_join((pthread_t) pL5, NULL);

    // L6:

    for (i = MAX_THREAD * 2; i--;) pthread_create(&pool[i], NULL, L6_pool, sizeData);
    for (i = MAX_THREAD * 2; i--;) pthread_join(pool[i], NULL);

}

//=================//
//===== STEPS =====//
//=================//


void *L1(void *data) {
    unsigned int i;

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);

    // L1:

    for (i = MAX_THREAD; i--;) pthread_create(&pool[i], NULL, L1_pool, data);
    for (i = MAX_THREAD; i--;) pthread_join(pool[i], NULL);

    return NULL;
}

void *L2(void *data) {
    unsigned int i;

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);

    // L2:

    for (i = MAX_THREAD; i--;) pthread_create(&pool[i], NULL, L2_pool, data);
    for (i = MAX_THREAD; i--;) pthread_join(pool[i], NULL);

    return NULL;
}

void *L4(void *data) {
    unsigned int i;

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);

    // L4:

    for (i = MAX_THREAD; i--;) pthread_create(&pool[i], NULL, L4_pool, data);
    for (i = MAX_THREAD; i--;) pthread_join(pool[i], NULL);

    return NULL;
}

void *L5(void *data) {
    unsigned int i;

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);

    // L5:

    for (i = MAX_THREAD; i--;) pthread_create(&pool[i], NULL, L5_pool, data);
    for (i = MAX_THREAD; i--;) pthread_join(pool[i], NULL);


    return NULL;
}

//=======================//
//===== THREAD POOL =====//
//=======================//


void *L1_pool(void *data) {
    unsigned int *dataBis = data;

    unsigned int width = dataBis[0];
    unsigned int height = dataBis[1];
    unsigned int i, j;
    unsigned int until;

    float xm1, ym1, ym2;

    while (1) {
        // Search for available col

        pthread_mutex_lock(&mutexL1);
        if (treatedL1 >= width) {
            pthread_mutex_unlock(&mutexL1);
            return NULL;
        }
        i = treatedL1;
        until = i + NB_LINE_PER_THREAD > width ? width : i + NB_LINE_PER_THREAD;
        treatedL1 = until;
        pthread_mutex_unlock(&mutexL1);

        for (; i < until; ++i) {
            ym1 = 0, ym2 = 0, xm1 = 0;
            for (j = 0; j < height; j++) {
                tmp1[i][j] = (A1 * in[i][j] + A2 * xm1 + B1 * ym1 + B2 * ym2);
                xm1 = in[i][j];
                ym2 = ym1;
                ym1 = tmp1[i][j];
            }
        }
    }
    return NULL;
}

void *L2_pool(void *data) {
    unsigned int *dataBis = data;

    unsigned int width = dataBis[0];
    unsigned int height = dataBis[1];
    unsigned int i, j;
    unsigned int until;

    float xp1, xp2;
    float yp1, yp2;


    while (1) {
        // Search for available col

        pthread_mutex_lock(&mutexL2);
        if (treatedL2 >= width) {
            pthread_mutex_unlock(&mutexL2);
            return NULL;
        }
        i = treatedL2;
        until = i + NB_LINE_PER_THREAD > width ? width : i + NB_LINE_PER_THREAD;
        treatedL2 = until;
        pthread_mutex_unlock(&mutexL2);


        for (; i < until; ++i) {
            yp1 = 0, yp2 = 0, xp1 = 0, xp2 = 0;

            for (j = height; j--;) {
                tmp2[i][j] = (A3 * xp1 + A1 * xp2 + B1 * yp1 + B2 * yp2);
                xp2 = xp1;
                xp1 = in[i][j];
                yp2 = yp1;
                yp1 = tmp2[i][j];
            }
        }
    }
    return NULL;
}

void *L3_pool(void *data) {
    unsigned int *dataBis = data;

    unsigned int width = dataBis[0];
    unsigned int height = dataBis[1];
    unsigned int i, j;
    unsigned int until;

    while (1) {
        // Search for available col

        pthread_mutex_lock(&mutexL3);
        if (treatedL3 >= width) {
            pthread_mutex_unlock(&mutexL3);
            return NULL;
        }
        i = treatedL3;
        until = i + NB_LINE_PER_THREAD > width ? width : i + NB_LINE_PER_THREAD;
        treatedL3 = until;
        pthread_mutex_unlock(&mutexL3);


        for (; i < until; ++i) {
            if (i == 0) continue;

            for (j = height; j--;) {
                tmp3[i][j] = (C1 * (tmp1[i][j] + tmp2[i][j]));
                pthread_mutex_init(&mutex4[i][j], NULL);
                pthread_mutex_init(&mutex5[i][j], NULL);
                pthread_mutex_lock(&mutex4[i][j]);
                pthread_mutex_lock(&mutex5[i][j]);

            }
        }
    }
    return NULL;
}

void *L4_pool(void *data) {
    unsigned int *dataBis = data;

    unsigned int width = dataBis[0];
    unsigned int height = dataBis[1];
    unsigned int i, j;
    unsigned int until;

    float tm1, ym1, ym2;

    while (1) {
        // Search for available col

        pthread_mutex_lock(&mutexL4);
        if (treatedL4 >= width) {
            pthread_mutex_unlock(&mutexL4);
            return NULL;
        }
        i = treatedL4;
        until = i + NB_LINE_PER_THREAD > width ? width : i + NB_LINE_PER_THREAD;
        treatedL4 = until;
        pthread_mutex_unlock(&mutexL4);

        for (; i < until; ++i) {
            tm1 = 0, ym1 = 0, ym2 = 0;
            for (j = 0; j < height; j++) {
                if (i != 0) pthread_mutex_lock(&mutex4[i - 1][j]);

                tmp4[i][j] = (A5 * tmp3[i][j] + A6 * tm1 + B1 * ym1 + B2 * ym2);
                tm1 = tmp3[i][j];
                ym2 = ym1;
                ym1 = tmp4[i][j];

                pthread_mutex_unlock(&mutex4[i][j]);
            }
        }
    }

    return NULL;
}

void *L5_pool(void *data) {
    unsigned int *dataBis = data;

    unsigned int width = dataBis[0];
    unsigned int height = dataBis[1];
    unsigned int i, j;
    unsigned int until;

    float tp1, tp2;
    float yp1, yp2;


    while (1) {
        // Search for available col

        pthread_mutex_lock(&mutexL5);
        if (treatedL5 >= width) {
            pthread_mutex_unlock(&mutexL5);
            return NULL;
        }
        i = treatedL5;
        until = i + NB_LINE_PER_THREAD > width ? width : i + NB_LINE_PER_THREAD;
        treatedL5 = until;
        pthread_mutex_unlock(&mutexL5);

        for (; i < until; ++i) {
            tp1 = 0, tp2 = 0, yp1 = 0, yp2 = 0;
            for (j = 0; j < height; j++) {
                if (i != 0) pthread_mutex_lock(&mutex5[i - 1][j]);

                tmp5[i][j] = (A7 * tp1 + A8 * tp2 + B1 * yp1 + B2 * yp2);
                tp2 = tp1;
                tp1 = tmp3[i][j];
                yp2 = yp1;
                yp1 = tmp5[i][j];

                pthread_mutex_unlock(&mutex5[i][j]);
            }
        }
    }
    return NULL;
}

void *L6_pool(void *data) {
    unsigned int *dataBis = data;

    unsigned int width = dataBis[0];
    unsigned int height = dataBis[1];
    unsigned int i, j;
    unsigned int until;

    while (1) { // Infinite loop
        // Search for available col
        pthread_mutex_lock(&mutexL6);
        if (treatedL6 >= width) {
            pthread_mutex_unlock(&mutexL6);
            return NULL;
        }
        i = treatedL6;
        until = i + NB_LINE_PER_THREAD > width ? width : i + NB_LINE_PER_THREAD;
        treatedL6 = until;
        pthread_mutex_unlock(&mutexL6);

        for (; i < until; ++i) {
            for (j = height; j--;) {
                out[i][j] = (unsigned char) ((unsigned char) (C2 * (tmp4[i][j] + tmp5[i][j])) > 25 ? 0 : 255);
            }
        }
    }
}

//========================//
//===== VERIFICATION =====//
//========================//

void oracle(int width, int height) {
    int i, j;

    float xm1, tm1, ym1, ym2;
    float xp1, xp2;
    float tp1, tp2;
    float yp1, yp2;
    float a1, a2, a3, a5, a6, a7, a8;
    float b1, b2, c1, c2;

    a1 = a5 = ((-0.188682));
    a2 = a6 = ((0.110209));
    a3 = a7 = ((-0.183682));
    a8 = ((0.114441));
    b1 = ((0.840896));
    b2 = ((-0.606531));
    c1 = c2 = (int) (1);



// L1:

    for (i = 0; i < width; i++) {
        ym1 = 0, ym2 = 0, xm1 = 0;
        for (j = 0; j < height; j++) {
            tmp1[i][j] = (a1 * in[i][j] + a2 * xm1 + b1 * ym1 + b2 * ym2);
            xm1 = in[i][j];
            ym2 = ym1;
            ym1 = tmp1[i][j];
        }
    }

// L2:

    for (i = 0; i < width; i++) {
        yp1 = 0, yp2 = 0, xp1 = 0, xp2 = 0;
        for (j = height - 1; j >= 0; j--) {
            tmp2[i][j] = (a3 * xp1 + a1 * xp2 + b1 * yp1 + b2 * yp2);
            xp2 = xp1;
            xp1 = in[i][j];
            yp2 = yp1;
            yp1 = tmp2[i][j];
        }
    }

// L3:

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            tmp3[i][j] = (c1 * (tmp1[i][j] + tmp2[i][j]));
        }
    }

// L4:

    for (j = 0; j < height; j++) {
        tm1 = 0, ym1 = 0, ym2 = 0;
        for (i = 0; i < width; i++) {
            tmp1[i][j] = (a5 * tmp3[i][j] + a6 * tm1 + b1 * ym1 + b2 * ym2);
            tm1 = tmp3[i][j];
            ym2 = ym1;
            ym1 = tmp1[i][j];
        }
    }
// L5:

    for (j = 0; j < height; j++) {
        tp1 = 0, tp2 = 0, yp1 = 0, yp2 = 0;
        for (i = width - 1; i >= 0; i--) {
            tmp2[i][j] = (a7 * tp1 + a8 * tp2 + b1 * yp1 + b2 * yp2);
            tp2 = tp1;
            tp1 = tmp3[i][j];
            yp2 = yp1;
            yp1 = tmp2[i][j];
        }
    }
// L6:

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            correct_answer[i][j] = (c2 * (tmp1[i][j] + tmp2[i][j]));
            if (correct_answer[i][j] > 25) {
                correct_answer[i][j] = 0;
            } else {
                correct_answer[i][j] = 255;
            }
        }
    }

}