#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
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

u2 count_args(char *method_desc)
{
  u2 args = 0;
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
  return args;
}

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
  long value = 0;
  lv->type_long = *(uint64_t *)&value;
  push_operand(lv, f->operands);
}

void lconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  long value = 1;
  lv->type_long = *(uint64_t *)&value;
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
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  double aux = 0.0;
  lv->type = CONSTANT_Double;
  lv->type_double = *(uint64_t *)&(aux);
  push_operand(lv, f->operands);
#ifdef DEBUG
  printf("dconst_0: %f\n", *(double *)&(lv->type_double));
#endif
}

void dconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  double aux = 1.0;
  lv->type = CONSTANT_Double;
  lv->type_double = *(uint64_t *)&(aux);
  push_operand(lv, f->operands);
#ifdef DEBUG
  printf("dconst_1: %f\n", *(double *)&(lv->type_double));
#endif
}

// push de um byte na pilha de operandos - o byte tem seu sinal extendido para um valor int
void bipush_eval(Frame *f)
{
  // Pega o byte de argument extend para int e empilha nos operandos.
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = (int32_t)((int8_t)(f->bytecode[f->pc++]));
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
  //TODO
}

void ldc2_w_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  u4 low;
  u4 high;
  uint64_t total;
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  switch (f->cp[index - 1].tag)
  {
  case CONSTANT_Double:
    low = f->cp[index - 1].Double.low_bytes;
    high = f->cp[index - 1].Double.high_bytes;
#ifdef DEBUG
    printf("low: %04x, high: %04x\n", low, high);
#endif
    total = (int64_t)high << 32 | (int64_t)low;
#ifdef DEBUG
    printf("total: %04x \n", total);
    printf("total: %f \n", *(double *)&total);
#endif
    lv->type = CONSTANT_Double;
    lv->type_double = total;
    break;
  case CONSTANT_Long:
    low = f->cp[index - 1].Long.low_bytes;
    high = f->cp[index - 1].Long.high_bytes;
#ifdef DEBUG
    printf("low: %04x, high: %04x\n", low, high);
#endif
    total = (int64_t)high << 32 | (int64_t)low;
#ifdef DEBUG
    printf("total: %04x \n", total);
    printf("total: %ld \n", *(long *)&total);
#endif
    lv->type = CONSTANT_Long;
    lv->type_double = total;
    break;

  default:
    break;
  }
  push_operand(lv, f->operands);
  // uint8_t indice = f->bytecode[f->pc + 2];
  // uint8_t tag = (f->cp[indice-1]).tag;
  // if(tag == 5) {
  //   uint32_t alta = f->cp[indice-1].info.Long.high_bytes;
  // 	uint32_t baixa = frameCorrente->constant_pool[indice-1].info.Long.low_bytes;
  // 	push(alta);
  // 	push(baixa);
  // }
  // if(tag == 6) {

  // }
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
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  if (f->local_variables[index].type == CONSTANT_Double)
  {
    push_operand(&(f->local_variables[index]), f->operands);
  }
  else
  {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void aload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]), f->operands);
#ifdef DEBUG
  printf("aload %d", f->local_variables[index].value);
