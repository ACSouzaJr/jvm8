/**
* @file         stack_operands.c
* @brief        Funções de gerenciamento da pilha de operandos.
* @detail       Contém as implementações das funções de gerenciamento para a pilha de operandos da JVM.
*/

#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "stack_operands.h"

/**
 * @brief Adiciona um novo operando dentro da pilha de operandos do Frame.
 */
void push_operand(LocalVariable *operand, StackOperand *stack_operand)
{
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
#ifdef DEBUG
    if (operand != NULL)
    {

        if (stack_operand->top->f->type == CONSTANT_Double)
        {
            printf("Acabou de ser empilhado doublemente: %f\n", *(double *)&(stack_operand->top->f->type_double));
        }
        else if (stack_operand->top->f->type == CONSTANT_Float)
        {
            printf("Acabou de ser empilhado floatmente: %f\n", *(float *)&stack_operand->top->f->value);
        }
        else if (stack_operand->top->f->type == CONSTANT_Long)
        {
            printf("Acabou de ser empilhado longmente: %ld\n", *(long *)&stack_operand->top->f->type_long);
        }
        else
        {
            printf("Acabou de ser empilhado: %04x\n", stack_operand->top->f->value);
        }
#endif
    }

    // return *stack_operand;
}

/**
 * @brief Retira o topo da pilha de operandos e retorna para o chamador.
 */
LocalVariable *pop_operand(StackOperand *stack_operand)
{
    LocalVariable *aux;
    Operand *ptr;
    // Se o ponteiro for nulo faz nada
    if (stack_operand == NULL)
    {
        aux = NULL;
    }
    else
    {
        // Se a pilha estiver vazia
        if (stack_operand->top == NULL)
        {
            // printf("not able to pop the element. No elements in stack\n");
            // return stack_operand->top;
            return NULL;
        }
        ptr = stack_operand->top;
        aux = stack_operand->top->f;
        stack_operand->top = ptr->pointer;
        // free(ptr);
        // printf("Item popped (cafebabe)\n");
    }
    return aux;
}

/**
 * @brief Retorna o topo da pilha de operandos.
 */
LocalVariable *top_operand(StackOperand *stack_operand)
{
    return stack_operand->top->f;
}