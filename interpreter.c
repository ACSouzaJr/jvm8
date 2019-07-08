#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structures.h"
#include "frame.h"
#include "stack_operands.h"
#include "interpreter.h"
#include "stack_frame.h"
#include "classfile.h"
#include "helpers.h"

#define exponent(x) ((x << 1) >> 24)
#define mantiss(x) ((x << 9) >> 9)
#define signal(x) (x >> 31)

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

void nop_eval(Frame *f)
{
}

// da um push NULL para a pilha de operandos
void aconst_null_eval(Frame *f)
{
  push_operand(NULL, f->operands);
}

// da um push de um int constante para a pilha de operandos
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
#ifdef DEBUG
  printf("Iconst_0");
#endif
}

void iconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 1;
  push_operand(lv, f->operands);
#ifdef DEBUG
  printf("Iconst_1");
#endif
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

// da um push de um long constante para a pilha de operandos
void lconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  lv->type_long = 0;
  push_operand(lv, f->operands);
}

void lconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  lv->type_long = 1;
  push_operand(lv, f->operands);
}

// push de um float constante para pilha de operandos (</> 0 ou 1)
void fconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  lv->value = 0.0;
  push_operand(lv, f->operands);
}

void fconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  lv->value = 1.0;
  push_operand(lv, f->operands);
}

void fconst_2_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  lv->value = 2.0;
  push_operand(lv, f->operands);
}

// push de um double constante para pilha de operandos (</> 0 ou 1)
void dconst_0_eval(Frame *f)
{
  //   push_operand();
}

void dconst_1_eval(Frame *f)
{
  //   push_operand();
}

// push de um byte na pilha de operandos - o byte tem seu sinal extendido para um valor int
void bipush_eval(Frame *f)
{
  // Pega o byte de argument extend para int e empilha nos operandos.
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = (int32_t)f->bytecode[f->pc++];
#ifdef DEBUG
  printf("bipush: %d \n", lv->value);
#endif
  push_operand(lv, f->operands);
}

// push para a pilha de operandos de um valor do tipo shot
// recebe dois bytes e os junto em um intermediário de 16 bits, para futuramente extender seu sinal e dar push para a pilha de operandos
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

// push de um item da constant_pool
// o índice de acesso à constant_pool deve ser de categoria 1
// Caso seja int ou float, seu valor é levado à cp como int, ou float
// Caso seja String, então a referência value para aquela instância é levada a op_stack
// Caso seja uma referência para uma classe, é retornada uma referência para a class Object [0]
void ldc_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  cp_info *item = &(f->cp[index - 1]);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
#ifdef DEBUG
  printf("tag: %01x\n", item->tag);
#endif
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

// Load de um int vindo do vetor de variáveis locais - índice sem sinal
void iload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]), f->operands);
#ifdef DEBUG
  printf("iload %d", f->local_variables[index].value);
#endif
}

// Load de um long vindo vo vetor de variáveis locais - índice sem sinal
void lload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]), f->operands);
}

void fload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  if (f->local_variables[index].type == CONSTANT_Float)
  {
    push_operand(&(f->local_variables[index]), f->operands);
  }
  else
  {
#ifdef DEBUG
    printf("javax.persistence.PersistenceException\n");
#endif
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
#ifdef DEBUG
  printf("iload_1");
#endif
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
  push_operand(&(f->local_variables[0]), f->operands);
}

void lload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]), f->operands);
}

void lload_2_eval(Frame *f)
{
  push_operand(&(f->local_variables[2]), f->operands);
}

void lload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]), f->operands);
}

void fload_0_eval(Frame *f)
{
  if (f->local_variables[0].type == CONSTANT_Float)
  {
    push_operand(&(f->local_variables[0]), f->operands);
  }
  else
  {
#ifdef DEBUG
    printf("javax.persistence.PersistenceException\n");
#endif
    exit(0);
  }
}

void fload_1_eval(Frame *f)
{
  if (f->local_variables[1].type == CONSTANT_Float)
  {
    push_operand(&(f->local_variables[1]), f->operands);
  }
  else
  {
#ifdef DEBUG
    printf("javax.persistence.PersistenceException\n");
#endif
    exit(0);
  }
}

