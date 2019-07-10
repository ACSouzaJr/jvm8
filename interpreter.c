/**
* @file         interpreter.c
* @brief        Implementação das instruções de execução da JVM.
* @detail       Contém a implementação de todas as instruções da JVM.
*/

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
#define hex_0 0x00000000

/**
 * @brief Conta o número de argumentos do método recebido como parâmetro.
 */
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
    if (method_desc[i] == 'D' || method_desc[i] == 'J')
    {
    	args++;
    }
    args++;
  }
  return args;
}

/**
 * @brief Implementação da instrução nop, que não faz nada.
 */
void nop_eval(Frame *f)
{
}

/**
 * @brief Realiza um push NULL na pilha de operandos.
 */
void aconst_null_eval(Frame *f)
{
  push_operand(NULL, f->operands);
}

/**
 * @brief Realiza um push de um int constante para a pilha de operandos.
 */
void iconst_m1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = -1;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca o inteiro constante 0 na pilha de operandos.
 */
void iconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 0;
  push_operand(lv, f->operands);
#if defined DEBUG
  printf("Iconst_0");
#endif
}

/**
 * @brief Coloca o inteiro constante 1 na pilha de operandos.
 */
void iconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 1;
  push_operand(lv, f->operands);
#if defined DEBUG
  printf("Iconst_1");
#endif
}

/**
 * @brief Coloca o inteiro constante 2 na pilha de operandos.
 */
void iconst_2_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 2;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca o inteiro constante 3 na pilha de operandos.
 */
void iconst_3_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 3;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca o inteiro constante 4 na pilha de operandos.
 */
void iconst_4_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 4;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca o inteiro constante 5 na pilha de operandos.
 */
void iconst_5_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 5;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca um long de valor constante 0 no topo da pilha de operandos.
 */
void lconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  long value = 0;
  lv->type_long = *(uint64_t *)&value;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca um long de valor constante 1 no topo da pilha de operandos.
 */
void lconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Long;
  long value = 1;
  lv->type_long = *(uint64_t *)&value;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca um float constante 0 no topo da pilha de operandos.
 */
void fconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  float value = 0.0;
  lv->value = *(u4 *)&value;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca um float constante 1 no topo da pilha de operandos.
 */
void fconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  float value = 1.0;
  lv->value = *(u4 *)&value;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca um float constante 2 no topo da pilha de operandos.
 */
void fconst_2_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Float;
  float value = 2.0;
  lv->value = *(u4 *)&value;
  push_operand(lv, f->operands);
}

/**
 * @brief Coloca um double constante 0 no topo da pilha de operandos.
 */
void dconst_0_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  double aux = 0.0;
  lv->type = CONSTANT_Double;
  lv->type_double = *(uint64_t *)&(aux);
  push_operand(lv, f->operands);
#if defined DEBUG
  printf("dconst_0: %f\n", *(double *)&(lv->type_double));
#endif
}

/**
 * @brief Coloca um double constante 1 no topo da pilha de operandos.
 */
void dconst_1_eval(Frame *f)
{
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  double aux = 1.0;
  lv->type = CONSTANT_Double;
  lv->type_double = *(uint64_t *)&(aux);
  push_operand(lv, f->operands);
#if defined DEBUG
  printf("dconst_1: %f\n", *(double *)&(lv->type_double));
#endif
}

/**
 * @brief Coloca um byte na pilha de operandos.
 *
 * Faz um push na pilha de operandos de um byte. O byte tem seu sinal extendido para um valor int.
 */
void bipush_eval(Frame *f)
{
  // Pega o byte de argument extend para int e empilha nos operandos.
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = (int32_t)((int8_t)(f->bytecode[f->pc++]));
#if defined DEBUG
  printf("bipush: %d \n", lv->value);
#endif
  push_operand(lv, f->operands);
}

/**
 * @brief Faz um push na pilha de operandos de um valor do tipo short.
 *
 * Recebe dois bytes e os junta em um intermediário de 16 bits, para futuramente
 * extender seu sinal e dar push para a pilha de operandos.
 */
void sipush_eval(Frame *f)
{
  u1 byte1, byte2;
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  u1 *bytecode = f->method->attributes->info->Code_attribute.code;
  byte1 = bytecode[f->pc++];
  byte2 = bytecode[f->pc++];

  int16_t val_sipush = (((int16_t)(int8_t)byte1 << 8) | byte2);
  lv->value = (uint32_t)val_sipush;
  lv->type = CONSTANT_Integer;

  #if defined DEBUG
  printf("sipush: %d", val_sipush);
  #endif

  push_operand(lv, f->operands);
}

/**
 * @brief Realiza o push de um item da constant pool.
 *
 * O índice de acesso a constant pool deve ser de categoria 1.
 * Caso seja int ou float, seu valor é levado à cp como int, ou float
 * Caso seja String, então a referência value para aquela instância é levada a op_stack
 * Caso seja uma referência para uma classe, é retornada uma referência para a class Object [0]
 */
void ldc_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  cp_info *item = &(f->cp[index - 1]);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
#if defined DEBUG
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

/**
 * @brief Faz um push na pilha de operandos de um long ou double da constant pool.
 *
 * Recebe dois bytes e os junta em um intermediário de 16 bits, os quais são
 * usados como índice na constant pool. O valor numérico naquela posição da constant pool
 * é colocado na pilha de operandos.
 */
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
#if defined DEBUG
    printf("low: %04x, high: %04x\n", low, high);
#endif
    total = (int64_t)high << 32 | (int64_t)low;
#if defined DEBUG
    printf("total: %04x \n", total);
    printf("total: %f \n", *(double *)&total);
#endif
    lv->type = CONSTANT_Double;
    lv->type_double = total;
    break;
  case CONSTANT_Long:
    low = f->cp[index - 1].Long.low_bytes;
    high = f->cp[index - 1].Long.high_bytes;
#if defined DEBUG
    printf("low: %04x, high: %04x\n", low, high);
#endif
    total = (int64_t)high << 32 | (int64_t)low;
#if defined DEBUG
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

/**
 * @brief Carrega um inteiro vindo do vetor de variáveis locais - índice sem sinal.
 */
void iload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]), f->operands);
#if defined DEBUG
  printf("iload %d", f->local_variables[index].value);
#endif
}

/**
 * @brief Load de um long vindo do vetor de variáveis locais - índice sem sinal.
 */
void lload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]), f->operands);
}

/**
 * @brief Carrega um float vindo do vetor de variáveis locais - índice sem sinal.
 */
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

/**
 * @brief Carrega um double vindo do vetor de variáveis locais - índice sem sinal.
 */
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

/**
 * @brief Carrega uma referência de uma variável local.
 *
 * Usa como parâmetro um índice que é usado no vetor de variáveis locais, para colocar
 * na pilha de operandos a referência correspondente aquele índice.
 */
void aload_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  push_operand(&(f->local_variables[index]), f->operands);
#if defined DEBUG
  printf("aload %d", f->local_variables[index].value);
#endif
}

/**
 * @brief Carrega um inteiro correspondente a posição 0 no vetor de variáveis locais na pilha de operandos.
 */
void iload_0_eval(Frame *f)
{
  push_operand(&(f->local_variables[0]), f->operands);
}

/**
 * @brief Carrega um inteiro correspondente a posição 1 no vetor de variáveis locais na pilha de operandos.
 */
void iload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]), f->operands);
#if defined DEBUG
  printf("iload_1");
#endif
}

/**
 * @brief Carrega um inteiro correspondente a posição 2 no vetor de variáveis locais na pilha de operandos.
 */
void iload_2_eval(Frame *f)
{
  push_operand(&(f->local_variables[2]), f->operands);
}

/**
 * @brief Carrega a referência correspondente a posição 3 no vetor de variáveis locais na pilha de operandos.
 */
void iload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]), f->operands);
}

/**
 * @brief Carrega um long correspondente ao índice 0 no vetor de variáveis locais na pilha de operandos.
 */
