#include <stdint.h>

typedef struct
{
  uint8_t key;
  char value[30];
  uint8_t arguments;
  uint16_t references;
} op_code;

extern op_code op_codes_array[300];
void initialize_op_codes();

enum
{
  newarray = 188,
  anewarray = 189,
  multianewarray = 197,
  checkcast = 192,
  getfield = 180,
  getstatic = 178,
  instanceofg = 193,
  invokedynamic = 186,
  invokeinterface = 185,
  invokespecial = 183,
  invokestatic = 184,
  invokevirtual = 182,
  ldc = 18,
  ldc_w = 19,
  ldc2_w = 20,
  NEW = 187,
  putfield = 181,
  putstatic = 179,
  ifeq = 153,
  ifne = 154,
  iflt = 155,
  ifge = 156,
  ifgt = 157,
  ifle = 158,
  if_icmpeq = 159,
  if_icmpne = 160,
  if_icmplt = 161,
  if_icmpge = 162,
  if_icmpgt = 163,
  if_icmple = 164,
  if_acmpeq = 165,
  if_acmpne = 166,
  GOTO = 167,
  jsr = 168,
  ifnonnull = 199,
  ifnull = 198,
  aload = 25,
  aload_0 = 42,
  aload_1 = 43,
  aload_2 = 44,
  aload_3 = 45,
  areturn = 176,
  return_original = 177,
  tableswitch = 170,
  lookupswitch = 171
} instructions;