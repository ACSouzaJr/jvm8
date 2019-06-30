#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "stack_operands.h"

void push_operand(LocalVariable * operand, StackOperand *stack_operand){
    Operand *ptr = (Operand *)malloc(sizeof(Operand));
    if (ptr == NULL)
    {
        // printf("not able to push the element");
    }
    else
    {
    
        if (stack_operand == NULL)
        {
        }
        else
        {
            ptr->f = operand;
            ptr->pointer = (stack_operand->top);
            stack_operand->top = ptr;
        }
    }

    printf("Acabou de ser empilhado: %04x\n", stack_operand->top->f->value);
    // return *stack_operand;
} 

LocalVariable* pop_operand(StackOperand *stack_operand){
    LocalVariable *aux;
    Operand *ptr;
    if (stack_operand == NULL)
    {
        // printf("not able to pop the element. No elements in stack\n");
    }
    else
    {
        ptr = stack_operand->top;
        aux = stack_operand->top->f;
        stack_operand->top = ptr->pointer;
        free(ptr);
        // printf("Item popped (cafebabe)\n");
    }
    return aux;
}

Operand* top_operand(StackOperand *stack_operand){
    return stack_operand->top;
}