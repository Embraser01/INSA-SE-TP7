#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <common.h>
#include <pthread.h>

extern unsigned char in[MAX_WIDTH][MAX_HEIGHT];
extern unsigned char out[MAX_WIDTH][MAX_HEIGHT];
extern unsigned char correct_answer[MAX_WIDTH][MAX_HEIGHT];

float tmp1[MAX_WIDTH][MAX_HEIGHT];
float tmp2[MAX_WIDTH][MAX_HEIGHT];
float tmp3[MAX_WIDTH][MAX_HEIGHT];
float tmp4[MAX_WIDTH][MAX_HEIGHT];
float tmp5[MAX_WIDTH][MAX_HEIGHT];

int l1Treated = 0;
int l2Treated = 0;
int l4Treated = 0;
int l5Treated = 0;

int *l1Tab;
int *l2Tab;
int *l4Tab;
int *l5Tab;

pthread_mutex_t l1MutexTab = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t l2MutexTab = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t l4MutexTab = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t l5MutexTab = PTHREAD_MUTEX_INITIALIZER;


void *L1(void *data);

void *L2(void *data);

void *L5(void *data);

void *L4(void *data);

void *L1_line(void *data);

void *L2_line(void *data);

void *L4_line(void *data);

void *L5_line(void *data);

void *L1_pool(void *data);

void *L2_pool(void *data);

void *L4_pool(void *data);

void *L5_pool(void *data);

void deriche_float(int width, int height)
{
    int i, j;

    float c1, c2;

    c1 = c2 = 1;

    pthread_t pL1;
    pthread_t pL2;
    pthread_t pL4;
    pthread_t pL5;

    int sizeData[2];
    sizeData[0] = width;
    sizeData[1] = height;


    pthread_create(&pL1, NULL, L1, sizeData);
    pthread_create(&pL2, NULL, L2, sizeData);

    pthread_join((pthread_t) pL1, NULL);
    pthread_join((pthread_t) pL2, NULL);


    // L3:

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            tmp3[i][j] = (c1 * (tmp1[i][j] + tmp2[i][j]));
        }
    }

    pthread_create(&pL4, NULL, L4, sizeData);
    pthread_create(&pL5, NULL, L5, sizeData);

    pthread_join((pthread_t) pL4, NULL);
    pthread_join((pthread_t) pL5, NULL);


    // L6:

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            out[i][j] = (c2 * (tmp4[i][j] + tmp5[i][j]));
            if (out[i][j] > 25)
            {
                out[i][j] = 0;
            } else
            {
                out[i][j] = 255;
            }
        }
    }

}

void *L1(void *data)
{
    int i;

    int *dataBis = data;

    int width = dataBis[0];

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);
    l1Tab = malloc(sizeof(int) * width);

    for (i = 0; i < width; ++i)
    {
        l1Tab[i] = 0;
    }


    // L1:

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_create(&pool[i], NULL, L1_pool, data);
    }

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_join(pool[i], NULL);
    }

    // TODO FREE
    return NULL;
}

void *L1_pool(void *data)
{
    int *dataBis = data;

    int width = dataBis[0];
    int height = dataBis[1];

    pthread_t pthread;

    int i;
    int tab[2];
    tab[1] = height;

    while (l1Treated < width)
    {
        // Search for available col

        pthread_mutex_lock(&l1MutexTab);
        for (i = 0; i < width; ++i)
        {
            if (tab[i] == 0)
            {
                l1Tab[i] = 1;
                l1Treated++;
                tab[0] = i;
                break;
            }
        }
        pthread_mutex_unlock(&l1MutexTab);

        pthread_create(&pthread, NULL, L1_line, tab);
        pthread_join(pthread, NULL);

    }
    return NULL;
}


void *L1_line(void *data)
{
    int *dataBis = data;

    int i = dataBis[0];
    int height = dataBis[1];
    int j;

    float xm1, ym1, ym2;
    float a1, a2;
    float b1, b2;

    a1 = (float) ((-0.188682));
    a2 = ((0.110209));
    b1 = ((0.840896));
    b2 = (float) ((-0.606531));

    ym1 = 0, ym2 = 0, xm1 = 0;

    for (j = 0; j < height; j++)
    {
        tmp1[i][j] = (a1 * in[i][j] + a2 * xm1 + b1 * ym1 + b2 * ym2);
        xm1 = in[i][j];
        ym2 = ym1;
        ym1 = tmp1[i][j];
    }
    return NULL;
}

