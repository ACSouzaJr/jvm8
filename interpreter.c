#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structures.h"
#include "frame.h"
#include "stack_operands.h"
#include "interpreter.h"
#include "stack_frame.h"
#include "classfile.h"

// ClassFile *resolveClass(char *class_name)
// {
//   // classesCarregadas *c = BuscarElemento_classes(jvm->classes,class_name);
//   // ClassFile *classe = NULL;

//   // if(c!=NULL){
//   // 	return c->arquivoClass;
//   // }
//   // else{
//   // 	char *nomearquivo = malloc((strlen(class_name)+7)*sizeof(char));
//   // 	strcpy(nomearquivo,class_name);
//   // 	strcat(nomearquivo,".class");
//   // 	classe = lerArquivo(nomearquivo);
//   // 	jvm->classes = InserirFim_classes(jvm->classes,classe);
//   // }

//   // return(classe);
// }

// int resolveMethod(cp_info *cp, u2 indice_cp, u1 interface)
// {

//   // cp_info *methodref = cp-1+indice_cp;
//   // char *class_name = NULL;
//   // if(!interface){
//   // 	class_name = decodificaNIeNT(cp,methodref->UnionCP.Methodref.class_index,NAME_INDEX);;
//   // }
//   // else{
//   // 	class_name = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.class_index,NAME_INDEX);
//   // }

//   // if(resolveClass(class_name)!=NULL){
//   // 	return 1;
//   // }
//   // else{
//   // 	return 0;
//   // }
//   return 0;
// }

void aconst_null_eval(Frame *f)
{
  push_operand(NULL, f->operands);
}

void iconst_m1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = -1;
  push_operand(lv, f->operands);
}

void iconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 0;
  push_operand(lv, f->operands);
  printf("Iconst_0");
}

void iconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 1;
  push_operand(lv, f->operands);
  printf("Iconst_1");
}

void iconst_2_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 2;
  push_operand(lv, f->operands);
}

void iconst_3_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 3;
  push_operand(lv, f->operands);
}

void iconst_4_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 4;
  push_operand(lv, f->operands);
}

void iconst_5_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 5;
  push_operand(lv, f->operands);
}

void lconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable*) malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  lv->type_long = 0;
  push_operand(lv, f->operands);
}

void lconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable*) malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  lv->type_long = 1;
  push_operand(lv, f->operands);
}

void fconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable*) malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  lv->value = 0.0;
  push_operand(lv, f->operands);
}

void fconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable*) malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  lv->value = 1.0;
  push_operand(lv, f->operands);
}

void fconst_2_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable*) malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  lv->value = 2.0;
  push_operand(lv, f->operands);
}

void dconst_0_eval(Frame *f)
{
  //   push_operand();
}

void dconst_1_eval(Frame *f)
{
  //   push_operand();
}

void bipush_eval(Frame *f)
{
  // Pega o byte de argument extend para int e empilha nos operandos.
  LocalVariable *lv = (LocalVariable*) malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = (int32_t)f->bytecode[f->pc++];
  printf("bipush: %d \n", lv->value);
  push_operand(lv, f->operands);
}

void sipush_eval(Frame *f)
{
  u1 byte1, byte2;
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  u1 *bytecode = f->method->attributes->info->Code_attribute.code;
  byte1 = bytecode[f->pc++];
  byte2 = bytecode[f->pc++];

  u2 val_sipush = ((byte1 << 8) | byte2);
  lv->value = (int32_t)val_sipush;
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

void ldc_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  cp_info *item = &(f->cp[index - 1]);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  printf("tag: %01x\n", item->tag);
  switch (item->tag)
  {
  case CONSTANT_String:
    lv->type = CONSTANT_String;
    lv->value = item->String.string_index;
    break;
  case CONSTANT_Float:
    lv->type = CONSTANT_Float;
    lv->value = item->Float.bytes;
    break;
  case CONSTANT_Integer:
    lv->type = CONSTANT_Integer;
    lv->value = item->Integer.bytes;
    break;
  case CONSTANT_Class:
    lv->type = CONSTANT_Class;
    lv->value = item->Class.name_index;
    break;
  default:
    break;
  }
  push_operand(lv, f->operands);
}