void lload_0_eval(Frame *f)
{
  push_operand(&(f->local_variables[0]), f->operands);
}

/**
 * @brief Carrega um long correspondente ao índice 1 no vetor de variáveis locais na pilha de operandos.
 */
void lload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]), f->operands);
}

/**
 * @brief Carrega um long correspondente ao índice 2 no vetor de variáveis locais na pilha de operandos.
 */
void lload_2_eval(Frame *f)
{
  // if (f->local_variables[2-1].type == CONSTANT_Long)
  // {
  //   push_operand(&(f->local_variables[2-1]), f->operands);
  // } else 
  if(f->local_variables[2].type == CONSTANT_Long)
  {
    push_operand(&(f->local_variables[2]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

/**
 * @brief Carrega um long correspondente ao índice 3 no vetor de variáveis locais na pilha de operandos.
 */
void lload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]), f->operands);
}

/**
 * @brief Carrega um float correspondente ao índice 0 no vetor de variáveis locais na pilha de operandos.
 */
void fload_0_eval(Frame *f)
{
  #if defined DEBUG
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

/**
 * @brief Carrega um float correspondente ao índice 1 no vetor de variáveis locais na pilha de operandos.
 */
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

/**
 * @brief Carrega um float correspondente ao índice 2 no vetor de variáveis locais na pilha de operandos.
 */
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

/**
 * @brief Carrega um float correspondente ao índice 3 no vetor de variáveis locais na pilha de operandos.
 */
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

/**
 * @brief Carrega um double correspondente ao índice 0 no vetor de variáveis locais na pilha de operandos.
 */
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

/**
 * @brief Carrega um double correspondente ao índice 1 no vetor de variáveis locais na pilha de operandos.
 */
void dload_1_eval(Frame *f)
{
  if (f->local_variables[1].type == CONSTANT_Double)
  {
    #if defined DEBUG
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

/**
 * @brief Carrega um double correspondente ao índice 2 no vetor de variáveis locais na pilha de operandos.
 */
void dload_2_eval(Frame *f)
{
  #if defined DEBUG
    printf("dload_2_type: %d\n", f->local_variables[2].type);
  #endif
  // if (f->local_variables[2-1].type == CONSTANT_Double)
  // {
  //   push_operand(&(f->local_variables[2-1]), f->operands);
  // } else 
  if(f->local_variables[2].type == CONSTANT_Double)
  {
    push_operand(&(f->local_variables[2]), f->operands);
  } else {
    printf("javax.persistence.PersistenceException\n");
    exit(0);
  }
}

/**
 * @brief Carrega um double correspondente ao índice 3 no vetor de variáveis locais na pilha de operandos.
 */
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

/**
 * @brief Carrega uma referência correspondente ao índice 0 no vetor de variáveis locais na pilha de operandos.
 */
void aload_0_eval(Frame *f)
{
#if defined DEBUG
// printf("lvv: %02x\n", f->local_variables[0].value);
#endif
  push_operand(&(f->local_variables[0]), f->operands);
}

/**
 * @brief Carrega uma referência correspondente ao índice 1 no vetor de variáveis locais na pilha de operandos.
 */
void aload_1_eval(Frame *f)
{
  push_operand(&(f->local_variables[1]), f->operands);
}

/**
 * @brief Carrega uma referência correspondente ao índice 2 no vetor de variáveis locais na pilha de operandos.
 */
void aload_2_eval(Frame *f)
{
  push_operand(&(f->local_variables[2]), f->operands);
}

/**
 * @brief Carrega uma referência correspondente ao índice 3 no vetor de variáveis locais na pilha de operandos.
 */
void aload_3_eval(Frame *f)
{
  push_operand(&(f->local_variables[3]), f->operands);
}

/**
 * @brief Carrega um int de um array. O arrayref deve ser do tipo reference, e deve se referir a um array
 * cujos objetos sejam do tipo int.
 */
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

/**
 * @brief Carrega um long de um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo long
 * no array.
 */
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

/**
 * @brief Carrega um float de um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo float
 * no array.
 */
void faload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  u4 *vetor;
  vetor = (u4 *)arrayref->type_array.array;
  lv->value = vetor[index->value];
  lv->type = CONSTANT_Float;

  push_operand(lv, f->operands);
}

/**
 * @brief Carrega um double de um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo double
 * no array.
 */
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

/**
 * @brief Carrega uma referência de um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo referência
 * no array.
 */
void aaload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  lv->type_object = ((Object *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Class;

  push_operand(lv, f->operands);
}

/**
 * @brief Carrega um byte ou booleano de um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo byte ou
 * booleano no array.
 */
void baload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->value = (int8_t)((u1 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

/**
 * @brief Carrega um char um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo char
 * no array.
 */
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

/**
 * @brief Carrega um short de um array.
 *
 * O arrayref e o índice são tirados da pilha de operandos e usados para acessar um valor do tipo short
 * no array.
 */
void saload_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *lv;
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);
  
  lv->value = (int16_t) ((u2 *)arrayref->type_array.array)[index->value];
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

/**
 * @brief Recupera um valor inteiro da pilha de operando e guarda esse operando na pilha de variáveis locais.
 */
void istore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  LocalVariable *aux = pop_operand(f->operands);
  f->local_variables[index] = *aux;
#if defined DEBUG
  printf("istore %d val: %04x\n", index, f->local_variables[index].value);
#endif
}


/**
 * @brief Recupera um valor long da pilha de operando e guarda esse operando na pilha de variáveis locais.
 */
void lstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[index] = *lv;
#if defined DEBUG
  printf("lstore val: %04lx\n", f->local_variables[index].type_long);
#endif
}

/**
 * @brief Recupera um valor float da pilha de operando e guarda esse operando na pilha de variáveis locais.
 */
void fstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[index] = *aux_linha;
#if defined DEBUG
  printf("fstore_0 val: %04x\n", f->local_variables[index].value);
#endif
}

/**
 * @brief Recupera um valor double da pilha de operando e guarda esse operando na pilha de variáveis locais.
 */
void dstore_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];

  LocalVariable *aux;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->type_double);
#endif
  f->local_variables[index] = *aux;
#if defined DEBUG
  printf("dstore val: %f\n", f->local_variables[index].type_double);
#endif
}

/**
 * @brief Recupera uma referência da pilha de operando e guarda esse operando na pilha de variáveis locais.
 */
void astore_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  u1 index = f->bytecode[f->pc++];
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[index] = *aux;
#if defined DEBUG
  printf("astore val: %04x\n", f->local_variables[index].value);
#endif
}

/**
 * @brief Guarda um inteiro na posição 0 do vetor de variáveis locais.
 */
void istore_0_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[0] = *aux;
#if defined DEBUG
  printf("istore_0 val: %04x\n", f->local_variables[0].value);
#endif
}

/**
 * @brief Guarda um inteiro na posição 1 do vetor de variáveis locais.
 */
void istore_1_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[1] = *aux;
#if defined DEBUG
  printf("istore_1 val: %04x\n", f->local_variables[1].value);
#endif
}

/**
 * @brief Guarda um inteiro na posição 2 do vetor de variáveis locais.
 */
void istore_2_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[2] = *aux;
#if defined DEBUG
  printf("istore_2 val: %04x\n", f->local_variables[2].value);
#endif
}

/**
 * @brief Guarda um inteiro na posição 3 do vetor de variáveis locais.
 */
void istore_3_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
  f->local_variables[3] = *aux;
#if defined DEBUG
  printf("istore_3 val: %04x\n", f->local_variables[3].value);
#endif
}

/**
 * @brief Guarda um long na posição 0 do vetor de variáveis locais.
 */
void lstore_0_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[0] = *lv;
#if defined DEBUG
  printf("lstore_0 val: %04llx\n", f->local_variables[0].type_long);
#endif
}

/**
 * @brief Guarda um long na posição 1 do vetor de variáveis locais.
 */