void *L2(void *data)
{
    int i;

    int *dataBis = data;

    int width = dataBis[0];

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);
    l2Tab = malloc(sizeof(int) * width);
    for (i = 0; i < width; ++i)
    {
        l2Tab[i] = 0;
    }

    // L1:

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_create(&pool[i], NULL, L2_pool, data);
    }

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_join(pool[i], NULL);
    }

    // TODO FREE
    return NULL;
}

void *L2_pool(void *data)
{
    int *dataBis = data;

    int width = dataBis[0];
    int height = dataBis[1];

    pthread_t pthread;

    int i;
    int tab[2];
    tab[1] = height;

    while (l2Treated < width)
    {
        // Search for available col

        pthread_mutex_lock(&l2MutexTab);
        for (i = 0; i < width; ++i)
        {
            if (tab[i] == 0)
            {
                l2Tab[i] = 1;
                l2Treated++;
                tab[0] = i;
                break;
            }
        }
        pthread_mutex_unlock(&l2MutexTab);

        pthread_create(&pthread, NULL, L2_line, tab);
        pthread_join(pthread, NULL);

    }
    return NULL;
}

void *L2_line(void *data)
{
    int *dataBis = data;

    int i = dataBis[0];
    int height = dataBis[1];
    int j;

    float xp1, xp2;
    float yp1, yp2;
    float a1, a3;
    float b1, b2;

    a1 = (float) ((-0.188682));
    a3 = (float) ((-0.183682));
    b1 = ((0.840896));
    b2 = (float) ((-0.606531));


    yp1 = 0, yp2 = 0, xp1 = 0, xp2 = 0;
    for (j = height - 1; j >= 0; j--)
    {
        tmp2[i][j] = (a3 * xp1 + a1 * xp2 + b1 * yp1 + b2 * yp2);
        xp2 = xp1;
        xp1 = in[i][j];
        yp2 = yp1;
        yp1 = tmp2[i][j];
    }
    return NULL;
}


void *L4(void *data)
{
    int i;

    int *dataBis = data;

    int height = dataBis[1];

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);
    l4Tab = malloc(sizeof(int) * height);

    for (i = 0; i < height; ++i)
    {
        l4Tab[i] = 0;
    }

    // L1:

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_create(&pool[i], NULL, L4_pool, data);
    }

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_join(pool[i], NULL);
    }

    // TODO FREE
    return NULL;
}

void *L4_pool(void *data)
{
    int *dataBis = data;

    int width = dataBis[0];
    int height = dataBis[1];

    pthread_t pthread;

    int j;
    int tab[2];
    tab[0] = width;

    while (l4Treated < width)
    {
        // Search for available col

        pthread_mutex_lock(&l4MutexTab);
        for (j = 0; j < height; ++j)
        {
            if (tab[j] == 0)
            {
                l4Tab[j] = 1;
                l4Treated++;
                tab[1] = j;
                break;
            }
        }
        pthread_mutex_unlock(&l4MutexTab);

        pthread_create(&pthread, NULL, L4_line, tab);
        pthread_join(pthread, NULL);

    }
    return NULL;
}

void *L4_line(void *data)
{
    int *dataBis = data;

    int width = dataBis[0];
    int j = dataBis[1];
    int i;

    float tm1, ym1, ym2;
    float a5, a6;
    float b1, b2;

    a5 = (float) ((-0.188682));
    a6 = ((0.110209));
    b1 = ((0.840896));
    b2 = (float) ((-0.606531));

    tm1 = 0, ym1 = 0, ym2 = 0;
    for (i = 0; i < width; i++)
    {
        tmp4[i][j] = (a5 * tmp3[i][j] + a6 * tm1 + b1 * ym1 + b2 * ym2);
        tm1 = tmp3[i][j];
        ym2 = ym1;
        ym1 = tmp4[i][j];
    }
    return NULL;
}


