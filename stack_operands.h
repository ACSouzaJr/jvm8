#ifndef STACK_OPERAND_H
#define STACK_OPERAND_H
#include "structures.h"

typedef struct StackOperand
{
    LocalVariable *f; //Frame * f;
    struct StackOperand *pointer;
} Operand;

typedef struct
{
   struct StackOperand *top;
} StackOperand;

LocalVariable *pop_operand(StackOperand *stack_operand);
void push_operand(LocalVariable *operand, StackOperand *operands);
LocalVariable* top_operand(StackOperand *stack_operand);
void init();
void print_stack();

#endif