void ldc_w_eval(Frame *f)
{
  //   push_operand();
}

void ldc2_w_eval(Frame *f)
{
  //   push_operand();
}

void iload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]),f->operands);
  printf("iload %d", f->local_variables[index].value);
}

void lload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]),f->operands);
}

void fload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  if(f->local_variables[index].type == CONSTANT_Float){
    push_operand(&(f->local_variables[index]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_eval(Frame *f)
{
  //   push_operand();
}

void aload_eval(Frame *f)
{
  //   push_operand();
}

void iload_0_eval(Frame *f)
{
  push_operand(&(f->local_variables[0]), f->operands);
}

void iload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]), f->operands);
  printf("iload_1");
}

void iload_2_eval(Frame *f)
{
  push_operand(&(f->local_variables[2]), f->operands);
}

void iload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]), f->operands);
}

void lload_0_eval(Frame *f)
{
  push_operand(&(f->local_variables[0]),f->operands);
}

void lload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]),f->operands);
}

void lload_2_eval(Frame *f)
{
  push_operand(&(f->local_variables[2]),f->operands);
}

void lload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]),f->operands);
}

void fload_0_eval(Frame *f)
{
  if(f->local_variables[0].type == CONSTANT_Float){
    push_operand(&(f->local_variables[0]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void fload_1_eval(Frame *f)
{
  if(f->local_variables[1].type == CONSTANT_Float){
    push_operand(&(f->local_variables[1]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void fload_2_eval(Frame *f)
{
  if(f->local_variables[2].type == CONSTANT_Float){
    push_operand(&(f->local_variables[2]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void fload_3_eval(Frame *f)
{
  if(f->local_variables[3].type == CONSTANT_Float){
    push_operand(&(f->local_variables[3]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_0_eval(Frame *f)
{
  //   push_operand();
}

void dload_1_eval(Frame *f)
{
  //   push_operand();
}

void dload_2_eval(Frame *f)
{
  //   push_operand();
}

void dload_3_eval(Frame *f)
{
  //   push_operand();
}

void aload_0_eval(Frame *f)
{
  // printf("lvv: %02x\n", f->local_variables[0].value);
  push_operand(&(f->local_variables[0]), f->operands);
}

void aload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]), f->operands);
}

void aload_2_eval(Frame *f)
{
  push_operand(&(f->local_variables[2]), f->operands);
}

void aload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]), f->operands);
}

void iaload_eval(Frame *f)
{
  //   push_operand();
}

void laload_eval(Frame *f)
{
  //   push_operand();
}

void faload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, result;
  arrayref = pop_operand(f->operands);
  index = pop_operand(f->operands);
  result = arrayref[index->value];

  push_operand(&result, f->operands);
}

void daload_eval(Frame *f)
{
  //   push_operand();
}

void aaload_eval(Frame *f)
{
  //   push_operand();
}

void baload_eval(Frame *f)
{
  //   push_operand();
}

void caload_eval(Frame *f)
{
  //   push_operand();
}

void saload_eval(Frame *f)
{
  //   push_operand();
}

void istore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  LocalVariable *aux = pop_operand(f->operands);
  f->local_variables[index] = *aux;
  printf("istore %d val: %04x\n", index, f->local_variables[index].value);
}

void lstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[index] = *lv;
  printf("lstore val: %04lx\n", f->local_variables[index].type_long);
}

void fstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
  printf("aux: %04x\n", aux->value);
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[index] = *aux_linha;
  printf("fstore_0 val: %04x\n", f->local_variables[index].value);
}

void dstore_eval(Frame *f)
{
  //   push_operand();
}

void astore_eval(Frame *f)
{
  //   push_operand();
}

void istore_0_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[0] = *aux;
  printf("istore_0 val: %04x\n", f->local_variables[0].value);
}

void istore_1_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  printf("aux: %04x\n", aux->value);
  f->local_variables[1] = *aux;
  printf("istore_1 val: %04x\n", f->local_variables[1].value);
}

void istore_2_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[2] = *aux;
  printf("istore_2 val: %04x\n", f->local_variables[2].value);
}

void istore_3_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[3] = *aux;
  printf("istore_3 val: %04x\n", f->local_variables[3].value);
}