void lstore_1_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[1] = *lv;
#if defined DEBUG
  printf("lstore_1 val: %04llx\n", f->local_variables[1].type_long);
#endif
}

/**
 * @brief Guarda um long na posição 2 do vetor de variáveis locais.
 */
void lstore_2_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[2] = *lv;
#if defined DEBUG
  printf("lstore_2 val: %04llx\n", f->local_variables[2].type_long);
#endif
}

/**
 * @brief Guarda um long na posição 3 do vetor de variáveis locais.
 */
void lstore_3_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  f->local_variables[3] = *lv;
#if defined DEBUG
  printf("lstore_3 val: %04llx\n", f->local_variables[3].type_long);
#endif
}

/**
 * @brief Guarda um float na posição 0 do vetor de variáveis locais.
 */
void fstore_0_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[0] = *aux_linha;
#if defined DEBUG
  printf("fstore_0 val: %04x\n", f->local_variables[0].value);
#endif
}

/**
 * @brief Guarda um float na posição 1 do vetor de variáveis locais.
 */
void fstore_1_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[1] = *aux_linha;
#if defined DEBUG
  printf("fstore_1 val: %04x\n", f->local_variables[1].value);
#endif
}

/**
 * @brief Guarda um float na posição 2 do vetor de variáveis locais.
 */
void fstore_2_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[2] = *aux_linha;
#if defined DEBUG
  printf("fstore_2 val: %04x\n", f->local_variables[2].value);
#endif
}

/**
 * @brief Guarda um float na posição 3 do vetor de variáveis locais.
 */
void fstore_3_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux_linha->value = aux->value;
  aux_linha->type = CONSTANT_Float;
  f->local_variables[3] = *aux_linha;
#if defined DEBUG
  printf("fstore_3 val: %04x\n", f->local_variables[3].value);
#endif
}

/**
 * @brief Guarda um double na posição 0 do vetor de variáveis locais.
 */
void dstore_0_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[0] = *aux_linha;
#if defined DEBUG
  printf("dstore_0 val: %08x\n", f->local_variables[0].type_double);
#endif
}

/**
 * @brief Guarda um double na posição 1 do vetor de variáveis locais.
 */
void dstore_1_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[1] = *aux_linha;
#if defined DEBUG
  printf("dstore_1 val: %08x\n", f->local_variables[1].type_double);
#endif
}

/**
 * @brief Guarda um double na posição 2 do vetor de variáveis locais.
 */
void dstore_2_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[2] = *aux_linha;
#if defined DEBUG
  printf("dstore_2 val: %08x\n", f->local_variables[2].type_double);
#endif
}

/**
 * @brief Guarda um double na posição 3 do vetor de variáveis locais.
 */
void dstore_3_eval(Frame *f)
{
  LocalVariable *aux, *aux_linha;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %08x\n", aux->type_double);
#endif
  aux_linha = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux_linha->type_double = aux->type_double;
  aux_linha->type = CONSTANT_Double;
  f->local_variables[3] = *aux_linha;
#if defined DEBUG
  printf("dstore_3 val: %08x\n", f->local_variables[3].type_double);
#endif
}

/**
 * @brief Guarda uma referência na posição 0 do vetor de variáveis locais.
 */
void astore_0_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[0] = *aux;
#if defined DEBUG
  printf("astore_0 val: %04x\n", f->local_variables[0].value);
#endif
}

/**
 * @brief Guarda uma referência na posição 0 do vetor de variáveis locais.
 */
void astore_1_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[1] = *aux;
#if defined DEBUG
  printf("astore_1 val: %04x\n", f->local_variables[1].value);
#endif
}

/**
 * @brief Guarda uma referência na posição 1 do vetor de variáveis locais.
 */
void astore_2_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[2] = *aux;
#if defined DEBUG
  printf("astore_2 val: %04x\n", f->local_variables[2].value);
#endif
}

/**
 * @brief Guarda uma referência na posição 2 do vetor de variáveis locais.
 */
void astore_3_eval(Frame *f)
{
  LocalVariable *aux;
  aux = pop_operand(f->operands);
#if defined DEBUG
  printf("aux: %04x\n", aux->value);
#endif
  f->local_variables[3] = *aux;
#if defined DEBUG
  printf("astore_3 val: %04x\n", f->local_variables[3].value);
#endif
}

/**
 * @brief Guarda uma referência na posição 3 do vetor de variáveis locais.
 */
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
#if defined DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo long.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo long.
 */
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
#if defined DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo float.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo float.
 */
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
#if defined DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo double.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo double.
 */
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
#if defined DEBUG
  printf("Referencia array: %d", ((uint64_t *)arrayref->type_array.array)[index->value]);
#endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo reference.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo reference.
 */
void aastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  Object *vetor;
  vetor = (Object *)arrayref->type_array.array;
  vetor[index->value] = value->type_object;
#if defined DEBUG
  printf("Referencia array: %d", ((u4 *)arrayref->type_array.array)[index->value]);
#endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo byte ou boolean.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo byte ou boolean.
 */
void bastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u1 *vetor;
  vetor = (u1 *)arrayref->type_array.array;
  vetor[index->value] = (int8_t) value->value;
  #if defined DEBUG
    printf("Referencia array: %d", ((u1 *)arrayref->type_array.array)[index->value]);
  #endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo char.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo char.
 */
void castore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u1 *vetor;
  vetor = (u1 *)arrayref->type_array.array;
  vetor[index->value] = value->value;
  #if defined DEBUG
    printf("Referencia array: %d", ((u1 *)arrayref->type_array.array)[index->value]);
  #endif
}

/**
 * @brief Guarda um valor da pilha de operandos num array do tipo short.
 *
 * O arrayref deve ser do tipo reference e deve referir a um array cujos elementos sejam variáveis do tipo short.
 */
void sastore_eval(Frame *f)
{
  LocalVariable *arrayref, *index, *value;
  value = pop_operand(f->operands);
  index = pop_operand(f->operands);
  arrayref = pop_operand(f->operands);

  u2 *vetor;
  vetor = (u2 *)arrayref->type_array.array;
  vetor[index->value] = (int16_t) value->value;
  #if defined DEBUG
    printf("Referencia array: %d", ((u2 *)arrayref->type_array.array)[index->value]);
  #endif
}

/**
 * @brief Realiza a operação pop() na pilha de operandos.
 *
 * Só pode ser usado para int -> cat1
 */
void pop_eval(Frame *f)
{
  pop_operand(f->operands);
}

/**
 * @brief Realiza a operação pop() na pilha de operandos.
 *
 * Só pode ser usado para long/double -> cat2
 */
void pop2_eval(Frame *f)
{
  pop_operand(f->operands);
}

/**
 * @brief Duplica o valor no topo da pilha de operandos.
 */
void dup_eval(Frame *f)
{
  // duplica o topo da pilha
  LocalVariable *dup_top = pop_operand(f->operands);

  push_operand(dup_top, f->operands);
  push_operand(dup_top, f->operands);
}

/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois valores abaixo.
 *
 * Só usado para int -> cat1
 */
void dup_x1_eval(Frame *f)
{
  LocalVariable *dup_top1 = pop_operand(f->operands);
  LocalVariable *dup_top2 = pop_operand(f->operands);

  push_operand(dup_top1, f->operands);
  push_operand(dup_top2, f->operands);
  push_operand(dup_top1, f->operands);
}

/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois ou três valores abaixo.
 */
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

/**
 * @brief Duplica os primeiro ou dois primeiros valores no topo da pilha de operandos.
 */
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

/**
 * @brief Duplica os primeiro ou dois primeiros valores no topo da pilha de operandos e insire dois
 * ou três valores abaixo.
 */
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

/**
 * @brief Duplica os primeiro ou dois primeiros valores no topo da pilha de operandos e insire dois,
 * três ou quatro valores abaixo.
 */
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

/**
 * @brief Realiza uma troca dos dois valores no topo da pilha de operandos.
 */
