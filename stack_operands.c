#include "structures.h"
#include "stack_operands.h"

StackOperand* push_operand(LocalVariable * operand, StackOperand *operands){
    printf("Ronaldo Prévio %lu", operands->f->value);
    StackOperand* aux = operands;
    operands->f = operand;
    operands->pointer = aux;

    printf("Ronaldo %lu", operands->f->value);
    return operands;
} 