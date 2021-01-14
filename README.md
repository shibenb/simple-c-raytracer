# simple-c-raytracer
Build a Raytracer - Workshop by DDU India at BITS-ATMOS Hyderabad 2012

This is a simplest possible raytracer written in C, with minimal usage of
pointers and external libraries, for easy understanding of the basic theory.

Compile and Build as
# make

Run the executable
# ./raytracer.out

Some points for performance:
1. Image Size: for quick testing, use something like 320x240
2. Recursion Depth: Can be kept 0 for no reflections/refractions
3. Cone and Cylinder can use smaller resolution, default image uses 32
4. Sphere resolution, unlike Cone and Cylinder, is power of 4

Thank you for downloading our code.

Authors:
Shiben Bhattacharjee
Naveen Kumar

Special mention of our dear friend:
Yash Agrawal
