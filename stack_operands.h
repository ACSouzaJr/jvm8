/**
* @file         stack_operands.h
* @brief        Implementação das structs de pilha de operandos.
* @detail       Contém as implementações das estruturas de pilha que serão usadas pela JVM.
*/

#ifndef STACK_OPERAND_H
#define STACK_OPERAND_H
#include "structures.h"

/**
 * @brief Estrutura que representa um operando da pilha.
 *
 * Possui um ponteiro para uma LocalVariable e um pointeiro para uma pilha de operandos.
 */
typedef struct StackOperand
{
    LocalVariable *f; //Frame * f;
    struct StackOperand *pointer;
} Operand;

/**
 * @brief Struct que implementa a pilha de operandos.
 */
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