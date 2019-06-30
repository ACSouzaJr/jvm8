#include "structures.h"
#include "frame.h"
#include <stdlib.h>
#include <stdio.h>

Frame* cria_frame(ClassFile *cf, method_info *mi){
  Frame * frame = (Frame *) malloc(sizeof(Frame));
  frame->pc = 0;
  frame->cp = cf->constant_pool;
  frame->method = mi;
  // aloca stack de operandos, acho que era para ser um vetor ?
  frame->operands = (StackOperand *) malloc(sizeof(StackOperand));
  if (!frame->operands)
  {
    frame->operands = NULL;
  }
  // aloca espaco para variaveis locais
  for (attribute_info *attr = mi->attributes; attr < mi->attributes + mi->attributes_count; attr++)
  {
    char *attribute_name = readUtf8(cf->constant_pool, attr->attribute_name_index);
    if (strcmp(attribute_name, "Code") == 0)
    {
      frame->local_variables = (LocalVariable *) malloc(sizeof(LocalVariable)* attr->info->Code_attribute.max_locals);
      break;
    }
  }
  return frame;
}