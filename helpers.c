/**
* @file         helpers.c
* @brief        Helpers
* @detail       Funções de apoio para a execução do interpretador da JVM.
*/
#include "helpers.h"
#define exponent(x) ((x << 1) >> 24)
#define mantiss(x) ((x << 9) >> 9)
#define signal(x) (x >> 31)

#define expoent_d(x) ((x << 1) >> 53)
#define mantiss_d(x) ((x << 12) >> 12)
#define signal_d(x) (x >> 63)

/**
 * @brief Retorna um indice a partir de dois bytes
 */
u2 getIndexFromb1b2(Frame *f){
    u1 index1byte, index2byte;
    u1 *bytecode = f->method->attributes->info->Code_attribute.code;
    index1byte = bytecode[f->pc++];
    index2byte = bytecode[f->pc++];

    // recupera Utf8 da referencia
    u2 index = ((index1byte << 8) | index2byte);

    return index;
}

/**
 * @brief Converte um valor em float para um valor em bytes
 */
u4 convertFloatToBytes(float *res){
    u4 sum, r1;
    sum = (u4)(*(u4*)res);
	r1 = (signal(sum)<<31) | (exponent(sum)<<23) | mantiss(sum);

    return r1;
}

/**
 * @brief Converte um valor do tipo double para bytes
 */
uint64_t convertDoubleToBytes(double *res){
    uint64_t r1, sum = (uint64_t)(*(uint64_t*)res);
    r1 = (signal_d(sum)<<63) | (expoent_d(sum)<<52) | mantiss_d(sum);

    return r1;
}