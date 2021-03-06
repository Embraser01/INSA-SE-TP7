/*
 Copyright (C) 2006 Pedro Felzenszwalb

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <common.h>
#include <sys/time.h>

#define MULTIPLIER 10

unsigned char in[MAX_WIDTH][MAX_HEIGHT];
unsigned char out[MAX_WIDTH][MAX_HEIGHT];
unsigned char correct_answer[MAX_WIDTH][MAX_HEIGHT];

extern void deriche_float(int width, int height);
extern void oracle(int width, int height);


int main(int argc, char **argv) {

    int width, height;
    struct timeval start, end;
    double delta, delta_oracle;

    fprintf(stderr, "usage: %s in.pgm out.pgm\n", argv[0]);


    ///////////////////////////
    ////// LOADING IMAGE //////
    ///////////////////////////

    load_pgm(argv[1], &width, &height, in);
    fprintf(stdout, "loaded ...\n");


    //////////////////////////////////
    ////// OPTIMIZED ALGORITHM //////
    /////////////////////////////////

    gettimeofday(&start, NULL);
    for (unsigned int i = MULTIPLIER; i--;) deriche_float(width, height);
    gettimeofday(&end, NULL);

    delta = (((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6) / MULTIPLIER;

    printf("Temps avec optimisation : %f sec\n", delta);


    //////////////////////////
    ////// SAVING IMAGE //////
    //////////////////////////

    save_pgm(argv[2], width, height, out);


    ////////////////////////////////
    ////// ORIGINAL ALGORITHM //////
    ////////////////////////////////

    gettimeofday(&start, NULL);
    oracle(width, height);
    gettimeofday(&end, NULL);

    delta_oracle = (((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6);

    printf("Temps sans optimisation : %f sec\n", delta_oracle);


    double ratio = delta_oracle / delta;
    printf("La version optimisée est %.2f fois plus rapide que l'originale\n", ratio);


    //////////////////////////
    ////// RESULT CHECK //////
    //////////////////////////

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (abs(out[i][j] - correct_answer[i][j]) > 100) {
                fprintf(stderr, "Résultat incorrect à i=%d et j=%d !\n", i, j);
                exit(0);
            }
        }
    }

    printf("Résultat correct.\n");

    return 0;
}
