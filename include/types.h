#ifndef TYPES_H
#define TYPES_H
#include "config.h"
#include <string.h>
#include <malloc.h>

#ifdef _DEFINE_TYPES
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

#define bool char
#define true 1
#define false 0
#else

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#endif

#define FNF_NEW(x) (x*)malloc(sizeof(x))
#define FNF_DELETE(x) free(x)
#define FNF_ZERO(x, y)  memset(x, 0, y);

typedef struct {
    float x,y;
} fnf_vector;

typedef struct {
    float x,y,w,h;
} fnf_rect;

typedef enum {
    NONE,
    X,
    Y,
    XY
} fnf_axis_t;

#ifdef __clang__
//#define NULL 0
#endif

#endif // TYPES_H