void swap_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Realiza a operação de soma de inteiros 
 * 
 * Soma os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
void iadd_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 + v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de soma de long 
 * 
 * Soma os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#if defined DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de soma de floats 
 * 
 * Soma os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_float: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2_float: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado_float: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de soma de doubles 
 * 
 * Soma os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
  #if defined DEBUG
    printf("v1_double: %f \n", value1);
  #endif
  #if defined DEBUG
    printf("v2_double: %f \n", value2);
    printf("result: %f \n", resultdouble);
  #endif
  #if defined DEBUG
    printf("resultado_double: %f\n", result->type_double);
  #endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de subtração de inteiros 
 * 
 * Subtrai os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
void isub_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 - v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de subtração de long 
 * 
 * Subtrai os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#if defined DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de subtração de floats 
 * 
 * Subtrai os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %04f\n", value1);
#endif
#if defined DEBUG
  printf("v2: %04f\n", value2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de subtração de doubles 
 * 
 * Subtrai os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_double: %f \n", value1);
#endif
#if defined DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#if defined DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de multiplicação de inteiros 
 * 
 * Multiplica os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
void imul_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 * v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de multiplicação de longs 
 * 
 * Multiplica os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#if defined DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de multiplicação de floats 
 * 
 * Multiplica os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %04f\n", value1);
#endif
#if defined DEBUG
  printf("v2: %04f\n", value2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de multiplicação de doubles 
 * 
 * Multiplica os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_double: %f \n", value1);
#endif
#if defined DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#if defined DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de divisão de inteiros 
 * 
 * Divide os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
void idiv_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 / v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de divisão de longs 
 * 
 * Divide os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#if defined DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de divisão de floats 
 * 
 * Divide os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %04f\n", value1);
#endif
#if defined DEBUG
  printf("v2: %04f\n", value2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza a operação de divisão de doubles 
 * 
 * Divide os valores do topo da pilha de operandos e guarda o valor na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_double: %f \n", value1);
#endif
#if defined DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#if defined DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Coloca na pilha de operandos o resto da divisão dos dois valores int no topo da pilha de operandos.
 */
void irem_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 - (v1 / v2) * v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Coloca na pilha de operandos o resto da divisão dos dois valores long no topo da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#if defined DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Coloca na pilha de operandos o resto da divisão dos dois valores float no topo da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %04f\n", value1);
#endif
#if defined DEBUG
  printf("v2: %04f\n", value2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Coloca na pilha de operandos o resto da divisão dos dois valores double no topo da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_double: %f \n", value1);
#endif
#if defined DEBUG
  printf("v2_double: %f \n", value2);
  printf("result: %f \n", resultdouble);
#endif
#if defined DEBUG
  printf("resultado_double: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz a negação do valor inteiro na pilha de operandos.
 */
void ineg_eval(Frame *f)
{
  LocalVariable *value = pop_operand(f->operands);
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));
  result->type = CONSTANT_Integer;
  result->value = -value->value;
#if defined DEBUG
  printf("value: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz a negação do valor long na pilha de operandos.
 */
void lneg_eval(Frame *f)
{
  LocalVariable *value = pop_operand(f->operands);
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  long result_long = *(long *)&value->type_long;
  result_long = -result_long;
  result->type = CONSTANT_Long;
  result->type_long = *(uint64_t *)&result_long;
#if defined DEBUG
  printf("type_long: %ld\n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz a negação do valor float na pilha de operandos.
 */
void fneg_eval(Frame *f)
{
  u4 v1;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));
  float aux, nunca_esqueca_que_voce_e_limitado = 0;

  v1 = pop_operand(f->operands)->value;
  aux = nunca_esqueca_que_voce_e_limitado - *((float*)&v1);
  result->type = CONSTANT_Float;
  result->value = *((u4*)&aux);
  #if defined DEBUG
    printf("neg_aux: %f\n", aux);
    printf("resultado_neg: %04x\n", result->value);
  #endif

  push_operand(result, f->operands);
}

/**
 * @brief Faz a negação do valor double na pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_double: %f \n", value1);
#endif
#if defined DEBUG
  printf("v2_double: %f \n", value2);
  printf("result_neg: %f \n", resultdouble);
#endif
#if defined DEBUG
  printf("resultado_double_neg: %f\n", result->type_double);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz o shift left de um inteiro da pilha de operandos.
 */
void ishl_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 << (v2 & 0x1F);
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", (v2 & 0x1F));
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz o shift left de um long da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2: %ld \n", (value2 & 0x1F));
#endif
#if defined DEBUG
  printf("resultado: %04x \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz o shift right de um inteiro da pilha de operandos.
 */
void ishr_eval(Frame *f)
{
  int32_t v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 >> (v2 & 0x1F);
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", (v2 & 0x1F));
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz o shift right de um long da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2: %ld \n", (value2 & 0x1F));
#endif
#if defined DEBUG
  printf("resultado: %04x \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz o shift left lógico de um inteiro da pilha de operandos.
 */
void iushr_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 >> (v2 & 0x1F);
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", (v2 & 0x1F));
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Faz o shift left lógico de um long da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2: %ld \n", (value2 & 0x1F));
#endif
#if defined DEBUG
  printf("resultado: %04x \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza o AND lógica de dois valores inteiros da pilha de operandos.
 */
void iand_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 & v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza o AND lógico de dois valores long da pilha de operandos.
 */
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
#if defined DEBUG
  printf("v1_long: %ld \n", value1);
#endif
#if defined DEBUG
  printf("v2_long: %ld \n", value2);
  printf("result: %ld \n", resultlong);
#endif
#if defined DEBUG
  printf("resultado_long: %ld \n", result->type_long);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza o OR lógico de dois valores inteiros da pilha de operandos.
 */
void ior_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 | v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza o OR lógico de dois valores long da pilha de operandos.
 */
void lor_eval(Frame *f)
{
  int64_t v1, v2;
  long value1, value2, resultlong;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->type_long;
  v1 = pop_operand(f->operands)->type_long;
  value1 = *(long *)&v1;
  value2 = *(long *)&v2;

  resultlong = value1 | value2;
  result->type = CONSTANT_Long;

  result->type_long = *(uint64_t *)&resultlong;
  #if defined DEBUG
    printf("v1_long: %ld \n", value1);
    printf("v2_long: %ld \n", value2);
    printf("result: %ld \n", resultlong);
    printf("resultado_long: %ld \n", result->type_long);
  #endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza o XOR lógico de dois valores inteiros da pilha de operandos.
 */
void ixor_eval(Frame *f)
{
  u4 v1, v2;
  LocalVariable *result = (LocalVariable *)malloc(sizeof(LocalVariable));

  v2 = pop_operand(f->operands)->value;
  v1 = pop_operand(f->operands)->value;
  result->type = CONSTANT_Integer;
  result->value = v1 ^ v2;
#if defined DEBUG
  printf("v1: %04x\n", v1);
#endif
#if defined DEBUG
  printf("v2: %04x\n", v2);
#endif
#if defined DEBUG
  printf("resultado: %04x\n", result->value);
#endif
  push_operand(result, f->operands);
}

/**
 * @brief Realiza o XOR lógico de dois valores long da pilha de operandos.
 */
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
  #if defined DEBUG
    printf("v1_long: %ld \n", value1);
    printf("v2_long: %ld \n", value2);
    printf("result: %ld \n", resultlong);
    printf("resultado_long: %ld \n", result->type_long);
  #endif
  push_operand(result, f->operands);
}

/**
 * @brief Incrementa a variável local por uma constante.
 */
void iinc_eval(Frame *f)
{
  u1 index = f->bytecode[f->pc++];
  int32_t value = ((int32_t)((int8_t)f->bytecode[f->pc++]));
  #if defined DEBUG
    printf("iinc Index: %d\n", index);
    printf("iinc Value: %d\n", value);
  #endif
  f->local_variables[index].value += value;
}

/**
 * @brief Converte um inteiro para um long.
 */
void i2l_eval(Frame *f)
{
  int32_t val = *((int32_t*)&pop_operand(f->operands)->value);
  long aux;

  LocalVariable *long_val = (LocalVariable *)malloc(sizeof(LocalVariable));
  // memcpy(&long_val->value, &val, sizeof(uint64_t));

  aux = (long)val;
  long_val->type_long = *((uint64_t*)&aux);
  long_val->type = CONSTANT_Long;
  push_operand(long_val, f->operands);
  #if defined DEBUG
    printf("VALOR CONVERTIDO EM LONG ====> %d\n", *(int *)&long_val->type_long);
  #endif
}

/**
 * @brief Converte um inteiro para um float.
 */
void i2f_eval(Frame *f)
{
  int32_t val = *((int32_t*)&pop_operand(f->operands)->value);
  float aux;
  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  // memcpy(&float_val->value, &val, sizeof(uint32_t));
  aux = (float)val;
  float_val->type = CONSTANT_Float;
  float_val->value = *((u4*)&aux);
  push_operand(float_val, f->operands);
}

/**
 * @brief Converte um inteiro para um double.
 */
void i2d_eval(Frame *f)
{
  int32_t val = *((int32_t*)&pop_operand(f->operands)->value);
  double aux;
  LocalVariable *double_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  // memcpy(&double_val->type_double, &val, sizeof(uint64_t));
  // printf("i2d_val: %d\n",val);
  aux = (double)val;
  // printf("i2d_aux: %f\n",aux);

  double_val->type = CONSTANT_Double;
  double_val->type_double = *((uint64_t*)&aux);
  push_operand(double_val, f->operands);
}

/**
 * @brief Converte um long para um inteiro.
 */
void l2i_eval(Frame *f)
{
  int val = (int) pop_operand(f->operands)->value;

  LocalVariable *int_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  memcpy(&int_val->value, &val, sizeof(uint32_t));

  int_val->type = CONSTANT_Integer;
  push_operand(int_val, f->operands);
}

/**
 * @brief Converte um long para um float.
 */
void l2f_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_long);
  u4 r1;
  long aux;
  float aux_float;
  aux = *(long *)(&val);
  aux_float = (float)aux;
  
  #if defined DEBUG
    printf("l2d_val: %f\n", aux_float);
  #endif
  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4*)(&aux_float));
  // memcpy(&float_val->value, &val, sizeof(ulong32_t));

  float_val->type = CONSTANT_Float;
  float_val->value = r1;
  push_operand(float_val, f->operands);
}


/**
 * @brief Converte um long para um double.
 */
void l2d_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_long);
  uint64_t r1;
  long aux;
  double aux_double;
  aux = *(long *)(&val);
  aux_double = (double)aux;
  
  #if defined DEBUG
    printf("l2d_val: %f\n", aux_double);
  #endif
  LocalVariable *double_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((uint64_t*)(&aux_double));
  // memcpy(&float_val->value, &val, sizeof(ulong32_t));

  double_val->type = CONSTANT_Double;
  double_val->type_double = r1;
  push_operand(double_val, f->operands);
}

/**
 * @brief Converte um float para um inteiro.
 */
void f2i_eval(Frame *f)
{
  u4 val = (pop_operand(f->operands)->value);
  u4 r1;
  float aux;
  int aux_int;
  aux = *(float *)(&val);
  aux_int = (int)aux;
  
  #if defined DEBUG
    printf("f2i_val: %d\n", aux_int);
  #endif
  LocalVariable *int_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4 *)(&aux_int));

  int_val->type = CONSTANT_Integer;
  int_val->value = r1;
  push_operand(int_val, f->operands);
}

/**
 * @brief Converte um float para um long.
 */
void f2l_eval(Frame *f)
{
  u4 val = (pop_operand(f->operands)->value);
  u4 r1;
  float aux;
  long aux_long;
  aux = *(float *)(&val);
  aux_long = (long)aux;
  
  #if defined DEBUG
    printf("f2l_val: %ld\n", aux_long);
  #endif
  LocalVariable *long_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((uint64_t *)(&aux_long));

  long_val->type = CONSTANT_Long;
  long_val->type_long = r1;
  push_operand(long_val, f->operands);
}

/**
 * @brief Converte um float para um double.
 */
void f2d_eval(Frame *f)
{
  float val = *((float*)&pop_operand(f->operands)->value);
  uint64_t r1;
  double aux_double;
  aux_double = (double)val;
  
  #if defined DEBUG
    printf("f2d_val: %f\n", aux_double);
  #endif
  // printf("f2d_val: %f\n", aux_double);
  LocalVariable *double_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((uint64_t*)&aux_double);
  // printf("r1: %ld\n", r1);
  double_val->type = CONSTANT_Double;
  double_val->type_double = r1;
  // printf("typedouble: %ld\n", double_val->type_double);
  push_operand(double_val, f->operands);
}

/**
 * @brief Converte um double para um inteiro.
 */
void d2i_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_double);
  u4 r1;
  double aux;
  int aux_int;
  aux = *(double *)(&val);
  aux_int = (int)aux;
  
  #if defined DEBUG
    printf("d2f_val: %d\n", aux_int);
  #endif
  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4*)(&aux_int));
  // memcpy(&float_val->value, &val, sizeof(uint32_t));

  float_val->type = CONSTANT_Integer;
  float_val->value = r1;
  push_operand(float_val, f->operands);
}

