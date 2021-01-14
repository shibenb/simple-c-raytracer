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

#ifndef _SHADE_H_
#define _SHADE_H_

#include "ray.h"

Vector ambient(Hit hit, Scene scene, Light light)
{
    int i = hit.objid;

    return floatVecMult(scene.obj[i].material.kambient, vecVecMult(scene.obj[i].material.color, light.color));
}

Vector diffuse(Hit hit, Scene scene, Light light)
{
    int i = hit.objid;

    Vector lightDir = normalize(subtract(light.position, hit.position));
    float t = dot(hit.normal, lightDir) * scene.obj[i].material.kdiffuse;

    return floatVecMult(t, vecVecMult(scene.obj[i].material.color, light.color));
}

Vector specular(Hit hit, Scene scene, Light light)
{
    int i = hit.objid;

    Ray reflect  = reflectRay(hit);
    Vector lightDir = normalize(subtract(light.position, hit.position));
    float t = pow(dot(lightDir, reflect.dir), scene.obj[i].material.shininess) * scene.obj[i].material.kspecular;

    return floatVecMult(t, vecVecMult(scene.obj[i].material.color, light.color));
}

#endif
