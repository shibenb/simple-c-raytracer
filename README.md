# simple-c-raytracer
This is a simplest possible raytracer written in C, intentionally brute force, with minimal usage of pointers and external libraries, for easy understanding of the basic theory. This was part of a workshop, [Build a Raytracer](https://shiben.tech/blog/simple-easy-ray-tracer/), by DDU India at BITS-ATMOS Hyderabad 2012.  

# Build and Run
make  
./raytracer.out

# Scene Description
Scene can be set up in `createScene()` function in `raytracer.c`.  
The function names are self explanatory.  

# Output Settings
Use the following `#define`s in `raytracer.c` for output settings:  
> #define FILENAME "output.ppm"  
> #define ISBINARY 1  
> #define WIDTH 160  
> #define HEIGHT 120  
> #define FOV 45.0f  
> #define RECURSION 2  
> #define CONE_DIVISIONS 32  
> #define CYLINDER_DIVISIONS 32  
> #define SPHERE_SUBDIVISIONS 3  

# Performance
1. Use a small image size (e.g. 160x120) for quicker results
2. Recursion Depth can reduced. 0 means no reflections and refractions
3. Use smaller number for primitive divisions (resolution)

# Authors
Shiben Bhattacharjee  
Naveen Kumar  
*Special mention of our dear friend for moral support: Yash Agrawal*
