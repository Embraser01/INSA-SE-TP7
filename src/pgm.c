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
#include <string.h>
#include <stdlib.h>
#include <assert.h>
//#include <math.h>
#include <common.h>


int verbose = 1;

int load_pgm(char* name, int* width, int* height, unsigned char img[MAX_WIDTH][MAX_HEIGHT]) {
	FILE* imagefile = fopen(name, "r");
	int depth = 0;
	int h, w;
	char comment[128];
	if (imagefile != NULL) {
		int state = 0;
		while (state < 5) {
			switch (state) {
			case 0:
				fscanf(imagefile, "%s", comment);
				if (verbose)
					printf("header %s\n", comment);
				if (strcmp("P5", comment) == 0) {
					state = 1;
					fscanf(imagefile, "%s", comment);

				} else
					exit(-1);
				break;
			case 1:
				if(comment[0]=='#') {
					fscanf(imagefile, "%s", comment);
					if (verbose)
								printf("comment : %s\n", comment);
							while (getc(imagefile) != '\n')
								;
					}
					state = 2;
				break;
			case 2:
				sscanf(comment, "%u", width);
				if (verbose)
					printf("width= %u\n", *width);
				state = 3;
				break;
			case 3:
				fscanf(imagefile, "%u", height);
				if (verbose)
					printf("height= %u\n", *height);
				state = 4;
				break;
			case 4:

				fscanf(imagefile, "%u", &depth);
				if (verbose)
					printf("depth= %u\n", depth);
				state = 5;
				break;
			}
		}
		assert(*height > 0);
		assert(*height <= MAX_HEIGHT);
		assert(*width > 0);
		assert(*width <= MAX_WIDTH);

		for (h = 0; h < *height; h++) {
			for (w = 0; w < *width; w++) {
				img[w][h] = getc(imagefile);
			}
		}
		fclose(imagefile);
	} else {
		return -1;
	}
	return 0;
}

int save_pgm(char* name, int width, int height, unsigned char img[MAX_WIDTH][MAX_HEIGHT]) {
	int w, h;
	FILE* imagefile = fopen(name, "w");
	printf("Saving image %s (w=%u x h=%u)\n", name, width, height);
	fflush(stdout);
	if (imagefile != NULL) {
		fprintf(imagefile, "P5\n%u %u\n 255\n", width, height);
		assert(height > 0);
		assert(height <= MAX_HEIGHT);
		assert(width > 0);
		assert(width <= MAX_WIDTH);
		for (h = 0; h < height; h++) {
			for (w = 0; w < width; w++) {
				int ok = putc(img[w][h], imagefile);
				if(ok<0) {
					fprintf(stderr, "Error during image write (at h=%d,w=%d)\n",h,w);
					fclose(imagefile);
					exit(-1);
				}
			}
		}
		fclose(imagefile);
		return 0;
	} else {
		fprintf(stderr, "Could not open file %s in read mode\n", name);
		return -1;
	}
}



