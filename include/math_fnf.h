#ifndef MATH_FNF_H
#define MATH_FNF_H
#define FNF_MAX(a, b) ((a) > (b) ? (a) : (b))
#define FNF_MIN(a, b) ((a) < (b) ? (a) : (b))

float lerp(float a, float b, float ratio);
float remap(float value, float start1, float stop1, float start2, float stop2);
float cubeInOut(float t);

#endif // MATH_FNF_H