#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "classfile.h"
#include "frame.h"

Frame* cria_frame(cp_info *cp, method_info *mi){
  Frame * frame = (Frame *) malloc(sizeof(Frame));
  frame->pc = 0;
  frame->cp = cp;
  frame->method = mi;
  frame->bytecode = mi->attributes->info->Code_attribute.code;
  // aloca stack de operandos, acho que era para ser um vetor ?
  frame->operands = (StackOperand *) malloc(sizeof(StackOperand));
  if (!frame->operands)
  {
    frame->operands = NULL;
  }
  // aloca espaco para variaveis locais
  for (attribute_info *attr = mi->attributes; attr < mi->attributes + mi->attributes_count; attr++)
  {
    char *attribute_name = readUtf8(cp, attr->attribute_name_index);
    if (strcmp(attribute_name, "Code") == 0)
    {
      frame->local_variables = (LocalVariable *) malloc(sizeof(LocalVariable)* attr->info->Code_attribute.max_locals);
      break;
    }
  }
  return frame;
}