/**
 * @brief Converte um double para um long.
 */
void d2l_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_double);
  uint64_t r1;
  double aux;
  long aux_long;
  aux = *(double *)(&val);
  aux_long = (long)aux;
  
  #if defined DEBUG
    printf("d2l_val: %ld\n", aux_long);
  #endif
  LocalVariable *long_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((uint64_t*)(&aux_long));
  // memcpy(&float_val->value, &val, sizeof(ulong32_t));

  long_val->type = CONSTANT_Long;
  long_val->type_long = r1;
  push_operand(long_val, f->operands);
}

/**
 * @brief Converte um double para um float.
 */
void d2f_eval(Frame *f)
{
  uint64_t val = (pop_operand(f->operands)->type_double);
  u4 r1;
  double aux;
  float aux_float;
  aux = *(double *)(&val);
  aux_float = (float)aux;
  
  #if defined DEBUG
    printf("d2f_val: %f\n", aux_float);
  #endif
  LocalVariable *float_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  r1 = *((u4*)(&aux_float));
  // memcpy(&float_val->value, &val, sizeof(uint32_t));

  float_val->type = CONSTANT_Float;
  float_val->value = r1;
  push_operand(float_val, f->operands);
}

/**
 * @brief Converte um inteiro para um byte.
 */
void i2b_eval(Frame *f)
{
  int32_t val = *((int32_t*)&pop_operand(f->operands)->value);
  int8_t aux;
  aux = *(int8_t*)&val;
  LocalVariable *byte_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  // memcpy(&byte_val->value, &val, sizeof(uint32_t));
  // printf("i2b_val: %01x\n", val);
  byte_val->value = (hex_0 | aux);

  byte_val->type = CONSTANT_Integer; // colocar o tipo CONSTANT_Byte
  push_operand(byte_val, f->operands);
}

/**
 * @brief Converte um inteiro para um char.
 */
void i2c_eval(Frame *f)
{
  int32_t val = *((int32_t*)&pop_operand(f->operands)->value);
  char aux;
  LocalVariable *short_val = (LocalVariable *)malloc(sizeof(LocalVariable));

  aux = val;
  // printf("aux_char_i2c: %c\n", aux);
  short_val->type = CONSTANT_String;
  short_val->value = (hex_0 | aux);
  push_operand(short_val, f->operands);
}

/**
 * @brief Converte um inteiro para uma string.
 */
void i2s_eval(Frame *f)
{
  int32_t val = *((int32_t*)&pop_operand(f->operands)->value);
  int16_t aux;
  LocalVariable *short_val = (LocalVariable *)malloc(sizeof(LocalVariable));
  aux = *(int16_t*)&val;
  short_val->type = CONSTANT_Integer;
  short_val->value = (hex_0 | aux);
  push_operand(short_val, f->operands);
}

