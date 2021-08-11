#include "Adafruit_PixelDust.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#define WIDTH 128
#define HEIGHT 64
#define DEPTH 16
#define N_GRAINS 500
#define MAX_ROUND 300
int _round = 0;

Adafruit_PixelDust sand(WIDTH, HEIGHT, DEPTH, N_GRAINS, 1, 128);
uint8_t pixelBuf[WIDTH * HEIGHT * DEPTH];

int main()
{
    memset(pixelBuf, 0, sizeof(pixelBuf));
    sand.begin();
    for (int z = 6; z < 10; z++)
    {
        for (int y = 30; y < 34; y++)
        {
            for (int x = 60; x < 68; x++)
            {
                sand.setPixel(x, y, z);
                pixelBuf[z * WIDTH * HEIGHT + y * WIDTH + x] = 3;
            }
        }
    }

    sand.randomize();

    FILE *out = fopen("./out.txt", "w");

    dimension_t x, y, z;

    for (int i = 0; i < N_GRAINS; i++)
    {
        sand.getPosition(i, &x, &y, &z);
        // printf("%d\t%d\t%d\n", x, y, z);
        pixelBuf[z * WIDTH * HEIGHT + y * WIDTH + x] = 1;
    }

    for (int i = 0; i < sizeof(pixelBuf); i++)
    {
        fprintf(out, "%d,", pixelBuf[i]);
    }

    fprintf(out, "\n");

    while (_round++ < MAX_ROUND)
    {
        for (int i = 0; i < N_GRAINS; i++)
        {
            sand.getPosition(i, &x, &y, &z);
            pixelBuf[z * WIDTH * HEIGHT + y * WIDTH + x] = 0;
        }

        sand.iterate(1000, 1000, -1000);

        // printf(" ************** round %d **************\n", _round);

        for (int i = 0; i < N_GRAINS; i++)
        {
            sand.getPosition(i, &x, &y, &z);
            // printf("%d\t%d\t%d\n", x, y, z);
            pixelBuf[z * WIDTH * HEIGHT + y * WIDTH + x] = 1;
        }

        for (int i = 0; i < sizeof(pixelBuf); i++)
        {
            fprintf(out, "%d,", pixelBuf[i]);
        }

        fprintf(out, "\n");
    }

    fclose(out);
}