void lstore_0_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[0] = *lv;
  printf("lstore_0 val: %04lx\n", f->local_variables[0].type_long);
}

void lstore_1_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[1] = *lv;
  printf("lstore_1 val: %04lx\n", f->local_variables[1].type_long);
}

void lstore_2_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[2] = *lv;
  printf("lstore_2 val: %04lx\n", f->local_variables[2].type_long);
}

void lstore_3_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[3] = *lv;
  printf("lstore_3 val: %04lx\n", f->local_variables[3].type_long);
}

void fstore_0_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
  printf("aux: %04x\n", aux->value);
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[0] = *aux_linha;
  printf("fstore_0 val: %04x\n", f->local_variables[0].value);
}

void fstore_1_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
  printf("aux: %04x\n", aux->value);
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[1] = *aux_linha;
  printf("fstore_1 val: %04x\n", f->local_variables[1].value);
}

void fstore_2_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
  printf("aux: %04x\n", aux->value);
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[2] = *aux_linha;
  printf("fstore_2 val: %04x\n", f->local_variables[2].value);
}

void fstore_3_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
  printf("aux: %04x\n", aux->value);
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[3] = *aux_linha;
  printf("fstore_3 val: %04x\n", f->local_variables[3].value);
}

void dstore_0_eval(Frame *f)
{
  //   push_operand();
}

void dstore_1_eval(Frame *f)
{
  //   push_operand();
}

void dstore_2_eval(Frame *f)
{
  //   push_operand();
}

void dstore_3_eval(Frame *f)
{
  //   push_operand();
}

void astore_0_eval(Frame *f)
{
  //   push_operand();
}

void astore_1_eval(Frame *f)
{
  //   push_operand();
}

void astore_2_eval(Frame *f)
{
  //   push_operand();
}

void astore_3_eval(Frame *f)
{
  //   push_operand();
}

void iastore_eval(Frame *f)
{
  //   push_operand();
}

void lastore_eval(Frame *f)
{
  //   push_operand();
}

void fastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  arrayref = pop_operand(f->operands);
  index = pop_operand(f->operands);
  value = pop_operand(f->operands);

  arrayref[index->value] = *value;
}

void dastore_eval(Frame *f)
{
  //   push_operand();
}

void aastore_eval(Frame *f)
{
  //   push_operand();
}

void bastore_eval(Frame *f)
{
  //   push_operand();
}

void castore_eval(Frame *f)
{
  //   push_operand();
}

void sastore_eval(Frame *f)
{
  //   push_operand();
}

void pop_eval(Frame *f)
{
  //   push_operand();
}

void pop2_eval(Frame *f)
{
  //   push_operand();
}

void dup_eval(Frame *f)
{
  //   push_operand();
}

void dup_x1_eval(Frame *f)
{
  //   push_operand();
}

void dup_x2_eval(Frame *f)
{
  //   push_operand();
}

void dup2_eval(Frame *f)
{
  //   push_operand();
}

void dup2_x1_eval(Frame *f)
{
  //   push_operand();
}

void dup2_x2_eval(Frame *f)
{
  //   push_operand();
}

void swap_eval(Frame *f)
{
  //   push_operand();
}

void iadd_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 + v2;
  printf("v1: %04x\n", v1);
  printf("v2: %04x\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void ladd_eval(Frame *f)
{
  //   push_operand();
}

void fadd_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 + v2;
  printf("v1_float: %04x\n", v1);
  printf("v2_float: %04x\n", v2);
  printf("resultado_float: %04x\n", result->value);
  push_operand(result, f->operands);
}

void dadd_eval(Frame *f)
{
  //   push_operand();
}