/**
 * @brief Realiza uma comparação entre dois valores do tipo long.
 */
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

/**
 * @brief Realiza uma comparação entre dois valores do tipo float.
 */
void fcmpl_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  float value1, value2;
  v2 = pop_operand(f->operands);
  v1 = pop_operand(f->operands);
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  value1 = *(float *)(&v1->value);
  value2 = *(float *)(&v2->value);
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

/**
 * @brief Compara dois valores do tipo float para saber qual é o maior.
 */
void fcmpg_eval(Frame *f)
{
  LocalVariable *v1, *v2, *lv;
  float value1, value2;
  v2 = pop_operand(f->operands);
  v1 = pop_operand(f->operands);
  lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  value1 = *(float *)(&v1->value);
  value2 = *(float *)(&v2->value);
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

/**
 * @brief Compara dois valores do tipo double.
 */
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

/**
 * @brief Compara dois valores do tipo double para saber qual é o maior.
 */
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

/**
 * @brief Operação condicional que sucede se o valor retirado da pilha de operandos for igual a zero.
 */
void ifeq_eval(Frame *f)
{
  int32_t v1 = pop_operand(f->operands)->value;
  #if defined DEBUG
  printf("valor_ifeq: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #if defined DEBUG
  printf("valor_ifeq_value: %d\n", value);
  #endif
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];
  if (value == 0)
  {

    int16_t offset = (((int16_t)branchbyte1 << 8) | (int16_t)branchbyte2);

#if defined DEBUG
    printf("ifeq: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se o valor retirado da pilha de operandos for igual diferente de zero.
 */
void ifne_eval(Frame *f)
{
  int32_t value = (int32_t) pop_operand(f->operands)->value;
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];
  #if defined DEBUG
  printf("valor_ifne: %04x\n", v1);
  #endif
  #if defined DEBUG
  printf("valor_ifne_value: %d\n", value);
  #endif
  if (value != 0)
  {

    int16_t offset = (((int16_t)branchbyte1 << 8) | (int16_t)branchbyte2);

#if defined DEBUG
    printf("ifne: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se o valor retirado da pilha de operandos for menor que zero.
 */
void iflt_eval(Frame *f)
{
  int32_t value = pop_operand(f->operands)->value;
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];
  #if defined DEBUG
  printf("valor_iflt: %04x\n", v1);
  #endif

  #if defined DEBUG
  printf("valor_iflt_value: %d\n", value);
  #endif
  if (value < 0)
  {

    int16_t offset = (((int16_t)branchbyte1 << 8) | (int16_t)branchbyte2);

#if defined DEBUG
    printf("iflt: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se o valor retirado da pilha de operandos for maior ou igual a zero.
 */
void ifge_eval(Frame *f)
{
  int32_t v1 = pop_operand(f->operands)->value;
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];
  #if defined DEBUG
  printf("valor_ifge: %04x\n", v1);
  #endif
  int value = *(int *)&(v1);
  #if defined DEBUG
  printf("valor_ifge_value: %d\n", value);
  #endif
  if (value >= 0)
  {

    int16_t offset = (((int16_t)branchbyte1 << 8) | (int16_t)branchbyte2);

#if defined DEBUG
    printf("ifge: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se o valor retirado da pilha de operandos for maior que zero.
 */
void ifgt_eval(Frame *f)
{
  int32_t value = pop_operand(f->operands)->value;
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];
  #if defined DEBUG
  printf("valor_ifgt: %04x\n", v1);
  #endif
  #if defined DEBUG
  printf("valor_ifgt_value: %d\n", value);
  #endif
  if (value > 0)
  {

    int16_t offset = (((int16_t)branchbyte1 << 8) | (int16_t)branchbyte2);

#if defined DEBUG
    printf("ifgt: vou pular %d\n", (offset - 3));
#endif

    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se o valor retirado da pilha de operandos for menor ou igual a zero.
 */
void ifle_eval(Frame *f)
{
  int32_t v1 = (int32_t)pop_operand(f->operands)->value;
  #if defined DEBUG
  printf("valor_ifle: %04x\n", v1);
  #endif
  int32_t value = *(int32_t *)&(v1);
  #if defined DEBUG
  printf("valor_ifle_value: %d\n", value);
  #endif
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];
  if (value <= 0)
  {

    int16_t offset = ((int16_t)(branchbyte1 << 8) | (int16_t)branchbyte2);

    #if defined DEBUG
        printf("ifle: vou pular %d\n", (offset - 3));
    #endif

    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se os valores retirados da pilha forem iguais.
 */
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

/**
 * @brief Operação condicional que sucede se os valores retirados da pilha forem diferentes.
 */
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

/**
 * @brief Operação condicional que sucede se o valor 1 < valor 2.
 *
 * Valor1 e Valor2 são retirados da pilha de operandos.
 */
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

/**
 * @brief Operação condicional que sucede se o o valor 1 >= valor 2.
 *
 * Valor1 e Valor2 são retirados da pilha de operandos.
 */
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

/**
 * @brief Operação condicional que sucede se o valor 1 > valor 2.
 *
 * Valor1 e Valor2 são retirados da pilha de operandos.
 */
void if_icmpgt_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  int32_t value2 = (int32_t)pop_operand(f->operands)->value;
  int32_t value1 = (int32_t)pop_operand(f->operands)->value;
  if (value1 > value2)
  {
    f->pc += offset - 3;
  }
}

/**
 * @brief Operação condicional que sucede se o valor 1 <= valor 2.
 *
 * Valor1 e Valor2 são retirados da pilha de operandos.
 */
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

/**
 * @brief Operação condicional que sucede se o valor 1 = valor 2, onde os valores são referências.
 *
 * Valor1 e Valor2 são retirados da pilha de operandos.
 */
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

/**
 * @brief Operação condicional que sucede se o valor 1 != valor 2, onde os valores são referências.
 *
 * Valor1 e Valor2 são retirados da pilha de operandos.
 */
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

/**
 * @brief É um branch que ocorre sempre.
 */
void goto_eval(Frame *f)
{
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  f->pc += offset - 3;
}

/**
 * @brief Pula para uma subrotina.
 *
 * Endereço do opcode que segue imediatamente a instrução é colocado na pilha de operandos.
 */
void jsr_eval(Frame *f)
{
  //TODO
}
/**
 * @brief Retorna de uma subrotina.
 */
void ret_eval(Frame *f)
{
  //TODO
}
/**
 * @brief Instrução de tamanho variável.
 */
void tableswitch_eval(Frame *f)
{
  u1 defaultbyte1, defaultbyte2, defaultbyte3, defaultbyte4;
  u4 aux_pc, bytes_padding, offset, pc_novo;
  u4 key = pop_operand(f->operands)->value;
  int32_t default_v, low, high, npairs; 
  int32_t qtd_offset, posicao;
  uint32_t temp;
  u1 *bytecode = f->method->attributes->info->Code_attribute.code;

  // diz se o novo valor de pc ja esta definido ou nao 
  int definido = 0; 

  // pc auxiliar que iremos seguindo durante a execucao da instrucao 
  aux_pc = f->pc; 
  
  /* passo 1 - le ateh o low. Se o key eh menor que low - define logo o novo valor para PC */
  // pula bytes de preenchimento
  //bytes_preench = (aux_pc + 1) % 4;
  bytes_padding = (4 - ((aux_pc + 1) % 4) ) % 4;
  aux_pc += bytes_padding;
  aux_pc++;
  
  // pega bytes do target default
  default_v = 0;
  for (int l = 0; l < 4; l++)
  {
      default_v = (default_v << 8) + bytecode[aux_pc];   
      aux_pc++;
  }       

  // pega bytes low
  low = 0;
  for (int l = 0; l < 4; l++)
  {
      low = (low << 8) + bytecode[aux_pc];   
      aux_pc++; 
  }       
  

  // se o key eh menor que o low e ainda nao definimos novo pc
  if (key < low && !definido)
  {
      definido = 1;
      pc_novo = f->pc + default_v; 
  }

  /* passo 2 - le ateh o high. Se o key eh maior que o high - define o novo valor para PC
    * caso low ainda nao tenha sido definido */ 
  // pega bytes high 
  high = 0;
  for (int l = 0; l < 4; l++)
  {
      high = (high << 8) + bytecode[aux_pc];   
      aux_pc++; 
  }       

  // se o key eh maior que o high e ainda nao definimos novo pc
  if (key > high && !definido)
  {
      definido = 1;
      pc_novo = f->pc + default_v; 
  }

  /* passo 3 - calcula offset na posicao index - low. Coloca novo endereco de PC aqui, caso ainda não tenha sido 
    * definido */ 
  qtd_offset = 1 + high - low; 
  posicao = key - low; 
  for (int32_t l = 0; l < qtd_offset; l++)
  {
      // se estamos na posicao correta
      if (l == posicao)
      {
          // extrai offset
          offset = 0;
          for (int i = 0; i < 4; i++)
          {
              offset = (offset << 8) + bytecode[aux_pc];   
              aux_pc++; 
          }       
          
          // calcula posicao 
          pc_novo = f->pc + offset; 
          definido = 1;
          
          // sai do loop 
          break;
      }

      // senao, passa pelo offset atual incrementando pc
      else
      {
          for (int i = 0; i < 4; i++)
          {
              aux_pc++;
          }       
      }
  }

  // poe valor correto em frameCorrente
  f->pc = pc_novo-1;  
}

/**
 * @brief Acessa jump table pela key
 */
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

/**
 * @brief Retorna um valor inteiro de um método.
 */
void ireturn_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

/**
 * @brief Retorna valor long de um método.
 */
void lreturn_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

/**
 * @brief Retorna valor float de um método.
 */
void freturn_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

/**
 * @brief Retorna valor double de um método.
 */
void dreturn_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);

  pop(JvmStack);
  if (!empty(JvmStack))
  {
    push_operand(lv, JvmStack->top->f->operands);
  }
}

/**
 * @brief Retorna valor reference de um método.
 */
void areturn_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Retorna void de um método.
 */
void return_eval(Frame *f)
{
  pop(JvmStack);
#if defined DEBUG
// printf("%02x", JvmStack->f->pc);
#endif
}

/**
 * @brief Pega um campo static de uma classe.
 *
 * A função getIndexFromb1b2() é usada para calcular um indice que é usado para
 * acessar um field na constant pool.
 */
void getstatic_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);

  // recupera Utf8 da referencia
  char *class_name = ret_method_name(f->cp, index);

#if defined DEBUG
  printf("nome da classe: %s\n", class_name);
#endif
#if defined DEBUG
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

#if defined DEBUG
  // printf("field_name: %s\n", field_name);
  printf("field_desc: %s\n", field_desc);
#endif

  u2 class_index = find_class(class_name);
  field_info *field = find_field(Mem.classes_arr[class_index], field_name, field_desc);

  push_operand(field->static_data, f->operands);

  // if (strcmp(field_desc, "[I") == 0)
  // {
  //   #if defined DEBUG
  //       printf("get static_data_low: %04x\n", *(GLOBAL_CLASS->fields->staticData->low));
  //   #endif
  //   lv->type_array.array = (u4 *)GLOBAL_CLASS->fields->staticData->low;
  //   lv->type = CONSTANT_Fieldref;
  // } else if((field_desc[0] == '[' ) && (field_desc[1] == 'L' )){
  //   lv->type_array.array = (u4 *)GLOBAL_CLASS->fields->staticData->low;
  //   lv->type = CONSTANT_Fieldref;
  //   #if defined DEBUG
  //       printf("getstatic field_desc: %s\n", field_desc);
  //   #endif
  // }
  // push_operand(lv, f->operands);
}

/**
 * @brief Seta campo static numa classe.
 *
 * A função getIndexFromb1b2() é usada para calcular um indice que é usado para
 * acessar um field na constant pool e guardar o seu valor.
 */
void putstatic_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);

  char *class_name = ret_method_name(f->cp, index);

  // Fieldref Name and type
  u2 name_n_type = f->cp[index - 1].Fieldref.name_and_type_index;

  char *field_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *field_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

#if defined DEBUG
  printf("field_name: %s\n", field_name);
  printf("field_desc: %s\n", field_desc);
#endif

  LocalVariable *lv = pop_operand(f->operands);
  // lv = f->operands->top;

#if defined DEBUG
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

  // #if defined DEBUG
  //     printf("put static_data_low: %04x\n", *(GLOBAL_CLASS->fields->staticData->low));
  // #endif
  //   }
  //   else if (strcmp(field_desc, "I") == 0)
  //   {
  //     GLOBAL_CLASS->fields->staticData = (staticData *)malloc(sizeof(staticData));
  //     GLOBAL_CLASS->fields->staticData->low = (u4 *)malloc(sizeof(u4));
  //     GLOBAL_CLASS->fields->staticData->high = NULL;
  //     GLOBAL_CLASS->fields->staticData->low[0] = lv->value;

  // #if defined DEBUG
  //     printf("put static_data_low: %04x\n", GLOBAL_CLASS->fields->staticData->low[0]);
  // #endif
  //   }
}

/**
 * @brief Acessar um field de um objeto.
 *
 * O objectref deve ser do tipo reference e é retirado da pilha de operandos. O valor de um
 * field é acessado na constant pool, o qual dá o nome e o descriptor de um field assim como
 * uma referência simbólica para a classe a qual ele pertence.
 */
void getfield_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  LocalVariable *obj_ref = pop_operand(f->operands);

  // Fieldref Name and type
  u2 name_n_type = f->cp[index - 1].Fieldref.name_and_type_index;

  char *field_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *field_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

#if defined DEBUG
  printf("get field name: %s\n", field_name);
  printf("get field desc: %s\n", field_desc);
#endif

  ClassFile *cf = Mem.classes_arr[obj_ref->type_object.class_index];

  LocalVariable *lv;

  // field_info *field = find_field(cf, field_name, field_desc);

  for (size_t i = 0; i < cf->fields_count; i++)
  {
    
    if (strcmp(field_name, obj_ref->type_object.fields[i].field_name) == 0 || strcmp(field_desc, obj_ref->type_object.fields[i].field_desc) == 0)
    {
      lv = obj_ref->type_object.fields[i].value;
      break;
    }
    
  }

  // EMpilha referencia para o field
  push_operand(lv, f->operands);
}

/**
 * @brief Seta field de um objeto.
 *
 * O objectref deve ser do tipo reference e é retirado da pilha de operandos. O valor de um
 * field é acessado na constant pool, o qual dá o nome e o descriptor de um field assim como
 * uma referência simbólica para a classe a qual ele pertence. O valor do field é setado.
 */
void putfield_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);

  // Fieldref Name and type
  u2 name_n_type = f->cp[index - 1].Fieldref.name_and_type_index;

  char *field_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *field_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

#if defined DEBUG
  printf("get field name: %s\n", field_name);
  printf("get field desc: %s\n", field_desc);
#endif

  LocalVariable *value = pop_operand(f->operands);
  LocalVariable *obj_ref = pop_operand(f->operands);

  ClassFile *cf = Mem.classes_arr[obj_ref->type_object.class_index];
  // field_info *field = find_field(cf, field_name, field_desc);

  for (size_t i = 0; i < cf->fields_count; i++) 
  {
    if (strcmp(field_name, obj_ref->type_object.fields[i].field_name) == 0 || strcmp(field_desc, obj_ref->type_object.fields[i].field_desc) == 0)
    {
      obj_ref->type_object.fields[i].value = value;
      break;
    }
  }
}

/**
 * @brief Invoca o método de uma instância.
 */
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
    if (strcmp(method_name, "println") == 0 || strcmp(method_name, "print") == 0)
    {
      if (strcmp(method_desc, "()V") == 0) // New line
      {
        printf("\n");
        return;
      }
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
      else if (strcmp(method_desc, "(C)V") == 0) // Char
      {
        u1 value = lv->value;
        printf("%c ", value);
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
    method_info *method = find_method(Mem.classes_arr[class_index], method_name, method_desc);
    Frame *frame = cria_frame(Mem.classes_arr[class_index]->constant_pool, method);
    // Adiciona argumestos comeca de 1
    // 0 é uma referencia
    // for (size_t i = 0; i <= args; i++)
    for (int8_t i = args; i >= 0; i--)
    {
      LocalVariable *lv = pop_operand(f->operands);
  	if (lv->type == CONSTANT_Double || lv->type == CONSTANT_Long)
  	{
  		i--;
    	frame->local_variables[i] = *lv;
  	}
  	else
  	{
  		frame->local_variables[i] = *lv;
  	}
#if defined DEBUG
      printf("DEBUG DE VERDADE:  ==== %04x\n", frame->local_variables[i].value);
#endif
    }

    push(frame);
  }
}

/**
 * @brief Invoca o método de uma instância, tratando de superclasses, métodos privados.
 */
void invokespecial_eval(Frame *f)
{
#if defined DEBUG
  printf("Invocando satanás...\n");
#endif
  // recupera Utf8 da referencia
  u2 index = getIndexFromb1b2(f);
#if defined DEBUG
// printf("%d", index);
#endif
//algo
#if defined DEBUG
// printf("index: %02x\n",f->cp[index].Class.name_index);
#endif
  char *class_name = ret_method_name(f->cp, index);
#if defined DEBUG
  printf("class_name: %s\n", class_name);
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

  // Pega o nome e tipo dó método pelo key da instrução.
  // Method Name and type
  u2 name_n_type = f->cp[index - 1].Methodref.name_and_type_index;

  char *method_name = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.name_index);

  char *method_desc = readUtf8(f->cp, f->cp[name_n_type - 1].NameAndType.descriptor_index);

  u2 args = count_args(method_desc);

  method_info *method = find_method(Mem.classes_arr[class_index], method_name, method_desc);
  Frame *frame = cria_frame(Mem.classes_arr[class_index]->constant_pool, method);

  // Adiciona argumestos
  // for (size_t i = args - 1; i >= 0; i--)
  // for (size_t i = 0; i < args; i++)
  for (int8_t i = args; i >= 0; i--)
  {
    LocalVariable *lv = pop_operand(f->operands);
  	if (lv->type == CONSTANT_Double || lv->type == CONSTANT_Long)
  	{
  		i--;
    	frame->local_variables[i] = *lv;
  	}
  	else
  	{
  		frame->local_variables[i] = *lv;
  	}
#if defined DEBUG
    printf("DEBUG DE VERDADE:  ==== %04x\n", frame->local_variables[i].value);
#endif
  }

  push(frame);
}

/**
 * @brief Invoca um método estático de uma classe.
 */
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

  #if defined DEBUG
    printf("Argumentos %d", args);
  #endif

  u2 class_index = find_class(class_name);

  method_info *method = find_method(Mem.classes_arr[class_index], method_name, method_desc);
  Frame *frame = cria_frame(Mem.classes_arr[class_index]->constant_pool, method);

  // Adiciona argumestos
  // for (size_t i = 0; i < args - 1; i++)
  for (int8_t i = args - 1; i >= 0; i--)
  {
  	LocalVariable *lv = pop_operand(f->operands);
  	if (lv->type == CONSTANT_Double || lv->type == CONSTANT_Long)
  	{
  		i--;
    	frame->local_variables[i] = *lv;
  	}
  	else
  	{
  		frame->local_variables[i] = *lv;
  	}
    #if defined DEBUG
        printf("DEBUG DE VERDADE:  ==== %04x\n", frame->local_variables[i].value);
    #endif
  }

  push(frame);
}

