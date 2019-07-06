#include "helpers.h"
#define exponent(x) ((x << 1) >> 24)
#define mantiss(x) ((x << 9) >> 9)
#define signal(x) (x >> 31)

u2 getIndexFromb1b2(Frame *f){
    u1 index1byte, index2byte;
    u1 *bytecode = f->method->attributes->info->Code_attribute.code;
    index1byte = bytecode[f->pc++];
    index2byte = bytecode[f->pc++];

    // recupera Utf8 da referencia
    u2 index = ((index1byte << 8) | index2byte);

    return index;
}

u4 convertFloatToBytes(float *res){
    u4 sum, r1;
    sum = (u4)(*(u4*)res);
	r1 = (signal(sum)<<31) | (exponent(sum)<<23) | mantiss(sum);

    return r1;
}