#endif
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
  #ifdef DEBUG
    printf("fload_0 type: %01x\n",f->local_variables[0].type);
  #endif
  if (f->local_variables[0].type == CONSTANT_Float)
  {
    push_operand(&(f->local_variables[0]), f->operands);
  }
  else
  {
    printf("javax.persistence.PersistenceException\n");
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
    printf("javax.persistence.PersistenceException\n");
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

    printf("javax.persistence.PersistenceException\n");
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
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_0_eval(Frame *f)
{
  if (f->local_variables[0].type == CONSTANT_Double)
  {
    push_operand(&(f->local_variables[0]), f->operands);
  }
  else
  {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_1_eval(Frame *f)
{
  if (f->local_variables[1].type == CONSTANT_Double)
  {
#ifdef DEBUG
    printf("DOUBLE TYPE\n");
#endif
    push_operand(&(f->local_variables[1]), f->operands);
  }
  else
  {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_2_eval(Frame *f)
{
  if (f->local_variables[2].type == CONSTANT_Double)
  {
    push_operand(&(f->local_variables[2]), f->operands);
  }
  else
  {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

void dload_3_eval(Frame *f)
{
  if (f->local_variables[3].type == CONSTANT_Double)
  {
    push_operand(&(f->local_variables[3]), f->operands);
  }
  else
  {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
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
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->type_long = ((uint64_t *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Long;

  push_operand(lv, f->operands);
}

void faload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->value = ((u4 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Float;

  push_operand(lv, f->operands);
}

void daload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->type_double = ((uint64_t *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Double;

  push_operand(lv, f->operands);
}

void aaload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  lv->value = ((u4 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Class;

  push_operand(lv, f->operands);
}

void baload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->value = ((u1 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

void caload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->value = ((u1 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

void saload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->value = ((u2 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
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
  printf("lstore val: %04lx\n", f->local_variables[index].type_long);
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
  u1 index = f->bytecode[f->pc++];

  LocalVariable *aux;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->type_double);
#endif
  f->local_variables[index] = *aux;
#ifdef DEBUG
  printf("dstore val: %f\n", f->local_variables[index].type_double);
#endif
}

void astore_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  u1 index = f->bytecode[f->pc++];
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[index] = *aux;
#ifdef DEBUG
  printf("astore val: %04x\n", f->local_variables[index].value);
#endif
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
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[0] = *aux_linha;
#ifdef DEBUG
  printf("dstore_0 val: %08x\n", f->local_variables[0].type_double);
#endif
}

void dstore_1_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[1] = *aux_linha;
#ifdef DEBUG
  printf("dstore_1 val: %08x\n", f->local_variables[1].type_double);
#endif
}

void dstore_2_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[2] = *aux_linha;
#ifdef DEBUG
  printf("dstore_2 val: %08x\n", f->local_variables[2].type_double);
#endif
}

void dstore_3_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[3] = *aux_linha;
#ifdef DEBUG
  printf("dstore_3 val: %08x\n", f->local_variables[3].type_double);
#endif
}

void astore_0_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[0] = *aux;
#ifdef DEBUG
  printf("astore_0 val: %04x\n", f->local_variables[0].value);
#endif
}

void astore_1_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[1] = *aux;
#ifdef DEBUG
  printf("astore_1 val: %04x\n", f->local_variables[1].value);
#endif
}

void astore_2_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[2] = *aux;
#ifdef DEBUG
  printf("astore_2 val: %04x\n", f->local_variables[2].value);
#endif
}

void astore_3_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#ifdef DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[3] = *aux;
#ifdef DEBUG
  printf("astore_3 val: %04x\n", f->local_variables[3].value);
#endif
}

void iastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

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
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  uint64_t *vetor;
  vetor = (uint64_t *)arrayref->type_array.array;
  // (u4 *) arrayref->type_array.array[index->value] = value;
  vetor[index->value] = value->type_long;
#ifdef DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
}

void fastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u4 *vetor;
  vetor = (u4 *)arrayref->type_array.array;
  // (u4 *) arrayref->type_array.array[index->value] = value;
  vetor[index->value] = value->value;
#ifdef DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
}

void dastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  uint64_t *vetor;
  vetor = (uint64_t *)arrayref->type_array.array;
  // (u4 *) arrayref->type_array.array[index->value] = value;
  vetor[index->value] = value->type_double;
#ifdef DEBUG
  printf("Referencia array: %d", ((uint64_t *)arrayref->type_array.array)[index->value]);
#endif
}

void aastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u4 *vetor;
  vetor = (u4 *)arrayref->type_array.array;
  vetor[index->value] = value->value;
  #ifdef DEBUG
    printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
  #endif
}

void bastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u1 *vetor;
  vetor = (u1 *)arrayref->type_array.array;
  vetor[index->value] = value->value;
  #ifdef DEBUG
    printf("Referencia array: %d", ((u1 *)arrayref->type_array.array)[index->value]);
  #endif
}

void castore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u1 *vetor;
  vetor = (u1 *)arrayref->type_array.array;
  vetor[index->value] = value->value;
  #ifdef DEBUG
    printf("Referencia array: %d", ((u1 *)arrayref->type_array.array)[index->value]);
  #endif
}

void sastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u2 *vetor;
  vetor = (u2 *)arrayref->type_array.array;
  vetor[index->value] = value->value;
  #ifdef DEBUG
    printf("Referencia array: %d", ((u2 *)arrayref->type_array.array)[index->value]);
  #endif
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

  // se for cat2 -> implementar => 1 2 1
  if((dup_top1->type != CONSTANT_Long && dup_top1->type != CONSTANT_Double)
      && (dup_top2->type == CONSTANT_Long || dup_top2->type == CONSTANT_Double)
    ) {
    push_operand(dup_top1, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
  else {
    LocalVariable *dup_top3 = pop_operand(f->operands);

    push_operand(dup_top1, f->operands);
    push_operand(dup_top3, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
}

void dup2_eval(Frame *f)
{
  LocalVariable *dup_top1 = pop_operand(f->operands);
  
  // se for cat2 -> implementar => 1 2 1
  if((dup_top1->type != CONSTANT_Long && dup_top1->type != CONSTANT_Double)
      && (f->operands->top->f == CONSTANT_Long || f->operands->top->f == CONSTANT_Double)
    ) {
    push_operand(dup_top1, f->operands);
    push_operand(dup_top1, f->operands);
  }
  else {
    LocalVariable *dup_top2 = pop_operand(f->operands);

    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
}

void dup2_x1_eval(Frame *f)
{
  LocalVariable *dup_top1 = pop_operand(f->operands);
  LocalVariable *dup_top2 = pop_operand(f->operands);
  
  // se for cat2 -> implementar => 1 2 1
  if((dup_top1->type != CONSTANT_Long && dup_top1->type != CONSTANT_Double)
      && (dup_top2->type == CONSTANT_Long && dup_top2->type == CONSTANT_Double)
    ) {
    push_operand(dup_top1, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
  else {
    LocalVariable *dup_top3 = pop_operand(f->operands);

    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
    push_operand(dup_top3, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
}

void dup2_x2_eval(Frame *f)
{
  LocalVariable *dup_top1 = pop_operand(f->operands);
  LocalVariable *dup_top2 = pop_operand(f->operands);
  LocalVariable *dup_top3 = pop_operand(f->operands);
  
  // se for cat2 -> implementar => 1 2 1
  if((dup_top1->type == CONSTANT_Long || dup_top1->type == CONSTANT_Double)
      && (dup_top2->type != CONSTANT_Long && dup_top2->type != CONSTANT_Double)
      && (dup_top3->type != CONSTANT_Long && dup_top3->type != CONSTANT_Double)){
    push_operand(dup_top1, f->operands);
    push_operand(dup_top3, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
  else if((dup_top1->type != CONSTANT_Long && dup_top1->type != CONSTANT_Double)
      && (dup_top2->type != CONSTANT_Long && dup_top2->type != CONSTANT_Double)
      && (dup_top3->type == CONSTANT_Long || dup_top3->type == CONSTANT_Double)) {
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
    push_operand(dup_top3, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
  else if((dup_top1->type != CONSTANT_Long && dup_top1->type != CONSTANT_Double)
      && (dup_top2->type != CONSTANT_Long && dup_top2->type != CONSTANT_Double)){
    push_operand(dup_top1, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
  else {
    LocalVariable *dup_top4 = pop_operand(f->operands);

    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
    push_operand(dup_top4, f->operands);
    push_operand(dup_top3, f->operands);
    push_operand(dup_top2, f->operands);
    push_operand(dup_top1, f->operands);
  }
}

void swap_eval(Frame *f)
{
  //TODO
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
  uint64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 + value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
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

  // r1 = convertFloatToBytes(&res);
  r1 = *(u4 *)&res;

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
  int64_t v1, v2;
  double value1, value2, resultdouble;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_double;
  v1 = pop_operand(f->operands)->type_double;
  value1 = *(double *)&v1;
  value2 = *(double *)&v2;

  resultdouble = value1 + value2;
  result->type = CONSTANT_Double;

  result->type_double = *(uint64_t *)&resultdouble;
#ifdef DEBUG
  printf("v1_double: %f \n", value1);
#endif
#ifdef DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#ifdef DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
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
  int64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 - value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

void fsub_eval(Frame *f)
{
  int64_t v1, v2;
  float value1, value2, resultfloat;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;

  value1 = *(float *)&v1;
  value2 = *(float *)&v2;
  resultfloat = value1 - value2;

  result->type = CONSTANT_Float;
  result->value = *(uint64_t *)&resultfloat;
#ifdef DEBUG
  printf("v1: %04f\n", value1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", value2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void dsub_eval(Frame *f)
{
  int64_t v1, v2;
  double value1, value2, resultdouble;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_double;
  v1 = pop_operand(f->operands)->type_double;
  value1 = *(double *)&v1;
  value2 = *(double *)&v2;
  // memcpy(&value1, &v1, sizeof(double));
  // memcpy(&value2, &v2, sizeof(double));
  resultdouble = value1 - value2;
  result->type = CONSTANT_Double;
  // result->type_double = convertDoubleToBytes(&resultdouble);
  //memcpy(&(result->type_double), &resultdouble, sizeof(uint64_t));
  // result->type_double = *(uint64_t *)&resultdouble;
  // result->type_double = *(uint64_t*)&((*(double *)&v1) + (*(double *)&v2));
  result->type_double = *(uint64_t *)&resultdouble;
#ifdef DEBUG
  printf("v1_double: %f \n", value1);
#endif
#ifdef DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#ifdef DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
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
  int64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 * value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

void fmul_eval(Frame *f)
{
  int64_t v1, v2;
  float value1, value2, resultfloat;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;

  value1 = *(float *)&v1;
  value2 = *(float *)&v2;
  resultfloat = value1 * value2;

  result->type = CONSTANT_Float;
  result->value = *(uint64_t *)&resultfloat;
#ifdef DEBUG
  printf("v1: %04f\n", value1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", value2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void dmul_eval(Frame *f)
{
  int64_t v1, v2;
  double value1, value2, resultdouble;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_double;
  v1 = pop_operand(f->operands)->type_double;
  value1 = *(double *)&v1;
  value2 = *(double *)&v2;
  // memcpy(&value1, &v1, sizeof(double));
  // memcpy(&value2, &v2, sizeof(double));
  resultdouble = value1 * value2;
  result->type = CONSTANT_Double;
  // result->type_double = convertDoubleToBytes(&resultdouble);
  //memcpy(&(result->type_double), &resultdouble, sizeof(uint64_t));
  // result->type_double = *(uint64_t *)&resultdouble;
  // result->type_double = *(uint64_t*)&((*(double *)&v1) + (*(double *)&v2));
  result->type_double = *(uint64_t *)&resultdouble;
#ifdef DEBUG
  printf("v1_double: %f \n", value1);
#endif
#ifdef DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#ifdef DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
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
  int64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 / value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

void fdiv_eval(Frame *f)
{
  int64_t v1, v2;
  float value1, value2, resultfloat;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;

  value1 = *(float *)&v1;
  value2 = *(float *)&v2;
  resultfloat = value1 / value2;

  result->type = CONSTANT_Float;
  result->value = *(uint64_t *)&resultfloat;
#ifdef DEBUG
  printf("v1: %04f\n", value1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", value2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void ddiv_eval(Frame *f)
{
  int64_t v1, v2;
  double value1, value2, resultdouble;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_double;
  v1 = pop_operand(f->operands)->type_double;
  value1 = *(double *)&v1;
  value2 = *(double *)&v2;
  // memcpy(&value1, &v1, sizeof(double));
  // memcpy(&value2, &v2, sizeof(double));
  resultdouble = value1 / value2;
  result->type = CONSTANT_Double;
  // result->type_double = convertDoubleToBytes(&resultdouble);
  //memcpy(&(result->type_double), &resultdouble, sizeof(uint64_t));
  // result->type_double = *(uint64_t *)&resultdouble;
  // result->type_double = *(uint64_t*)&((*(double *)&v1) + (*(double *)&v2));
  result->type_double = *(uint64_t *)&resultdouble;
#ifdef DEBUG
  printf("v1_double: %f \n", value1);
#endif
#ifdef DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#ifdef DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
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
  int64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 - (value1 / value2) * value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

void frem_eval(Frame *f)
{
  int64_t v1, v2;
  float value1, value2, resultfloat;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;

  value1 = *(float *)&v1;
  value2 = *(float *)&v2;
  resultfloat = fmodf(value1, value2);

  result->type = CONSTANT_Float;
  result->value = *(uint64_t *)&resultfloat;
#ifdef DEBUG
  printf("v1: %04f\n", value1);
#endif
#ifdef DEBUG
  printf("v2: %04f\n", value2);
#endif
#ifdef DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void drem_eval(Frame *f)
{
  int64_t v1, v2;
  double value1, value2, resultdouble;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_double;
  v1 = pop_operand(f->operands)->type_double;
  value1 = *(double *)&v1;
  value2 = *(double *)&v2;
  // memcpy(&value1, &v1, sizeof(double));
  // memcpy(&value2, &v2, sizeof(double));
  resultdouble = fmod(value1, value2);
  result->type = CONSTANT_Double;
  // result->type_double = convertDoubleToBytes(&resultdouble);
  //memcpy(&(result->type_double), &resultdouble, sizeof(uint64_t));
  // result->type_double = *(uint64_t *)&resultdouble;
  // result->type_double = *(uint64_t*)&((*(double *)&v1) + (*(double *)&v2));
  result->type_double = *(uint64_t *)&resultdouble;
#ifdef DEBUG
  printf("v1_double: %f \n", value1);
#endif
#ifdef DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#ifdef DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
}

void ineg_eval(Frame *f)
{
  LocalVariable *value = pop_operand(f->operands);
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));
  result->type = CONSTANT_Integer;
  result->value = -value->value;
#ifdef DEBUG
  printf("value: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

void lneg_eval(Frame *f)
{
  LocalVariable *value = pop_operand(f->operands);
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  long result_long = *(long *)&value->type_long;
  result_long = -result_long;
  result->type = CONSTANT_Long;
  result->type_long = *(uint64_t *)&result_long;
#ifdef DEBUG
  printf("type_long: %ld\n", result->type_long);
#endif
  push_operand(result, f->operands);
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
  int64_t v1, v2;
  double value1, value2, resultdouble;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = 0;
  v1 = pop_operand(f->operands)->type_double;
  value1 = *(double *)&v1;
  value2 = *(double *)&v2;
  // memcpy(&value1, &v1, sizeof(double));
  // memcpy(&value2, &v2, sizeof(double));
  resultdouble = value2 - value1;
  result->type = CONSTANT_Double;
  // result->type_double = convertDoubleToBytes(&resultdouble);
  //memcpy(&(result->type_double), &resultdouble, sizeof(uint64_t));
  // result->type_double = *(uint64_t *)&resultdouble;
  // result->type_double = *(uint64_t*)&((*(double *)&v1) + (*(double *)&v2));
  result->type_double = *(uint64_t *)&resultdouble;
#ifdef DEBUG
  printf("v1_double: %f \n", value1);
#endif
#ifdef DEBUG
  printf("v2_double: %f \n", value2);
  printf("result_neg: %f \n", resultdouble);
#endif
#ifdef DEBUG
  printf("resultado_double_neg: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
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
  uint64_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;

  long value1, value2, resultlong;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;
  resultlong = value1 << (value2 & 0x1F);

  result->type = CONSTANT_Long;
  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2: %ld \n", (value2 & 0x1F));
#endif
#ifdef DEBUG
  printf("resultado: %04x \n", result->type_long);
#endif
  push_operand(result, f->operands);
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
  int64_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;

  long value1, value2, resultlong;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;
  resultlong = value1 >> (value2 & 0x1F);

  result->type = CONSTANT_Long;
  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2: %ld \n", (value2 & 0x1F));
#endif
#ifdef DEBUG
  printf("resultado: %04x \n", result->type_long);
#endif
  push_operand(result, f->operands);
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
  uint64_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;

  long value1, value2, resultlong;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;
  resultlong = value1 >> (value2 & 0x1F);

  result->type = CONSTANT_Long;
  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2: %ld \n", (value2 & 0x1F));
#endif
#ifdef DEBUG
  printf("resultado: %04x \n", result->type_long);
#endif
  push_operand(result, f->operands);
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
  uint64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 & value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
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
  uint64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 | value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
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
  uint64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 ^ value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
#ifdef DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#ifdef DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#ifdef DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
}

void iinc_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  int32_t value = f->bytecode[f->pc++];
  f->local_variables[index].value += value;
}

void i2l_eval(Frame *f)
{
  long val = (long ) pop_operand(f->operands)->value;

  LocalVariable *long_val = (LocalVariable *)malloc(sizeof(LocalVariable));
  memcpy(&long_val->value, &val, sizeof(uint64_t));

  long_val->type = CONSTANT_Long;
  push_operand(long_val, f->operands);
  printf("VALOR CONVERTIDO EM LONG ====> %d\n", *(int *)&long_val->type_long);
}

void i2f_eval(Frame *f)
{
  float val = (float) pop_operand(f->operands)->value;

  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&float_val->value, &val, sizeof(uint32_t));

  float_val->type = CONSTANT_Float;
  push_operand(float_val, f->operands);
}

void i2d_eval(Frame *f)
{
  double val = (double) pop_operand(f->operands)->value;

  LocalVariable *double_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&double_val->value, &val, sizeof(uint64_t));

  double_val->type = CONSTANT_Double;
  push_operand(double_val, f->operands);
}

void l2i_eval(Frame *f)
{
  int val = (int) pop_operand(f->operands)->value;

  LocalVariable *int_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&int_val->value, &val, sizeof(uint32_t));

  int_val->type = CONSTANT_Integer;
  push_operand(int_val, f->operands);
}

void l2f_eval(Frame *f)
{
  float val = (float) pop_operand(f->operands)->value;

  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&float_val->value, &val, sizeof(uint32_t));

  float_val->type = CONSTANT_Float;
  push_operand(float_val, f->operands);
}

void l2d_eval(Frame *f)
{
  double val = (double) pop_operand(f->operands)->value;

  LocalVariable *double_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&double_val->value, &val, sizeof(uint64_t));

  double_val->type = CONSTANT_Double;
  push_operand(double_val, f->operands);
}

void f2i_eval(Frame *f)
{
  int val = (int) pop_operand(f->operands)->value;

  LocalVariable *int_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&int_val->value, &val, sizeof(uint32_t));

  int_val->type = CONSTANT_Integer;
  push_operand(int_val, f->operands);
}

void f2l_eval(Frame *f)
{
  long val = (long) pop_operand(f->operands)->value;

  LocalVariable *long_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&long_val->value, &val, sizeof(uint64_t));

  long_val->type = CONSTANT_Long;
  push_operand(long_val, f->operands);
}

void f2d_eval(Frame *f)
{
  double val = (double) pop_operand(f->operands)->value;

  LocalVariable *double_value = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&double_value->value, &val, sizeof(uint64_t));

  double_value->type = CONSTANT_Double;
  push_operand(double_value, f->operands);
}

void d2i_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_double);
  u4 r1;
  double aux;
  int aux_int;
  aux = *(double *)(&val);
  aux_int = (int)aux;
  
  #ifdef DEBUG
    printf("d2f_val: %d\n", aux_int);
  #endif
  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4*)(&aux_int));
  // memcpy(&float_val->value, &val, sizeof(uint32_t));

  float_val->type = CONSTANT_Integer;
  float_val->value = r1;
  push_operand(float_val, f->operands);
}

void d2l_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_double);
  u4 r1;
  double aux;
  long aux_long;
  aux = *(double *)(&val);
  aux_long = (long)aux;
  
  #ifdef DEBUG
    printf("d2l_val: %d\n", aux_long);
  #endif
  LocalVariable *long_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4*)(&aux_long));
  // memcpy(&float_val->value, &val, sizeof(ulong32_t));

  long_val->type = CONSTANT_Long;
  long_val->value = r1;
  push_operand(long_val, f->operands);
}

void d2f_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_double);
  u4 r1;
  double aux;
  float aux_float;
  aux = *(double *)(&val);
  aux_float = (float)aux;
  
  #ifdef DEBUG
    printf("d2f_val: %f\n", aux_float);
  #endif
  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4*)(&aux_float));
  // memcpy(&float_val->value, &val, sizeof(uint32_t));

  float_val->type = CONSTANT_Float;
  float_val->value = r1;
  push_operand(float_val, f->operands);
}

void i2b_eval(Frame *f)
{
  uint8_t val = (uint8_t) pop_operand(f->operands)->value;

  LocalVariable *byte_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&byte_val->value, &val, sizeof(uint32_t));

  byte_val->type = CONSTANT_Integer; // colocar o tipo CONSTANT_Byte
  push_operand(byte_val, f->operands);
}

void i2c_eval(Frame *f)
{
  uint8_t val = (uint8_t) pop_operand(f->operands)->value;

  LocalVariable *char_value = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&char_value->value, &val, sizeof(uint32_t));

  char_value->type = CONSTANT_Integer; // colocar o tipo CONSTANT_Char
  push_operand(char_value, f->operands);
}

void i2s_eval(Frame *f)
{
  uint32_t val = (uint32_t) pop_operand(f->operands)->value;

  LocalVariable *string_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&string_val->value, &val, sizeof(uint32_t));

  string_val->type = CONSTANT_String;
  push_operand(string_val, f->operands);
}

void lcmp_eval(Frame *f)
{
  int64_t v1, v2;
  long value1, value2;
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  lv->type = CONSTANT_Integer;
  // resultlong = value1 + value2;
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
  push_operand(lv, f->operands);
}

void fcmpl_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  int32_t value1, value2;
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
}

void fcmpg_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  u4 value1, value2;
  v2 = pop_operand(f->operands);
  v1 = pop_operand(f->operands);
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
  LocalVariable *v1, *v2, *lv;
  uint64_t value1, value2;
  v2 = pop_operand(f->operands);
  v1 = pop_operand(f->operands);
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  value1 = *(double *)&(v1->type_double);
  value2 = *(double *)&(v2->type_double);
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
}

void dcmpg_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  uint64_t value1, value2;
  v2 = pop_operand(f->operands);
  v1 = pop_operand(f->operands);
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  value1 = *(double *)&(v1->type_double);
  value2 = *(double *)&(v2->type_double);
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

void ifeq_eval(Frame *f)
{
  u4 v1 = pop_operand(f->operands)->value;
  #ifdef DEBUG
  printf("valor_ifeq: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #ifdef DEBUG
  printf("valor_ifeq_value: %d\n", value);
  #endif
  if (value == 0)
  {
    u1 branchbyte1, branchbyte2;
    branchbyte1 = f->bytecode[f->pc++];
    branchbyte2 = f->bytecode[f->pc++];

    int16_t offset = ((branchbyte1 << 8) | branchbyte2);

#ifdef DEBUG
    printf("ifeq: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

void ifne_eval(Frame *f)
{
  u4 v1 = pop_operand(f->operands)->value;
  #ifdef DEBUG
  printf("valor_ifne: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #ifdef DEBUG
  printf("valor_ifne_value: %d\n", value);
  #endif
  if (value != 0)
  {
    u1 branchbyte1, branchbyte2;
    branchbyte1 = f->bytecode[f->pc++];
    branchbyte2 = f->bytecode[f->pc++];

    int16_t offset = ((branchbyte1 << 8) | branchbyte2);

#ifdef DEBUG
    printf("ifne: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

void iflt_eval(Frame *f)
{
  u4 v1 = pop_operand(f->operands)->value;
  #ifdef DEBUG
  printf("valor_iflt: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #ifdef DEBUG
  printf("valor_iflt_value: %d\n", value);
  #endif
  if (value < 0)
  {
    u1 branchbyte1, branchbyte2;
    branchbyte1 = f->bytecode[f->pc++];
    branchbyte2 = f->bytecode[f->pc++];

    int16_t offset = ((branchbyte1 << 8) | branchbyte2);

#ifdef DEBUG
    printf("iflt: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

void ifge_eval(Frame *f)
{
  u4 v1 = pop_operand(f->operands)->value;
  #ifdef DEBUG
  printf("valor_ifge: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #ifdef DEBUG
  printf("valor_ifge_value: %d\n", value);
  #endif
  if (value >= 0)
  {
    u1 branchbyte1, branchbyte2;
    branchbyte1 = f->bytecode[f->pc++];
    branchbyte2 = f->bytecode[f->pc++];

    int16_t offset = ((branchbyte1 << 8) | branchbyte2);

#ifdef DEBUG
    printf("ifge: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

void ifgt_eval(Frame *f)
{
  u4 v1 = pop_operand(f->operands)->value;
  #ifdef DEBUG
  printf("valor_ifgt: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #ifdef DEBUG
  printf("valor_ifgt_value: %d\n", value);
  #endif
  if (value > 0)
  {
    u1 branchbyte1, branchbyte2;
    branchbyte1 = f->bytecode[f->pc++];
    branchbyte2 = f->bytecode[f->pc++];

    int16_t offset = ((branchbyte1 << 8) | branchbyte2);

#ifdef DEBUG
    printf("ifgt: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

void ifle_eval(Frame *f)
{
  u4 v1 = pop_operand(f->operands)->value;
  #ifdef DEBUG
  printf("valor_ifle: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #ifdef DEBUG
  printf("valor_ifle_value: %d\n", value);
  #endif
  if (value <= 0)
  {
    u1 branchbyte1, branchbyte2;
    branchbyte1 = f->bytecode[f->pc++];
    branchbyte2 = f->bytecode[f->pc++];

    int16_t offset = ((branchbyte1 << 8) | branchbyte2);

#ifdef DEBUG
    printf("ifle: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
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
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  u4 value2 = pop_operand(f->operands)->value;
  u4 value1 = pop_operand(f->operands)->value;
  if (value1 == value2)
  {
    f->pc += offset - 3;
  }
}

void if_acmpne_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  u4 value2 = pop_operand(f->operands)->value;
  u4 value1 = pop_operand(f->operands)->value;
  if (value1 != value2)
  {
    f->pc += offset - 3;
  }
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
  //TODO
}

void ret_eval(Frame *f)
{
  //TODO
}

void tableswitch_eval(Frame *f)
{
  //TODO
}

void lookupswitch_eval(Frame *f)
{
  u1 defaultbyte1, defaultbyte2, defaultbyte3, defaultbyte4;
  u4 aux_pc, bytes_padding, offset, pc_novo;
  u4 key = pop_operand(f->operands)->value;
  u1 *bytecode = f->method->attributes->info->Code_attribute.code;

  int definido = 0;
  int32_t pairs, match, default_v;

  aux_pc = f->pc;
  bytes_padding = (4 - ((aux_pc + 1) % 4) ) % 4;
  aux_pc += bytes_padding;
  aux_pc++;

   // pega bytes default 
  default_v = 0;
  for (int l = 0; l < 4; l++)
  {
      default_v = (default_v << 8) + bytecode[aux_pc];   
      aux_pc++;
  }  

  pairs = 0;
  for (int l = 0; l < 4; l++)
  {
    pairs = (pairs << 8) + bytecode[aux_pc++];
  }

    // itera pelo numero de pares
  for (int32_t l = 0; l < pairs; l++)
  {
      // pega match atual 
      match = 0;
      for (int l = 0; l < 4; l++)
      {
          match = (match << 8) + bytecode[aux_pc];   
          aux_pc++;
      }       
      
      // se a key corresponde ao match 
      if (key == match)
      {
          // pega offset
          offset = 0;
          for (int l = 0; l < 4; l++)
          {
              offset = (offset << 8) + bytecode[aux_pc];   
              aux_pc++;
          }       
          
          // poe valor correto em pc_novo
          pc_novo = f->pc + offset; 

          // set booleano que achou o match
          definido = 1;
      } else {
          // pula offset
          for(int i = 0; i < 4; i++)
          {
              aux_pc++;
          }
      }
    }

    if (!definido)
    {
        pc_novo = f->pc + default_v;
    }

    // poe valor correto no offset
    f->pc = pc_novo-1; 
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
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
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
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

void areturn_eval(Frame *f)
{
  //TODO
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

  // recupera Utf8 da referencia
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

  // Fieldref Name and type
  u2 name_n_type = f->cp[index - 1].Fieldref.name_and_type_index;

  char *field_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *field_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

#ifdef DEBUG
  // printf("field_name: %s\n", field_name);
  printf("field_desc: %s\n", field_desc);
#endif

  u2 class_index = find_class(class_name);
  field_info *field = find_field(Mem.classes_arr[class_index], field_name, field_desc);

  push_operand(field->static_data, f->operands);

  // if (strcmp(field_desc, "[I") == 0)
  // {
  //   #ifdef DEBUG
  //       printf("get static_data_low: %04x\n", *(GLOBAL_CLASS->fields->staticData->low));
  //   #endif
  //   lv->type_array.array = (u4 *)GLOBAL_CLASS->fields->staticData->low;
  //   lv->type = CONSTANT_Fieldref;
  // } else if((field_desc[0] == '[' ) && (field_desc[1] == 'L' )){
  //   lv->type_array.array = (u4 *)GLOBAL_CLASS->fields->staticData->low;
  //   lv->type = CONSTANT_Fieldref;
  //   #ifdef DEBUG
  //       printf("getstatic field_desc: %s\n", field_desc);
  //   #endif
  // }
  // push_operand(lv, f->operands);
}

void putstatic_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);

  char *class_name = ret_method_name(f->cp, index);

  // Fieldref Name and type
  u2 name_n_type = f->cp[index - 1].Fieldref.name_and_type_index;

  char *field_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *field_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

#ifdef DEBUG
  printf("field_name: %s\n", field_name);
  printf("field_desc: %s\n", field_desc);
#endif

  LocalVariable *lv = pop_operand(f->operands);
  // lv = f->operands->top;

#ifdef DEBUG
  printf("put lv_putstatic: %04x\n", lv->value);
#endif

  u2 class_index = find_class(class_name);
  field_info *field = find_field(Mem.classes_arr[class_index], field_name, field_desc);

  field->static_data = lv;

  // if (field_desc[0] == '[') {
  //       // field->staticValue.tag = CAT1;
  //       // field->staticValue.type_char = frame->operandStack.top().type_char;
  //       // frame->operandStack.pop();
  //   }
  //   else if (fieldDescriptor.compare("I") == 0) {
  //       field->staticValue.tag = CAT1;
  //       field->staticValue.type_int = frame->operandStack.top().type_int;
  //       frame->operandStack.pop();
  //   }
  //   else if (fieldDescriptor.compare("F") == 0) {
  //       field->staticValue.tag = CAT1;
  //       field->staticValue.type_float = frame->operandStack.top().type_float;
  //       frame->operandStack.pop();
  //   }
  //   else if (fieldDescriptor.compare("D") == 0) {
  //       field->staticValue.tag = CAT2;
  //       field->staticValue.type_double = frame->operandStack.top().type_double;
  //       frame->operandStack.pop();
  //   }
  //   else if (fieldDescriptor.compare("J") == 0) {
  //       field->staticValue.tag = CAT2;
  //       field->staticValue.type_long = frame->operandStack.top().type_long;
  //       frame->operandStack.pop();
  //   }
  //   else if (fieldDescriptor.compare("Z") == 0) {
  //       field->staticValue.tag = CAT1;
  //       field->staticValue.type_boolean = frame->operandStack.top().type_boolean;
  //       frame->operandStack.pop();
  //   }
  //   else if (fieldDescriptor[0] == '[') {
  //       field->staticValue.tag = CAT1;
  //       field->staticValue.type_reference = frame->operandStack.top().type_reference;
  //       frame->operandStack.pop();
  //   }
  //   else {
  //       printf("putstatic: tipo do descritor nao reconhecido: %s\n", fieldDescriptor.c_str());
  //       exit(0);
  //   }

  //   if (strcmp(field_desc, "[I") == 0)
  //   {
  //     GLOBAL_CLASS->fields->staticData = (staticData *)malloc(sizeof(staticData));
  //     GLOBAL_CLASS->fields->staticData->low = (u4 *)malloc(sizeof(u4));
  //     GLOBAL_CLASS->fields->staticData->high = NULL;
  //     GLOBAL_CLASS->fields->staticData->low = &(lv->value);

  // #ifdef DEBUG
  //     printf("put static_data_low: %04x\n", *(GLOBAL_CLASS->fields->staticData->low));
  // #endif
  //   }
  //   else if (strcmp(field_desc, "I") == 0)
  //   {
  //     GLOBAL_CLASS->fields->staticData = (staticData *)malloc(sizeof(staticData));
  //     GLOBAL_CLASS->fields->staticData->low = (u4 *)malloc(sizeof(u4));
  //     GLOBAL_CLASS->fields->staticData->high = NULL;
  //     GLOBAL_CLASS->fields->staticData->low[0] = lv->value;

  // #ifdef DEBUG
  //     printf("put static_data_low: %04x\n", GLOBAL_CLASS->fields->staticData->low[0]);
  // #endif
  //   }
}

void getfield_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  LocalVariable *lv = pop_operand(f->operands);

  // Fieldref Name and type
  u2 name_n_type = f->cp[index - 1].Fieldref.name_and_type_index;

  char *field_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *field_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

#ifdef DEBUG
  printf("get field name: %s\n", field_name);
  printf("get field desc: %s\n", field_desc);
#endif

  ClassFile *cf = Mem.classes_arr[lv->value];

  field_info *field = find_field(cf, field_name, field_desc);

  lv->type = CONSTANT_Fieldref;
  lv->value = *(u4 *)&field;

  // EMpilha referencia para o field
  push_operand(lv, f->operands);
}

void putfield_eval(Frame *f)
{
  //TODO
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
  u2 name_n_type = f->cp[index - 1].Methodref.name_and_type_index;

  char *method_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *method_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

  if (strcmp(class_name, "java/io/PrintStream") == 0)
  {
    if (strcmp(method_name, "println") == 0 || strcmp(method_name, "print"))
    {
      LocalVariable *lv = pop_operand(f->operands);
      if (strcmp(method_desc, "(Ljava/lang/String;)V") == 0) // String
      {
        char *string = readUtf8(f->cp, lv->value);
        printf("%s ", string);
      }
      else if (strcmp(method_desc, "(I)V") == 0) // Int
      {
        // printf("PASSEI POR AQUI DE MOTO\n");
        int32_t value = lv->value;
        printf("%d ", value);
      }
      else if (strcmp(method_desc, "(Z)V") == 0) // Bool
      {
        int32_t value = lv->value;
        printf("%s ", value ? "True" : "False");
      }
      else if (strcmp(method_desc, "(F)V") == 0) // Float
      {
        int32_t value = lv->value;
        printf("%.2f ", *(float *)&value);
      }
      else if (strcmp(method_desc, "(J)V") == 0) // Long
      {
        int64_t value = lv->type_long;
        printf("%ld ", *(long *)&value);
      }
      else if (strcmp(method_desc, "(D)V") == 0) // Double
      {
        int64_t value = lv->type_double;
        printf("%.4f ", *(double *)&value);
      }
      else if (strcmp(method_desc, "()V") == 0) // Double
      {
        printf("\n");
      }
      else
      {
        printf("invokevirtual tipo nao encontrado \n");
        exit(0);
      }
      if (strcmp(method_name, "println") == 0)
        printf("\n");
    }
    else
    {
      printf("invokevirtual so existe print aqui \n");
      exit(0);
    }
  }
  else
  {
    u2 args = count_args(method_desc);
    u2 class_index = find_class(class_name);
    method_info *method = find_method(Mem.classes_arr[class_index], method_name);
    Frame *frame = cria_frame(f->cp, method);
    // Adiciona argumestos comeca de 1
    // 0 é uma referencia
    // for (size_t i = 0; i <= args; i++)
    for (int8_t i = args; i >= 0; i--)
    {
      frame->local_variables[i] = *(pop_operand(f->operands));
#ifdef DEBUG
      printf("DEBUG DE VERDADE:  ==== %04x\n", frame->local_variables[i].value);
#endif
    }

    push(frame);
  }
}

void invokespecial_eval(Frame *f)
{
#ifdef DEBUG
  printf("Invocando satanás...\n");
#endif
  // recupera Utf8 da referencia
  u2 index = getIndexFromb1b2(f);
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

    // ClassLoader(class_name);
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

  // Pega referencia da classe
  u2 class_index = find_class(class_name);

  // Pega o nome e tipo dó método pelo indice da instrução.
  // Method Name and type
  u2 name_n_type = f->cp[index - 1].Methodref.name_and_type_index;

  char *method_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *method_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

  u2 args = count_args(method_desc);
  method_info *method = find_method(Mem.classes_arr[class_index], method_name);
  Frame *frame = cria_frame(f->cp, method);
  // Adiciona argumestos
  // for (size_t i = args - 1; i >= 0; i--)
  // for (size_t i = 0; i < args; i++)
  for (int8_t i = args - 1; i >= 0; i--)
  {
    frame->local_variables[i] = *(pop_operand(f->operands));
#ifdef DEBUG
    printf("DEBUG DE VERDADE:  ==== %04x\n", frame->local_variables[i].value);
#endif
  }

  push(frame);
}

void invokestatic_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  // Method Name and type
  uint16_t name_n_type = f->cp[index - 1].Methodref.name_and_type_index;

  char *method_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *method_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

  char *class_name = ret_method_name(f->cp, index);

  if (strcmp(class_name, "java/lang/Object") == 0)
  {
    printf("Nao implementado ! \n");
    return;
  }

  u2 args = count_args(method_desc);

#ifdef DEBUG
  printf("Argumentos %d", args);
#endif

  u2 class_index = find_class(class_name);
  method_info *method = find_method(Mem.classes_arr[class_index], method_name);
  Frame *frame = cria_frame(f->cp, method);
  // Adiciona argumestos
  // for (size_t i = 0; i < args - 1; i++)
  for (int8_t i = args - 1; i >= 0; i--)
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
  //TODO
}

void invokedynamic_eval(Frame *f)
{
  //TODO
}

void new_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  char *class_name = ret_method_name(f->cp, index);
  u2 class_index = find_class(class_name);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Class;
  lv->value = class_index;
  // Empilha referencia para a classe no array de classes;
  push_operand(lv, f->operands);
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
      arrayref = (u4 *)malloc((count) * sizeof(u4));
      // int return_array[count];
      break;
    case T_CHAR:
      rlv->type = CONSTANT_String;
      arrayref = (u1 *)malloc((count) * sizeof(u1));
      // char return_array[count];
      break;
    case T_FLOAT:
      rlv->type = CONSTANT_Float;
      arrayref = (u4 *)malloc((count) * sizeof(u4));
      // float return_array[count];
      break;
    case T_DOUBLE:
      rlv->type = CONSTANT_Double;
      arrayref = (uint64_t *)malloc((count) * sizeof(uint64_t));
      // double return_array[count];
      break;
    case T_BYTE:
      rlv->type = CONSTANT_Integer;
      arrayref = (u1 *)malloc((count) * sizeof(u1));
      // u1 return_array[count];
      break;
    case T_SHORT:
      rlv->type = CONSTANT_Integer;
      arrayref = (u2 *)malloc((count) * sizeof(u2));
      // u2 return_array[count];
      break;
    case T_INT:
      rlv->type = CONSTANT_Integer;
      arrayref = (u4 *)malloc((count) * sizeof(u4));
#ifdef DEBUG
      printf("array ref: %x \n", (u4)arrayref);
#endif
      break;
    case T_LONG:
      arrayref = (uint64_t *)malloc((count) * sizeof(uint64_t));
      // long return_array[count];
      break;
    default:
      break;
    }
    rlv->type_array.array = (u4 *)arrayref;
    rlv->type_array.size = count;
#ifdef DEBUG
    printf("array type: %x \n", (u4)(rlv->type_array.array));
    printf("rlv type: %x \n", rlv->type);
#endif

    push_operand(rlv, f->operands);
  }
}

void anewarray_eval(Frame *f)
{
  LocalVariable *lv, *rlv;
  lv = pop_operand(f->operands);
  u2 index = getIndexFromb1b2(f);
  u4 count;
  count = lv->value;
  void *arrayref = NULL;
  rlv = (LocalVariable *)malloc(sizeof(LocalVariable));
  u2 name_index = f->cp[index - 1].Class.name_index;

  rlv->type = CONSTANT_Fieldref;

  arrayref = (u4 *)malloc((count) * sizeof(u4));
  rlv->value = *((u4 *)(arrayref));

#ifdef DEBUG
  printf("arrayref: %04x\n", rlv->value);
  printf("classname_index: %02x\n", name_index);
#endif

  if (count < 0)
  {
    printf("NegativeArraySizeException.\n");
  }
  else
  {
    // rlv->type_array.array = arrayref;

    push_operand(rlv, f->operands);
  }
}

void arraylength_eval(Frame *f)
{
  LocalVariable *arrayref;
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  arrayref = pop_operand(f->operands);

  lv->value = arrayref->type_array.size;
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

void athrow_eval(Frame *f)
{
  //TODO
}

void checkcast_eval(Frame *f)
{
  //TODO
}

void instanceof_eval(Frame *f)
{
  //TODO
}

void monitorenter_eval(Frame *f)
{
  //TODO
}

void monitorexit_eval(Frame *f)
{
  //TODO
}

void wide_eval(Frame *f)
{
  //TODO
}

void multianewarray_eval(Frame *f)
{
  //TODO
}

void ifnull_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  if (!lv->value)
  {
    f->pc += offset - 3;
  }
}

void ifnonnull_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  if (lv->value)
  {
    f->pc += offset - 3;
  }
}

void goto_w_eval(Frame *f)
{
  //TODO
}

void jsr_w_eval(Frame *f)
{
  //TODO
}

void breakpoint_eval(Frame *f)
{
  //TODO
}

void impdep1_eval(Frame *f)
{
  //TODO
}

void impdep2_eval(Frame *f)
{
  //TODO
}