void isub_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 - v2;
  printf("v1: %04x\n", v1);
  printf("v2: %04x\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void lsub_eval(Frame *f)
{
  //   push_operand();
}

void fsub_eval(Frame *f)
{
  float v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 - v2;
  printf("v1: %04f\n", v1);
  printf("v2: %04f\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void dsub_eval(Frame *f)
{
  //   push_operand();
}

void imul_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 * v2;
  printf("v1: %04x\n", v1);
  printf("v2: %04x\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void lmul_eval(Frame *f)
{
  //   push_operand();
}

void fmul_eval(Frame *f)
{
  float v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 * v2;
  printf("v1: %04f\n", v1);
  printf("v2: %04f\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void dmul_eval(Frame *f)
{
  //   push_operand();
}

void idiv_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 / v2;
  printf("v1: %04x\n", v1);
  printf("v2: %04x\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void ldiv_eval(Frame *f)
{
  //   push_operand();
}

void fdiv_eval(Frame *f)
{
  float v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  v2 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 / v2;
  printf("v1: %04f\n", v1);
  printf("v2: %04f\n", v2);
  printf("resultado: %04x\n", result->value);
  push_operand(result, f->operands);
}

void ddiv_eval(Frame *f)
{
  //   push_operand();
}

void irem_eval(Frame *f)
{
  //   push_operand();
}

void lrem_eval(Frame *f)
{
  //   push_operand();
}

void frem_eval(Frame *f)
{
  //   push_operand();
}

void drem_eval(Frame *f)
{
  //   push_operand();
}

void ineg_eval(Frame *f)
{
  //   push_operand();
}

void lneg_eval(Frame *f)
{
  //   push_operand();
}

void fneg_eval(Frame *f)
{
  u4 v1;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = 0 - v1;
  printf("resultado_neg: %04x\n", result->value);

  push_operand(result, f->operands);
}

void dneg_eval(Frame *f)
{
  //   push_operand();
}

void ishl_eval(Frame *f)
{
  //   push_operand();
}

void lshl_eval(Frame *f)
{
  //   push_operand();
}

void ishr_eval(Frame *f)
{
  //   push_operand();
}

void lshr_eval(Frame *f)
{
  //   push_operand();
}

void iushr_eval(Frame *f)
{
  //   push_operand();
}

void lushr_eval(Frame *f)
{
  //   push_operand();
}

void iand_eval(Frame *f)
{
  //   push_operand();
}

void land_eval(Frame *f)
{
  //   push_operand();
}

void ior_eval(Frame *f)
{
  //   push_operand();
}

void lor_eval(Frame *f)
{
  //   push_operand();
}

void ixor_eval(Frame *f)
{
  //   push_operand();
}

void lxor_eval(Frame *f)
{
  //   push_operand();
}

void iinc_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  int32_t value = f->bytecode[f->pc++];
  f->local_variables[index].value += value;
}

void i2l_eval(Frame *f)
{
  //   push_operand();
}

void i2f_eval(Frame *f)
{
  //   push_operand();
}

void i2d_eval(Frame *f)
{
  //   push_operand();
}

void l2i_eval(Frame *f)
{
  //   push_operand();
}

void l2f_eval(Frame *f)
{
  //   push_operand();
}

void l2d_eval(Frame *f)
{
  //   push_operand();
}

void f2i_eval(Frame *f)
{
  //   push_operand();
}

void f2l_eval(Frame *f)
{
  //   push_operand();
}

void f2d_eval(Frame *f)
{
  //   push_operand();
}

void d2i_eval(Frame *f)
{
  //   push_operand();
}

void d2l_eval(Frame *f)
{
  //   push_operand();
}

void d2f_eval(Frame *f)
{
  //   push_operand();
}

void i2b_eval(Frame *f)
{
  //   push_operand();
}

void i2c_eval(Frame *f)
{
  //   push_operand();
}

void i2s_eval(Frame *f)
{
  //   push_operand();
}

void lcmp_eval(Frame *f)
{
  //   push_operand();
}

void fcmpl_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  u4 value1, value2;
  v1 = pop_operand(f->operands);
  v2 = pop_operand(f->operands);

  value1 = v1->value;
  value2 = v2->value;
  lv->type = CONSTANT_Integer;
  if(value1 > value2){
    lv->value = 1;
  } else if(value1 == value2){
    lv->value = 0;
  } else if(value1 < value2){
    lv->value = -1;
  } else {
    lv->value = -1;
  }
  push_operand(lv, f->operands);
  //   push_operand();
}

void fcmpg_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  u4 value1, value2;
  v1 = pop_operand(f->operands);
  v2 = pop_operand(f->operands);

  value1 = v1->value;
  value2 = v2->value;
  lv->type = CONSTANT_Integer;
  if(value1 > value2){
    lv->value = 1;
  } else if(value1 == value2){
    lv->value = 0;
  } else if(value1 < value2){
    lv->value = -1;
  } else {
    lv->value = 1;
  }
  push_operand(lv, f->operands);
}

void dcmpl_eval(Frame *f)
{
  //   push_operand();
}

void dcmpg_eval(Frame *f)
{
  //   push_operand();
}

void ifeq_eval(Frame *f)
{
  //   push_operand();
}

void ifne_eval(Frame *f)
{
  //   push_operand();
}

void iflt_eval(Frame *f)
{
  //   push_operand();
}

void ifge_eval(Frame *f)
{
  //   push_operand();
}

void ifgt_eval(Frame *f)
{
  //   push_operand();
}

void ifle_eval(Frame *f)
{
  //   push_operand();
}

void if_icmpeq_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = pop_operand(f->operands)->value;
  int32_t value1 = pop_operand(f->operands)->value;
  if (value1 == value2)
  {
    f->pc += offset - 3;
  }
}

void if_icmpne_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = pop_operand(f->operands)->value;
  int32_t value1 = pop_operand(f->operands)->value;
  if (value1 != value2)
  {
    f->pc += offset - 3;
  }
}

void if_icmplt_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = pop_operand(f->operands)->value;
  int32_t value1 = pop_operand(f->operands)->value;
  if (value1 < value2)
  {
    f->pc += offset - 3;
  }
}

void if_icmpg2_eval(Frame *f)
{
  //   push_operand();
}

void if_icmpgt_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  u4 value1 = pop_operand(f->operands)->value;
  u4 value2 = pop_operand(f->operands)->value;
  if (value1 > value2)
  {
    f->pc += offset - 3;
  }
  
}