void fload_2_eval(Frame *f)
{
  if (f->local_variables[2].type == CONSTANT_Float)
  {
    push_operand(&(f->local_variables[2]), f->operands);
  }
  else
  {
#ifdef DEBUG
    printf("javax.persistence.PersistenceException\n");
#endif
    exit(0);
  }
}

void fload_3_eval(Frame *f)
{
  if (f->local_variables[3].type == CONSTANT_Float)
  {
    push_operand(&(f->local_variables[3]), f->operands);
  }
  else
  {
#ifdef DEBUG
    printf("javax.persistence.PersistenceException\n");
#endif
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
#ifdef DEBUG
// printf("lvv: %02x\n", f->local_variables[0].value);
#endif
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
  // Incompleto
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  // value.value = ((u4*)arrayref->type_array.array)[index->value];
  lv->value = ((u4 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Integer;
  // value.value = arrayref.value[index->value];

  push_operand(lv, f->operands);
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

// recupera um valor int da op_stack e da store desse operando na pilha de var_locais
void istore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  LocalVariable *aux = pop_operand(f->operands);
  f->local_variables[index] = *aux;
#ifdef DEBUG
  printf("istore %d val: %04x\n", index, f->local_variables[index].value);
#endif
}

void lstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[index] = *lv;
#ifdef DEBUG
  printf("lstore val: %04llx\n", f->local_variables[index].type_long);
#endif
}

void fstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[index] = *aux_linha;
#ifdef DEBUG
  printf("fstore_0 val: %04x\n", f->local_variables[index].value);
#endif
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
#ifdef DEBUG
  printf("istore_0 val: %04x\n", f->local_variables[0].value);
#endif
}

void istore_1_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[1] = *aux;
#ifdef DEBUG
  printf("istore_1 val: %04x\n", f->local_variables[1].value);
#endif
}

void istore_2_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[2] = *aux;
#ifdef DEBUG
  printf("istore_2 val: %04x\n", f->local_variables[2].value);
#endif
}

void istore_3_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[3] = *aux;
#ifdef DEBUG
  printf("istore_3 val: %04x\n", f->local_variables[3].value);
#endif
}

void lstore_0_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[0] = *lv;
#ifdef DEBUG
  printf("lstore_0 val: %04llx\n", f->local_variables[0].type_long);
#endif
}

void lstore_1_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[1] = *lv;
#ifdef DEBUG
  printf("lstore_1 val: %04llx\n", f->local_variables[1].type_long);
#endif
}

void lstore_2_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[2] = *lv;
#ifdef DEBUG
  printf("lstore_2 val: %04llx\n", f->local_variables[2].type_long);
#endif
}

void lstore_3_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[3] = *lv;
#ifdef DEBUG
  printf("lstore_3 val: %04llx\n", f->local_variables[3].type_long);
#endif
}

void fstore_0_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[0] = *aux_linha;
#ifdef DEBUG
  printf("fstore_0 val: %04x\n", f->local_variables[0].value);
#endif
}

void fstore_1_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[1] = *aux_linha;
#ifdef DEBUG
  printf("fstore_1 val: %04x\n", f->local_variables[1].value);
#endif
}

void fstore_2_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[2] = *aux_linha;
#ifdef DEBUG
  printf("fstore_2 val: %04x\n", f->local_variables[2].value);
#endif
}

void fstore_3_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[3] = *aux_linha;
#ifdef DEBUG
  printf("fstore_3 val: %04x\n", f->local_variables[3].value);
#endif
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
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

#ifdef DEBUG
  printf("array ref (dentro do iastore): %x \n", (u4)arrayref);
#endif

  u4 *vetor;
  vetor = (u4 *)arrayref->type_array.array;
  // (u4 *) arrayref->type_array.array[index->value] = value;
  vetor[index->value] = value->value;
#ifdef DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
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

// só usado para int -> cat1
void pop_eval(Frame *f)
{
  pop_operand(f->operands);
}

// só usado para long/double -> cat2
void pop2_eval(Frame *f)
{
  pop_operand(f->operands);
}

