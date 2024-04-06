#include "math_fnf.h"

float lerp(float a, float b, float ratio) {
    return a + ratio * (b - a);
}

float remap(float value, float start1, float stop1, float start2, float stop2){
    return start2 + (value - start1) * ((stop2 - start2) / (stop1 - start1));
}