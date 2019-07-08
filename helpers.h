#ifndef HELPERS_H
#define HELPERS_H
#include <stdlib.h>
#include "structures.h"
#include "frame.h"

u2 getIndexFromb1b2(Frame *f);
u4 convertFloatToBytes(float *res);
void print_stack_debug(Frame *f);
#endif