void *L5(void *data)
{
    int i;

    int *dataBis = data;

    int height = dataBis[1];

    pthread_t *pool = malloc(sizeof(pthread_t) * MAX_THREAD);
    l5Tab = malloc(sizeof(int) * height);
    for (i = 0; i < height; ++i)
    {
        l5Tab[i] = 0;
    }

    // L1:

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_create(&pool[i], NULL, L5_pool, data);
    }

    for (i = 0; i < MAX_THREAD; i++)
    {
        pthread_join(pool[i], NULL);
    }

    // TODO FREE
    return NULL;
}

void *L5_pool(void *data)
{
    int *dataBis = data;

    int width = dataBis[0];
    int height = dataBis[1];

    pthread_t pthread;

    int j;
    int tab[2];
    tab[0] = width;

    while (l5Treated < width)
    {
        // Search for available col

        pthread_mutex_lock(&l5MutexTab);
        for (j = 0; j < height; ++j)
        {
            if (tab[j] == 0)
            {
                l5Tab[j] = 1;
                l5Treated++;
                tab[1] = j;
                break;
            }
        }
        pthread_mutex_unlock(&l5MutexTab);

        pthread_create(&pthread, NULL, L5_line, tab);
        pthread_join(pthread, NULL);

    }
    return NULL;
}

void *L5_line(void *data)
{
    int *dataBis = data;

    int width = dataBis[0];
    int j = dataBis[1];
    int i;

    float tp1, tp2;
    float yp1, yp2;
    float a7, a8;
    float b1, b2;

    a7 = (float) ((-0.183682));
    a8 = ((0.114441));
    b1 = ((0.840896));
    b2 = (float) ((-0.606531));

    tp1 = 0, tp2 = 0, yp1 = 0, yp2 = 0;
    for (i = width - 1; i >= 0; i--)
    {
        tmp5[i][j] = (a7 * tp1 + a8 * tp2 + b1 * yp1 + b2 * yp2);
        tp2 = tp1;
        tp1 = tmp3[i][j];
        yp2 = yp1;
        yp1 = tmp5[i][j];
    }
    return NULL;
}


void oracle(int width, int height)
{
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

    for (i = 0; i < width; i++)
    {
        ym1 = 0, ym2 = 0, xm1 = 0;
        for (j = 0; j < height; j++)
        {
            tmp1[i][j] = (a1 * in[i][j] + a2 * xm1 + b1 * ym1 + b2 * ym2);
            xm1 = in[i][j];
            ym2 = ym1;
            ym1 = tmp1[i][j];
        }
    }

// L2:

    for (i = 0; i < width; i++)
    {
        yp1 = 0, yp2 = 0, xp1 = 0, xp2 = 0;
        for (j = height - 1; j >= 0; j--)
        {
            tmp2[i][j] = (a3 * xp1 + a1 * xp2 + b1 * yp1 + b2 * yp2);
            xp2 = xp1;
            xp1 = in[i][j];
            yp2 = yp1;
            yp1 = tmp2[i][j];
        }
    }

// L3:

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            tmp3[i][j] = (c1 * (tmp1[i][j] + tmp2[i][j]));
        }
    }

// L4:

    for (j = 0; j < height; j++)
    {
        tm1 = 0, ym1 = 0, ym2 = 0;
        for (i = 0; i < width; i++)
        {
            tmp1[i][j] = (a5 * tmp3[i][j] + a6 * tm1 + b1 * ym1 + b2 * ym2);
            tm1 = tmp3[i][j];
            ym2 = ym1;
            ym1 = tmp1[i][j];
        }
    }
// L5:

    for (j = 0; j < height; j++)
    {
        tp1 = 0, tp2 = 0, yp1 = 0, yp2 = 0;
        for (i = width - 1; i >= 0; i--)
        {
            tmp2[i][j] = (a7 * tp1 + a8 * tp2 + b1 * yp1 + b2 * yp2);
            tp2 = tp1;
            tp1 = tmp3[i][j];
            yp2 = yp1;
            yp1 = tmp2[i][j];
        }
    }
// L6:

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            correct_answer[i][j] = (c2 * (tmp1[i][j] + tmp2[i][j]));
            if (correct_answer[i][j] > 25)
            {
                correct_answer[i][j] = 0;
            } else
            {
                correct_answer[i][j] = 255;
            }
        }
    }

}