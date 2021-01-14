/*
   Copyright 2012 Shiben Bhattacharjee & Naveen Kumar

   licensed under the apache license, version 2.0 (the "license");
   you may not use this file except in compliance with the license.
   you may obtain a copy of the license at

       http://www.apache.org/licenses/license-2.0

   unless required by applicable law or agreed to in writing, software
   distributed under the license is distributed on an "as is" basis,
   without warranties or conditions of any kind, either express or implied.
   see the license for the specific language governing permissions and
   limitations under the license.
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
