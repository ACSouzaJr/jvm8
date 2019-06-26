#ifndef FRAME_H
#define FRAME_H
#include "structures.h"
#include "stack_operands.h"


typedef struct 
{
  u4 pc;
  method_info *method;
  cp_info *cp;  
  LocalVariable local_variables[300]; //[]
  StackOperand *operands;
}Frame;


#endif

// struct Frame {
//   std::vector<Operand*> local_variables_array;
//   std::stack<Operand*> operand_stack;
// };

// typedef struct frame_s{
// 	PilhaOperandos *operandos;
// 	LocalVariables *locals;
// } frame;