void if_icmple_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = pop_operand(f->operands)->value;
  int32_t value1 = pop_operand(f->operands)->value;
  if (value1 <= value2)
  {
    f->pc += offset - 3;
  }
}

void if_acmpeq_eval(Frame *f)
{
  //   push_operand();
}

void if_acmpne_eval(Frame *f)
{
  //   push_operand();
}

void goto_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc];
  branchbyte2 = f->bytecode[f->pc];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  f->pc += offset;
}

void jsr_eval(Frame *f)
{
  //   push_operand();
}

void ret_eval(Frame *f)
{
  //   push_operand();
}

void tableswitch_eval(Frame *f)
{
  //   push_operand();
}

void lookupswitch_eval(Frame *f)
{
  //   push_operand();
}

void ireturn_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

void lreturn_eval(Frame *f)
{
  //   push_operand();
}

void freturn_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

void dreturn_eval(Frame *f)
{
  //   push_operand();
}

void areturn_eval(Frame *f)
{
  //   push_operand();
}

void return_eval(Frame *f)
{
  pop(JvmStack);
  // printf("%02x", JvmStack->f->pc);
}

void getstatic_eval(Frame *f)
{
  //  f->method->attributes->info->Code_attribute.code;
  // u2 opcode = bytecode[current_frame->pc++];
  u1 index1byte, index2byte;
  u1 *bytecode = f->method->attributes->info->Code_attribute.code;
  index1byte = bytecode[f->pc++];
  index2byte = bytecode[f->pc++];

  // recupera Utf8 da referencia do invokespecial
  u2 index = ((index1byte << 8) | index2byte);
  char *class_name = ret_method_name(f->cp, index);

  // Name and type
  // uint16_t name_n_type = f->cp[index-1].Methodref.name_and_type_index;

  printf("nome da classe: %s\n", class_name);
  // printf("local_variable_to_empilhar: %04x\n", f->local_variables[index].value);
  if (strcmp(class_name, "java/lang/System") == 0)
  {
    // push_operand(&(f->local_variables[index]), f->operands);
    return;
  }
  else
  {
    //TODO
  }
}

void putstatic_eval(Frame *f)
{
  //   push_operand();
}

void getfield_eval(Frame *f)
{
  //   push_operand();
}

void putfield_eval(Frame *f)
{
  //   push_operand();
}

