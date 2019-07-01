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

enum
{
    t_boolean = 1,
    t_byte = 2,
    t_char = 3,
    t_short = 4,
    t_int = 5,
    t_float = 6,
    t_reference = 7,
    t_returnAddress = 8,
    t_long = 9,
    t_double = 10
}operand_type;

#endif