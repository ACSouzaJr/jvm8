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

void print_stack_debug(Frame *f)
{
  Operand * stack_top, * next;
  stack_top = f->operands->top;
  next = f->operands->top->pointer;
  
  printf("==================\n");
  printf("DEBUG: INFORMAÇÕES DO STACK DE OPERANDOS\n");
  printf("STACK TOP TYPE == %d\n", f->operands->top->f->type);

  while(next->f != NULL) {
    printf("STACK: %d\n", next->f->type);

    if(next->f->type == 9) {
        printf("   array ref: %x\n", next->f);
    }

    next = next->pointer;
  }
  printf("==================\n");
  printf("\n");
}