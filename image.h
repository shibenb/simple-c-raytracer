/*
Copyright (c) 2012, Shiben Bhattacharjee, Naveen Kumar

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define RGB 3
#define COMMENT "Created by 'Build a Raytracer - Workshop by DDU at BITS-ATMOS Hyd 2012'"

typedef struct Image
{
    int width;
    int height;
    unsigned char *data;
}
Image;

/* For all color calculations, Vector must be used */
/* For final writing out in the image, Color should be used */
typedef struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}
Color;

Color vector2color(Vector incolor)
{
    Color out;
    out.r = (unsigned char)(fmin(1.0f, fmax(0.0f, incolor.x)) * UCHAR_MAX);
    out.g = (unsigned char)(fmin(1.0f, fmax(0.0f, incolor.y)) * UCHAR_MAX);
    out.b = (unsigned char)(fmin(1.0f, fmax(0.0f, incolor.z)) * UCHAR_MAX);
    return out;
}

/* initializes the image and sets all pixels to black */
void initImage(Image *img, int width, int height)
{
    int pixels = 0;

    (*img).width = width;
    (*img).height = height;
    pixels = (*img).width * (*img).height * RGB;

    (*img).data = (unsigned char *)malloc(sizeof(unsigned char) * pixels);
    memset((*img).data, 0, sizeof(unsigned char) * pixels);
}

/* sets a given pixel color at the pixel location */
void setPixel(Image *img, int i, int j, Color color)
{
    int h = (*img).height, w = (*img).width;
    int index = ((h - j) * w + i) * RGB;

	(*img).data[index + 0] = color.r;
	(*img).data[index + 1] = color.g;
	(*img).data[index + 2] = color.b;
}

/* Write image in ascii format, more human readable */
void writeImage(Image img, const char * filename)
{
    int i = 0, j = 0, index = 0;
	FILE *fp;
	fp = fopen(filename, "w");

	fprintf(fp, "P3\n#%s\n%d %d\n%d\n", COMMENT, img.width, img.height, UCHAR_MAX);

    for(j = 0; j < img.height; j++)
    {
        for(i = 0; i < img.width; i++)
        {
            index = (j * img.width + i) * RGB;
            fprintf(fp, "%3d %3d %3d  ", img.data[index + 0], img.data[index + 1], img.data[index + 2]);
        }
        fprintf(fp, "\n");
    }
	fclose(fp);
}

/* write image in binary format, non human readable but saves diskspace */
void writeImageBinary(Image img, const char * filename)
{
	FILE *fp;
	fp = fopen(filename, "w");
	fprintf(fp, "P6\n#%s\n%d %d\n%d\n", COMMENT, img.width, img.height, UCHAR_MAX);
	fwrite(img.data, sizeof(unsigned char), RGB * img.width * img.height, fp);
	fclose(fp);
}

#endif