void invokevirtual_eval(Frame *f)
{
  u1 index1byte, index2byte;
  u1 *bytecode = f->method->attributes->info->Code_attribute.code;
  index1byte = bytecode[f->pc++];
  index2byte = bytecode[f->pc++];

  // recupera Utf8 da referencia 
  u2 index = ((index1byte << 8) | index2byte);
  char *class_name = ret_method_name(f->cp, index);

  // Method Name and type
  uint16_t name_n_type = f->cp[index - 1].Methodref.name_and_type_index;

  char *method_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *method_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

  if (strcmp(class_name, "java/io/PrintStream") == 0)
  {
    if (strcmp(method_name, "println") == 0 || strcmp(method_name, "print"))
    {
      LocalVariable *lv = pop_operand(f->operands);
      if (strcmp(method_desc, "(Ljava/lang/String;)V") == 0)
      {
        char *string = readUtf8(f->cp, lv->value);
        printf("%s \n", string);
      }
      else if (strcmp(method_desc, "(J)V") == 0)
      {
        int64_t value = lv->type_long;
        printf("%ld \n", value);
      }
      else
      {
        printf("invokevirtualFunction: falta implementar\n");
        exit(0);
      }
    }
    else
    {
      printf("invokevirtualFunction: falta implementar\n");
      exit(0);
    }
  }
  else
  {
    printf("invokevirtualFunction: falta implementar\n");
    exit(0);
  }
}

void invokespecial_eval(Frame *f)
{
  printf("Invocando satanás...\n");
  u1 index1byte, index2byte;
  index1byte = f->pc++;
  index2byte = f->pc++;

  // recupera Utf8 da referencia do invokespecial
  u2 index = ((index1byte << 8) | index2byte);
  // printf("%d", index);
  //algo
  // printf("index: %02x\n",f->cp[index].Class.name_index);
  char *class_name = ret_method_name(f->cp, index);
  printf("string_method: %s\n", class_name);

  if (strcmp("java/lang/Object", class_name) == 0)
  {

		ClassLoader(class_name);
		return;
	}

  if (strcmp("java/lang/StringBuffer", class_name) == 0)
  {

		return;
	}

  if (strcmp("java/util/Scanner", class_name) == 0)
  {

		return;
	}

  //Pega posição da classe no array de classes

  //Pega referencia ao classFile pelo indice anterior.
	// ClassFile* cf = Mem.classes_arr[ClassLoader(class_name)];

  // //Pega o nome e tipo dó método pelo indice da instrução.
	// uint16_t nomeTipoIndice = f->cp[index-1].Methodref.name_and_type_index;

}

void invokestatic_eval(Frame *f)
{
  //   push_operand();
}

void invokeinterface_eval(Frame *f)
{
  //   push_operand();
}

void invokedynamic_eval(Frame *f)
{
  //   push_operand();
}

void new_eval(Frame *f)
{
  //   push_operand();
}

void newarray_eval(Frame *f)
{
  //   push_operand();
}

void anewarray_eval(Frame *f)
{
  //   push_operand();
}

void arraylength_eval(Frame *f)
{
  //   push_operand();
}

void athrow_eval(Frame *f)
{
  //   push_operand();
}

void checkcast_eval(Frame *f)
{
  //   push_operand();
}

void instanceof_eval(Frame *f)
{
  //   push_operand();
}

void monitorenter_eval(Frame *f)
{
  //   push_operand();
}

void monitorexit_eval(Frame *f)
{
  //   push_operand();
}

void wide_eval(Frame *f)
{
  //   push_operand();
}

void multianewarray_eval(Frame *f)
{
  //   push_operand();
}

void ifnull_eval(Frame *f)
{
  //   push_operand();
}

void ifnonnull_eval(Frame *f)
{
  //   push_operand();
}

void goto_w_eval(Frame *f)
{
  //   push_operand();
}

void jsr_w_eval(Frame *f)
{
  //   push_operand();
}

void breakpoint_eval(Frame *f)
{
  //   push_operand();
}

void impdep1_eval(Frame *f)
{
  //   push_operand();
}

void impdep2_eval(Frame *f)
{
  //   push_operand();
}
