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

#ifndef _RAY_H_
#define _RAY_H_

#include "scene.h"

#define FARPLANE 1<<30

typedef struct Ray
{
    Vector src; /* Ray's origin */
    Vector dir; /* Ray's unit direction */
}
Ray;

typedef struct Hit
{
    Vector  position;   /* Position of the ray hit point */
    Vector  normal;     /* normal at that point */
    Ray     ray;        /* incident ray at the hit point */
    int     objid;      /* Object id of the hit object */
    float   t;          /* Distance from camera postion to nearest hit */
}
Hit;


/* Ray functions */
void setRay(Ray *r, Vector src, Vector dir)
{
    (*r).src = src;
    (*r).dir = dir;
}

Ray generateRay(int i, int j, Camera cam)
{
    float samplei = (2.0f * ((float)i / (float)cam.width ) - 1.0f) * cam.ar;
    float samplej =  2.0f * ((float)j / (float)cam.height) - 1.0f;

    Vector sampleHori, sampleVert, sampleLook;
    Ray out;

    sampleHori = floatVecMult(samplei * cam.fov2, cam.hori);
    sampleVert = floatVecMult(samplej * cam.fov2, cam.vert);
    sampleLook = add(add(sampleHori, sampleVert), cam.look);

    setRay(&out, cam.position, normalize(sampleLook));
    return out;
}

float intersectTriangle(Ray r, Triangle tri)
{
    float u = 0.0f, v = 0.0f, t = 0.0f, a = 0.0f, ia = 0.0f;
    Vector edge1, edge2, h, s, q;

    edge1 = subtract(tri.v1, tri.v0);
    edge2 = subtract(tri.v2, tri.v0);

    h = cross(r.dir, edge2);
    a = dot(edge1, h);
    if(a > -EPSILON && a < EPSILON) /* If a is zero */
        return 0.0f;

    ia = 1.0f / a;
    s = subtract(r.src, tri.v0);
    u = dot(s, h) * ia;
    if(u < 0.0f || u > 1.0f)
        return 0.0f;

    q = cross(s, edge1);
    v = dot(r.dir, q) * ia;
    if(v < 0.0f || u + v > 1.0f)
        return 0.0f;

    t = dot(edge2, q) * ia;

    if(t > EPSILON)
        return t;
    else
        return 0.0f;
}

Hit intersectObject(Ray ray, Object object, int objid)
{
    int i, triId = -1;
    float t, near = FARPLANE;
    Triangle nearTri;
    Vector temp;
    Hit hit;

    hit.t = 0.0;

    for(i = 0; i < object.ntris; i++)
    {
        t = intersectTriangle(ray, object.tri[i]);

        /* Self occlusion check */
        if (t > 0.0f && t < near)
        {
            temp = normalize(cross(subtract(object.tri[i].v1, object.tri[i].v0),
                                   subtract(object.tri[i].v2, object.tri[i].v0)));

            /* only front facing triangles respond */
            if(dot(temp, ray.dir) < EPSILON)
            {
                near = t;
                triId = i;
            }
        }
    }

    /* near and far sanity check for hit */
    if(near > 0.0f && near < FARPLANE)
    {
        nearTri = object.tri[triId];
        hit.position = add(ray.src, floatVecMult(near, ray.dir));

        hit.normal = normalize(cross(subtract(nearTri.v1, nearTri.v0), subtract(nearTri.v2, nearTri.v0)));
        hit.ray = ray;
        hit.objid = objid;
        hit.t = near;
        return hit;
    }

    /* objid = -1 means no hit, other vars may contain garbage values */
    hit.objid = -1;
    return hit;
}

Hit intersectScene(Ray ray, Scene scene)
{
    int i, t=0;
    Hit h;
    Hit nearh;
    nearh.t = FARPLANE;

    for(i = 0; i < scene.nObjects; i++)
    {
        h = intersectObject(ray, scene.obj[i], i);

        /* Occlusion check */
        if(h.t > 0.0f && h.t < nearh.t)
            nearh = h;
    }

    /* near and far sanity check for hit */
    if(nearh.t > 0.0f && nearh.t < FARPLANE)
        return nearh;

    /* objid = -1 means no hit, other vars may contain garbage values */
    nearh.objid = -1;
    return nearh;
}

float traceShadow(Hit hit, Scene scene, Light light)
{
    int i, j;
    float t = 0.0f;
    Ray shadowRay;
    Vector dir;

    dir = normalize(subtract(light.position, hit.position));
    setRay(&shadowRay, hit.position, dir);

    /* Send the shadow ray back to light, if it hits, our original ray is in shadow */
    for(i = 0; i < scene.nObjects; i++)
        for(j = 0; j < scene.obj[i].ntris; j++)
            if(intersectTriangle(shadowRay, scene.obj[i].tri[j]) > EPSILON)
                return light.shadow;

    return 0.0f;
}

Ray reflectRay(Hit hit)
{
    Vector viewDir;
    Ray reflect;

    /* 2 (N . V) N - V */
    viewDir  = negate(hit.ray.dir);
    reflect.dir = subtract(floatVecMult(2.0f * dot(hit.normal, viewDir), hit.normal), viewDir);
    reflect.src = hit.position;

    return reflect;
}

Ray refractRay(Hit hit, float ir)
{
    Ray refract;
    Vector incident = negate(hit.ray.dir);
    float c = dot(incident, hit.normal);
    float i = 1.0f / ir;
    float s = i * c - sqrtf(1.0f - i * i * (1.0f - c * c));

    refract.dir = normalize(subtract(floatVecMult(s, hit.normal), floatVecMult(i, incident)));

    /*float c = dot(hit.ray.dir, hit.normal);
    float s = ir * ir * (1.0f - (c * c));

    refract.dir = subtract(floatVecMult(ir, hit.ray.dir), floatVecMult((ir + sqrtf(1.0f - s)), hit.normal));*/
    refract.src = hit.position;

    return refract;
}

#endif
