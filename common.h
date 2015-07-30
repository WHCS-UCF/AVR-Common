#ifndef COMMON_H
#define COMMON_H

#define NULL 0
typedef unsigned long time_t;
typedef unsigned int size_t;

// helper macros
#define min(x, y) (((x) > (y)) ? (y) : (x))
#define max(x, y) (((x) < (y)) ? (y) : (x))
#ifndef abs
#define abs(x) (((x) < 0) ? -(x) : (x))
#endif

static inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // COMMON_H