void invokeinterface_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Invoca um método dinâmico.
 */
void invokedynamic_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Instacia um novo objeto de uma classe.
 */
void new_eval(Frame *f)
{
  u2 index = getIndexFromb1b2(f);
  char *class_name = ret_method_name(f->cp, index);
  u2 class_index = find_class(class_name);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Class;
  lv->type_object.class_index = class_index;
  ClassFile *cf = Mem.classes_arr[class_index];

  lv->type_object.fields = (InstanceField*) malloc(sizeof(InstanceField) * cf->fields_count);

  for (size_t i = 0; i < cf->fields_count; i++)
  {
    char *field_name = readUtf8(cf->constant_pool, cf->fields[i].name_index);
    char *field_desc = readUtf8(cf->constant_pool, cf->fields[i].descriptor_index);

    lv->type_object.fields[i].field_name = field_name;

    lv->type_object.fields[i].field_desc = field_desc;

    lv->type_object.fields[i].value = (LocalVariable*) malloc(sizeof(LocalVariable));

  }

  // Empilha referencia para a classe no array de classes;
  push_operand(lv, f->operands);
}

/**
 * @brief Cria um novo array.
 *
 * Utiliza um parâmetro para saber que tipo de array criar.
 */
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
#if defined DEBUG
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
#if defined DEBUG
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
#if defined DEBUG
    printf("array type: %x \n", (u4)(rlv->type_array.array));
    printf("rlv type: %x \n", rlv->type);
