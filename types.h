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

#ifndef _MYTYPES_H_
#define _MYTYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00001

/* Data Types */
typedef struct Vector
{
    float x;
    float y;
    float z;
}
Vector;

typedef struct Triangle
{
    Vector v0;
    Vector v1;
    Vector v2;
}
Triangle;

typedef struct Matrix
{
    float m[4][4];
}
Matrix;


/* Data Type Setters */
void setVector(Vector *v, float x, float y, float z)
{
    (*v).x = x;
    (*v).y = y;
    (*v).z = z;
}

void setTriangle(Triangle *tri, Vector v0, Vector v1, Vector v2)
{
    (*tri).v0 = v0;
    (*tri).v1 = v1;
    (*tri).v2 = v2;
}

void setMatrix(Matrix *mat, float *m)
{
    /* Column major storage */
    int i, j;
    for(j = 0; j < 4; j++)
        for(i = 0; i < 4; i++)
            (*mat).m[i][j] = m[j + 4 * i];
}


/* Data Type Printers */
void printVector(Vector v)
{
    printf("(%.4f, %.4f, %.4f)", v.x, v.y, v.z);
}

void printTriangle(Triangle tri)
{
    printf("0:");
    printVector(tri.v0);
    printf(" 1:");
    printVector(tri.v1);
    printf(" 2:");
    printVector(tri.v2);
}

void printMatrix(Matrix M)
{
    /* column major printing */
    int i, j;
    for(j = 0; j < 4; j++)
        printf("%.4f\t%.4f\t%.4f\t%.4f\n", M.m[j][0], M.m[j][1], M.m[j][2], M.m[j][3]);
}


/* utility functions */
float deg2rad(float deg)
{
    return M_PI * deg / 180.0f;
}

Vector vecVecMult(Vector a, Vector b)
{
    Vector c;
    setVector(&c, a.x * b.x, a.y * b.y, a.z * b.z);
    return c;
}

Vector floatVecMult(float a, Vector b)
{
    Vector c;
    setVector(&c, a * b.x, a * b.y, a * b.z);
    return c;
}

float dot(Vector a, Vector b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector cross(Vector a, Vector b)
{
    Vector c;
    setVector(&c, (a.y * b.z) - (b.y * a.z),
                  (a.z * b.x) - (b.z * a.x),
                  (a.x * b.y) - (b.x * a.y));
    return c;
}

Vector add(Vector a, Vector b)
{
    Vector c;
    setVector(&c, a.x + b.x, a.y + b.y, a.z + b.z);
    return c;
}

Vector subtract(Vector a, Vector b)
{
    Vector c;
    setVector(&c, a.x - b.x, a.y - b.y, a.z - b.z);
    return c;
}

Vector negate(Vector a)
{
    Vector c;
    setVector(&c, -a.x, -a.y, -a.z);
    return c;
}

float length(Vector a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector normalize(Vector a)
{
    float l = length(a);
    Vector c;

    if(l > -EPSILON && l < EPSILON) /* If length is zero */
        return a;

    setVector(&c, a.x/l, a.y/l, a.z/l);
    return c;
}

Vector matVecMult(Matrix M, Vector v)
{
    /* Column Major (Post multiplication of Column vectors) */
    Vector c;
    int i;
    setVector(&c, M.m[0][0] * v.x + M.m[0][1] * v.y + M.m[0][2] * v.z + M.m[0][3],
                  M.m[1][0] * v.x + M.m[1][1] * v.y + M.m[1][2] * v.z + M.m[1][3],
                  M.m[2][0] * v.x + M.m[2][1] * v.y + M.m[2][2] * v.z + M.m[2][3]);
                  /* Last row multiplication with the Vector is discarded intentionally*/
    return c;
}

Matrix matMatMult(Matrix A, Matrix B)
{
    Matrix C;
    int i, j, k;
    for(j = 0; j < 4; j++)
        for(i = 0; i < 4; i++)
        {
            C.m[i][j] = 0.0f;
            for(k = 0; k < 4; k++)
                C.m[i][j] += A.m[i][k] * B.m[k][j];
        }
    return C;
}

Matrix identity(void)
{
    Matrix C;
    float m[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};
    setMatrix(&C, m);
    return C;
}

Matrix rotateX(float a)
{
    float cosa = cos(deg2rad(a)), sina = sin(deg2rad(a));
    Matrix C;
    float m[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, cosa,-sina, 0.0f,
                   0.0f, sina, cosa, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};
    setMatrix(&C, m);
    return C;
}

Matrix rotateY(float a)
{
    float cosa = cos(deg2rad(a)), sina = sin(deg2rad(a));
    Matrix C;
    float m[16] = {cosa, 0.0f, sina, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                  -sina, 0.0f, cosa, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};
    setMatrix(&C, m);
    return C;
}

Matrix rotateZ(float a)
{
    float cosa = cos(deg2rad(a)), sina = sin(deg2rad(a));
    Matrix C;
    float m[16] = {cosa,-sina, 0.0f, 0.0f,
                   sina, cosa, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};
    setMatrix(&C, m);
    return C;
}

Matrix rotate(float ax, float ay, float az)
{
    /* Simply doing rotX x rotY x rotZ */
    return matMatMult(matMatMult(rotateX(ax), rotateY(ay)), rotateZ(az));
}

Matrix translate(float tx, float ty, float tz)
{
    Matrix C;
    float m[16] = {1.0f, 0.0f, 0.0f, tx,
                   0.0f, 1.0f, 0.0f, ty,
                   0.0f, 0.0f, 1.0f, tz,
                   0.0f, 0.0f, 0.0f, 1.0f};
    setMatrix(&C, m);
    return C;
}

Matrix scale(float sx, float sy, float sz)
{
    Matrix C;
    float m[16] = {sx,   0.0f, 0.0f, 0.0f,
                   0.0f,   sy, 0.0f, 0.0f,
                   0.0f, 0.0f,   sz, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};
    setMatrix(&C, m);
    return C;
}

#endif
