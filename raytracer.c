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

#include "types.h"
#include "scene.h"
#include   "ray.h"
#include "shade.h"
#include "image.h"

void createScene(Scene *scene)
{
    Object cone, cylinder, cube, sphere, plane_base, plane_right, plane_left, plane_back;
    Material mtl_matt1, mtl_matt2, mtl_matt3, mtl_matt4, mtl_glass, mtl_shiny1, mtl_shiny2;
    Vector red, blue, green, purple, white;

    /* lets collect a few colors */
    setVector(&red,    1.0f, 0.0f, 0.0f);
    setVector(&green,  0.0f, 1.0f, 0.0f);
    setVector(&blue,   0.0f, 0.0f, 1.0f);
    setVector(&purple, 1.0f, 0.0f, 1.0f);
    setVector(&white,  1.0f, 1.0f, 1.0f);

    /* Create some materials */
    /* setMaterial(&mtl, color, kambient, kdiffuse, kspecular, shininess, refl, refr, ir) */
    setMaterial(&mtl_shiny1, white,  0.1f, 0.5f, 0.4f, 2.0f,  0.4f, 0.2f, 1.4f);
    setMaterial(&mtl_shiny2, blue,   0.1f, 0.5f, 0.4f, 3.0f,  0.2f, 0.0f, 1.4f);
    setMaterial(&mtl_matt1,  red,    0.1f, 0.5f, 0.4f, 32.0f, 0.0f, 0.0f, 1.4f);
    setMaterial(&mtl_matt2,  blue,   0.1f, 0.5f, 0.4f, 2.0f,  0.0f, 0.0f, 1.4f);
    setMaterial(&mtl_matt3,  green,  0.1f, 0.5f, 0.4f, 2.0f,  0.0f, 0.0f, 1.4f);
    setMaterial(&mtl_matt4,  purple, 0.1f, 0.5f, 0.4f, 2.0f,  0.0f, 0.0f, 1.4f);
    setMaterial(&mtl_glass,  white,  0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 0.9f, 1.4f);

    /* create some objects, attach created materials to them */
    createCone(     &cone,          mtl_matt4,  1.0f, 1.5f, 32);
    createSphere(   &sphere,        mtl_matt1,  0.9f, 3);
    createCylinder( &cylinder,      mtl_shiny2, 0.75f, 1.0f, 32);
    createCube(     &cube,          mtl_glass,  1.0f);
    createPlaneXZ(  &plane_base,    mtl_shiny1, 10.0f);
    createPlaneXZ(  &plane_left,    mtl_matt1,  10.0f);
    createPlaneXZ(  &plane_right,   mtl_matt2,  10.0f);
    createPlaneXZ(  &plane_back,    mtl_matt3,  10.0f);

    /* arrange them in the scene */
    transformObject(&cone,     translate(2.0f, 0.0f, -2.8f));
    transformObject(&cube,     matMatMult(translate( 2.0f, 0.5f, -1.0f), rotateY(45.0f)));
    transformObject(&cylinder, translate(-0.0f, 0.0f, -1.0f));
    transformObject(&sphere,   translate(-0.0f, 0.9f, -2.7f));
    transformObject(&plane_base,  translate( 1.0f, 0.0f, -4.0f));
    transformObject(&plane_left,  matMatMult(translate( -2.0f, 0.0f, -4.0f), rotateZ(-90.0)));
    transformObject(&plane_right, matMatMult(translate(  4.0f, 0.0f, -4.0f), rotateZ(90.0)));
    transformObject(&plane_back,  matMatMult(translate(  1.0f, 0.0f, -6.0f), rotateX(90.0)));

    /* add them to the scene */
    initScene(scene, 8);
    addObjectToScene(scene, cone);
    addObjectToScene(scene, cube);
    addObjectToScene(scene, cylinder);
    addObjectToScene(scene, sphere);
    addObjectToScene(scene, plane_base);
    addObjectToScene(scene, plane_right);
    addObjectToScene(scene, plane_left);
    addObjectToScene(scene, plane_back);
}

Vector trace(Ray ray, Scene scene, Light light, int recur)
{
    Hit hit;
    Vector output, reflColor, refrColor;
    float krefl, krefr;

    /* default return color */
    setVector(&output, 0.0f, 0.0f, 0.0f);

    hit = intersectScene(ray, scene);

    if(hit.objid >= 0)
    {
        /* current object color */
        output = add(ambient(hit, scene, light), add(diffuse(hit, scene, light), specular(hit, scene, light)));

        if(recur > 0)
        {
            /* collect color captured by reflected ray */
            reflColor = trace(reflectRay(hit), scene, light, recur - 1);
            krefl = scene.obj[hit.objid].material.reflectivity;
            output = add(output, floatVecMult(krefl, reflColor));
            
            /* collect color captured by refracted ray */
            refrColor = trace(refractRay(hit, scene.obj[hit.objid].material.ir), scene, light, recur - 1);
            krefr = scene.obj[hit.objid].material.translucency;
            output = add(output, floatVecMult(krefr, refrColor)); 
        }

        /* reduce color by the shadow factor of the light */
        return floatVecMult(1.0f - traceShadow(hit, scene, light), output);
    }

    return output;
}

int main(int argc, char **argv)
{
    int width = 640, height = 480, recur = 2, i, j;
    char *filename = "output.ppm";

    Vector lcolor, lpos, camPos, lookat;
    Light light;
    Camera cam;
    Hit hit;
    Ray ray;
    Color outcolor;
    Image image;
    Scene scene;

    /* setup scene */
    createScene(&scene);

    /* setup light */
    setVector(&lcolor, 1.0f, 1.0f, 1.0f);
    setVector(&lpos,  -1.0f, 4.0f, 4.0f);
    setLight(&light, lpos, lcolor, 0.3f);

    /* setup camera */
    setVector(&camPos, 1.0f, 2.0f,  4.0f);
    setVector(&lookat, 1.0f, 0.0f, -6.0f);
    setCamera(&cam, camPos, lookat, 45.0f, width, height);

    /* setup image */
    initImage(&image, width, height);

    printf("Image: %s, Size: %dx%d\n", filename, width, height);
    printf("Initial Ray Count: %d, Triangle Count: %d, Recursion Depth: %d\n", width * height, getTriangleCount(scene), recur);
    printf("Raytracing...\n");

    /* tracing */
    for(j = 0; j < height; j++)
        for(i = 0; i < width; i++)
        {
            ray = generateRay(i, j, cam);
            outcolor = vector2color(trace(ray, scene, light, recur));
            setPixel(&image, i, j, outcolor);
        }

    printf("Done, writing image...\n");
    writeImage(image, filename);
    cleanScene(&scene);
    printf("Done\n");
    return 0;
}