#endif

    push_operand(rlv, f->operands);
  }
}

/**
 * @brief Cria um novo array de references.
 */
void anewarray_eval(Frame *f)
{
  LocalVariable *lv, *rlv;
  lv = pop_operand(f->operands);
  u2 index = getIndexFromb1b2(f);
  u4 count;
  count = lv->value;
  void *arrayref = NULL;
  rlv = (LocalVariable *)malloc(sizeof(LocalVariable));

  rlv->type = CONSTANT_Class;

  arrayref = (Object *)malloc((count) * sizeof(Object));
  rlv->type_array.array = (Object *)arrayref;
  rlv->type_array.size = count;

#if defined DEBUG
  printf("arrayref: %04x\n", rlv->type_array.array);
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

/**
 * @brief Encontra o tamanho do array.
 */
void arraylength_eval(Frame *f)
{
  LocalVariable *arrayref;
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));

  arrayref = pop_operand(f->operands);

  lv->value = arrayref->type_array.size;
  lv->type = CONSTANT_Integer;

  push_operand(lv, f->operands);
}

/**
 * @brief Lança uma exceção ou erro.
 */
void athrow_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Checa se um objeto é de determinado tipo.
 */
void checkcast_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Checa se o objeto é uma instância de um determinado tipo.
 */
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

/**
 * @brief Aumenta o índice de variáveis locais com bytes adicionais.
 */
void wide_eval(Frame *f)
{
  //TODO
}

void multianewarray_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Realiza um branch condicional se a referência for nula.
 */
void ifnull_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  if (lv == NULL)
  {
    f->pc += offset - 3;
  }
}

/**
 * @brief Realiza um branch se a condição não for nula.
 */
void ifnonnull_eval(Frame *f)
{
  LocalVariable *lv = pop_operand(f->operands);
  u1 branchbyte1, branchbyte2;
  branchbyte1 = f->bytecode[f->pc++];
  branchbyte2 = f->bytecode[f->pc++];

  int16_t offset = ((branchbyte1 << 8) | branchbyte2);
  if (lv != NULL)
  {
    f->pc += offset - 3;
  }
}

/**
 * @brief Sempre realiza o branch (com indice wide)
 */
void goto_w_eval(Frame *f)
{
  //TODO
}
/**
 * @brief Pula para subrotina (com indice wide)
 */
void jsr_w_eval(Frame *f)
{
  //TODO
}

/**
 * @brief Invoca o método de uma instância, tratando de superclasses, métodos privados.
 */
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
