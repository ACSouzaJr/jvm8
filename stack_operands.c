#include <stdlib.h>
#include "structures.h"
#include "stack_operands.h"

StackOperand* push_operand(LocalVariable *operand, StackOperand **stack_operand){
    StackOperand *ptr = (StackOperand *)malloc(sizeof(StackOperand));
    if (ptr == NULL)
    {
        // printf("not able to push the element");
    }
    else
    {
    
        if (stack_operand == NULL)
        {
            ptr->operand = operand;
            ptr->pointer = NULL;
            *stack_operand = ptr;
        }
        else
        {
            ptr->operand = operand;
            ptr->pointer = stack_operand;
            *stack_operand = ptr;
        }
        // printf("Item pushed");
    }

    printf("Ronaldo %02x", (*stack_operand)->operand->value);
    return stack_operand;
} 

StackOperand* top_operand(StackOperand *stack_operand){
    return stack_operand;
}