void dup_eval(Frame *f)
{
  // duplica o topo da pilha
  LocalVariable *dup_top = pop_operand(f->operands);

  push_operand(dup_top, f->operands);
  push_operand(dup_top, f->operands);
}

// só usado para int -> cat1
void dup_x1_eval(Frame *f)
{
  LocalVariable *dup_top1 = pop_operand(f->operands);
  LocalVariable *dup_top2 = pop_operand(f->operands);

  push_operand(dup_top1, f->operands);
  push_operand(dup_top2, f->operands);
  push_operand(dup_top1, f->operands);
}

void dup_x2_eval(Frame *f)
{
  // se for cat1 => 1 3 2 1
  LocalVariable *dup_top1 = pop_operand(f->operands);
  LocalVariable *dup_top2 = pop_operand(f->operands);
  LocalVariable *dup_top3 = pop_operand(f->operands);

  push_operand(dup_top1, f->operands);
  push_operand(dup_top3, f->operands);
  push_operand(dup_top2, f->operands);
  push_operand(dup_top1, f->operands);

  // se for cat2 -> implementar => 1 2 1
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 + v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void ladd_eval(Frame *f)
{
  //   push_operand();
}

void fadd_eval(Frame *f)
{
  u4 v1, v2, r1;
  float value1, value2, res;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  value1 = *(float *)&v1;
  value2 = *(float *)&v2;
  res = value1 + value2;

  r1 = convertFloatToBytes(&res);

  result->value = r1;
#ifdef DEBUG
  printf("v1_float: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2_float: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado_float: %04x\n", result->value);
#endif
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 - v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 - v2;
#ifdef DEBUG
  printf("v1: %04f\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 * v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 * v2;
#ifdef DEBUG
  printf("v1: %04f\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 / v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
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

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Float;
  result->value = v1 / v2;
#ifdef DEBUG
  printf("v1: %04f\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void ddiv_eval(Frame *f)
{
  //   push_operand();
}

void irem_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 - (v1 / v2) * v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
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
  LocalVariable *result = pop_operand(f->operands);
  result->value = -result->value;
#ifdef DEBUG
  printf("value: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
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
#ifdef DEBUG
  printf("resultado_neg: %04x\n", result->value);
#endif

  push_operand(result, f->operands);
}

void dneg_eval(Frame *f)
{
  //   push_operand();
}

void ishl_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 << (v2 & 0x1F);
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", (v2 & 0x1F));
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void lshl_eval(Frame *f)
{
  //   push_operand();
}

void ishr_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 >> (v2 & 0x1F);
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", (v2 & 0x1F));
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void lshr_eval(Frame *f)
{
  //   push_operand();
}

void iushr_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 >> (v2 & 0x1F);
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", (v2 & 0x1F));
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void lushr_eval(Frame *f)
{
  //   push_operand();
}

void iand_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 & v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void land_eval(Frame *f)
{
  //   push_operand();
}

void ior_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 | v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void lor_eval(Frame *f)
{
  //   push_operand();
}

void ixor_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 ^ v2;
#ifdef DEBUG
  printf("v1: %04x\n", v1);
#endif
#ifdef DEBUG
  printf("v2: %04x\n", v2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
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
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  value1 = v1->value;
  value2 = v2->value;
  lv->type = CONSTANT_Integer;
  if (value1 > value2)
  {
    lv->value = 1;
  }
  else if (value1 == value2)
  {
    lv->value = 0;
  }
  else if (value1 < value2)
  {
    lv->value = -1;
  }
  else
  {
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
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  value1 = v1->value;
  value2 = v2->value;
  lv->type = CONSTANT_Integer;
  if (value1 > value2)
  {
    lv->value = 1;
  }
  else if (value1 == value2)
  {
    lv->value = 0;
  }
  else if (value1 < value2)
  {
    lv->value = -1;
  }
  else
  {
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

void if_icmpge_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = pop_operand(f->operands)->value;
  int32_t value1 = pop_operand(f->operands)->value;
  if (value1 >= value2)
  {
    f->pc += offset - 3;
  }
}

void if_icmpgt_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = pop_operand(f->operands)->value;
  int32_t value1 = pop_operand(f->operands)->value;
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
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  f->pc += offset - 3;
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
#ifdef DEBUG
// printf("%02x", JvmStack->f->pc);
#endif
}

void getstatic_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);

  // recupera Utf8 da referencia do invokespecial
  char *class_name = ret_method_name(f->cp, index);

#ifdef DEBUG
  printf("nome da classe: %s\n", class_name);
#endif
#ifdef DEBUG
// printf("local_variable_to_empilhar: %04x\n", f->local_variables[index].value);
#endif
  if (strcmp(class_name, "java/lang/System") == 0)
  {
    // push_operand(&(f->local_variables[index]), f->operands);
    return;
  }

  u2 nati = f->cp[index - 1].Fieldref.name_and_type_index;

  // char *field_name = readUtf8(f->cp, f->cp[nati - 1].NameAndType.name_index);
  char *field_desc = readUtf8(f->cp, f->cp[nati - 1].NameAndType.descriptor_index);

#ifdef DEBUG
  // printf("field_name: %s\n", field_name);
  printf("field_desc: %s\n", field_desc);
#endif

  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  if (strcmp(field_desc, "[I") == 0)
  {
#ifdef DEBUG
    printf("get static_data_low: %04x\n", GLOBAL_CLASS->fields->staticData->low[0]);
#endif
    lv->type_array.array = (u4*)GLOBAL_CLASS->fields->staticData->low[0];
    lv->type = CONSTANT_Fieldref;
    push_operand(lv, f->operands);
  }
}

void putstatic_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  u2 nati = f->cp[index - 1].Fieldref.name_and_type_index;

  // char *field_name = readUtf8(f->cp, f->cp[nati - 1].NameAndType.name_index);
  char *field_desc = readUtf8(f->cp, f->cp[nati - 1].NameAndType.descriptor_index);

#ifdef DEBUG
  // printf("field_name: %s\n", field_name);
  printf("field_desc: %s\n", field_desc);
#endif

  LocalVariable *lv;

  lv = pop_operand(f->operands);
  // lv = f->operands->top;

#ifdef DEBUG
  printf("put lv_putstatic: %04x\n", lv->value);
#endif

  if (strcmp(field_desc, "[I") == 0)
  {
    GLOBAL_CLASS->fields->staticData = (staticData *)malloc(sizeof(staticData));
    GLOBAL_CLASS->fields->staticData->low = (u4 *)malloc(sizeof(u4));
    GLOBAL_CLASS->fields->staticData->high = NULL;
    GLOBAL_CLASS->fields->staticData->low[0] = lv->value;

#ifdef DEBUG
    printf("put static_data_low: %04x\n", GLOBAL_CLASS->fields->staticData->low[0]);
#endif
  }
  else if (strcmp(field_desc, "I") == 0)
  {
    GLOBAL_CLASS->fields->staticData = (staticData *)malloc(sizeof(staticData));
    GLOBAL_CLASS->fields->staticData->low = (u4 *)malloc(sizeof(u4));
    GLOBAL_CLASS->fields->staticData->high = NULL;
    GLOBAL_CLASS->fields->staticData->low[0] = lv->value;

#ifdef DEBUG
    printf("put static_data_low: %04x\n", GLOBAL_CLASS->fields->staticData->low[0]);
#endif
  }
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
      else if (strcmp(method_desc, "(I)V") == 0)
      {
        // printf("PASSEI POR AQUI DE MOTO\n");
        int32_t value = lv->value;
        printf("%d \n", value);
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
#ifdef DEBUG
  printf("Invocando satanás...\n");
#endif
  u1 index1byte, index2byte;
  index1byte = f->pc++;
  index2byte = f->pc++;

  // recupera Utf8 da referencia do invokespecial
  u2 index = ((index1byte << 8) | index2byte);
#ifdef DEBUG
// printf("%d", index);
#endif
//algo
#ifdef DEBUG
// printf("index: %02x\n",f->cp[index].Class.name_index);
#endif
  char *class_name = ret_method_name(f->cp, index);
#ifdef DEBUG
  printf("string_method: %s\n", class_name);
#endif

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
  u2 index = getIndexFromb1b2(f);
  // Method Name and type
  uint16_t name_n_type = f->cp[index - 1].Methodref.name_and_type_index;

  char *method_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *method_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);
  u2 args = 0;

  char *class_name = ret_method_name(f->cp, index);

  if (strcmp(class_name, "java/lang/Object") == 0)
  {
    printf("Nao implementado ! \n");
    return;
  }

  for (size_t i = 1; method_desc[i] != ')'; i++)
  {
    if (method_desc[i] == '[')
    {
      continue;
    }
    if (method_desc[i] == 'L')
    {
      while (method_desc[++i] != ';')
        ;
    }
    args++;
  }

#ifdef DEBUG
  printf("Argumentos %d", args);
#endif

  u2 class_index = find_class(class_name);
  method_info *method = find_method(Mem.classes_arr[class_index], method_name);
  Frame *frame = cria_frame(f->cp, method);
  // Adiciona argumestos
  // for (size_t i = args - 1; i >= 0; i--)
  for (size_t i = 0; i <= args; i++)
  {
    frame->local_variables[i] = *(pop_operand(f->operands));
#ifdef DEBUG
    printf("DEBUG DE VERDADE:  ==== %04x\n", frame->local_variables[i].value);
#endif
  }

  push(frame);
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
  u2 index = getIndexFromb1b2(f);
  char *class_name = ret_method_name(f->cp, index);
  u2 class_index = ClassLoader(class_name);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Class;
  lv->value = class_index;
  // Empilha referencia para a classe no array de classes;
  push_operand(lv,f->operands);
}

void newarray_eval(Frame *f)
{
  LocalVariable *lv, *rlv;
  u4 count;
  u1 byte1;
  u1 *bytecode = f->method->attributes->info->Code_attribute.code;
  byte1 = bytecode[f->pc++];
  lv = pop_operand(f->operands);
  count = lv->value;
  void *arrayref = NULL;
  rlv = (LocalVariable *)malloc(sizeof(LocalVariable));

  if (count < 0)
  {
#ifdef DEBUG
    printf("NegativeArraySizeException.\n");
#endif
  }
  else
  {
    switch (byte1)
    {
    case T_BOOLEAN:
      rlv->type = CONSTANT_Integer;
      arrayref = (u4 *)malloc((count + 1) * sizeof(u4));
      // int return_array[count];
      break;
    case T_CHAR:
      rlv->type = CONSTANT_String;
      arrayref = (u1 *)malloc((count + 1) * sizeof(u1));
      // char return_array[count];
      break;
    case T_FLOAT:
      rlv->type = CONSTANT_Float;
      arrayref = (u4 *)malloc((count + 1) * sizeof(u4));
      // float return_array[count];
      break;
    case T_DOUBLE:
      arrayref = (uint64_t *)malloc((count + 1) * sizeof(uint64_t));
      // double return_array[count];
      break;
    case T_BYTE:
      rlv->type = CONSTANT_Integer;
      arrayref = (u1 *)malloc((count + 1) * sizeof(u1));
      // u1 return_array[count];
      break;
    case T_SHORT:
      rlv->type = CONSTANT_Integer;
      arrayref = (u2 *)malloc((count + 1) * sizeof(u2));
      // u2 return_array[count];
      break;
    case T_INT:
      rlv->type = CONSTANT_Integer;
      arrayref = (u4 *)malloc((count) * sizeof(u4));
#ifdef DEBUG
      printf("array ref: %x \n", (u4)arrayref);
#endif
      rlv->type_array.array = (u4 *)arrayref;
      rlv->type_array.size = count;
#ifdef DEBUG
      printf("array type: %x \n", (u4)(rlv->type_array.array));
      printf("rlv type: %x \n", rlv->type);
#endif
      // int return_array[count];
      break;
    case T_LONG:
      arrayref = (uint64_t *)malloc((count + 1) * sizeof(uint64_t));
      // long return_array[count];
      break;
    default:
      break;
    }
    // rlv->type_array.array = arrayref;

    push_operand(rlv, f->operands);
  }
}

void anewarray_eval(Frame *f)
{
  // u2 index = getIndexFromb1b2(f);
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
