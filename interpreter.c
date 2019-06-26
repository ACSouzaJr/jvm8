#include "structures.h"
#include "frame.h"
#include "stack_operands.h"
#include <stdlib.h>
#include "interpreter.h"
#include "stack_frame.h"

void aconst_null_eval(Frame *f) {
  push_operand(NULL, f->operands);
}

// void iconst_m1_eval(Frame *f) {
//   push_operand();
// }

// void iconst_0_eval(Frame *f) {
//   push_operand();
// }

// void iconst_1_eval(Frame *f) {
//   push_operand();
// }

// void iconst_2_eval(Frame *f) {
//   push_operand();
// }

// void iconst_3_eval(Frame *f) {
//   push_operand();
// }

// void iconst_4_eval(Frame *f) {
//   push_operand();
// }

// void iconst_5_eval(Frame *f) {
//   push_operand();
// }

// void lconst_0_eval(Frame *f) {
//   push_operand();
// }

// void lconst_1_eval(Frame *f) {
//   push_operand();
// }

// void fconst_0_eval(Frame *f) {
//   push_operand();
// }

// void fconst_1_eval(Frame *f) {
//   push_operand();
// }

// void fconst_2_eval(Frame *f) {
//   push_operand();
// }

// void dconst_0_eval(Frame *f) {
//   push_operand();
// }

// void dconst_1_eval(Frame *f) {
//   push_operand();
// }

// void bipush_eval(Frame *f) {
//   push_operand();
// }

// void sipush_eval(Frame *f) {
//   push_operand();
// }

// void ldc_eval(Frame *f) {
//   push_operand();
// }

// void ldc_w_eval(Frame *f) {
//   push_operand();
// }

// void ldc2_w_eval(Frame *f) {
//   push_operand();
// }

// void iload_eval(Frame *f) {
//   push_operand();
// }

// void lload_eval(Frame *f) {
//   push_operand();
// }

// void fload_eval(Frame *f) {
//   push_operand();
// }

// void dload_eval(Frame *f) {
//   push_operand();
// }

// void aload_eval(Frame *f) {
//   push_operand();
// }

// void iload_0_eval(Frame *f) {
//   push_operand();
// }

// void iload_1_eval(Frame *f) {
//   push_operand();
// }

// void iload_2_eval(Frame *f) {
//   push_operand();
// }

// void iload_3_eval(Frame *f) {
//   push_operand();
// }

// void lload_0_eval(Frame *f) {
//   push_operand();
// }

// void lload_1_eval(Frame *f) {
//   push_operand();
// }

// void lload_2_eval(Frame *f) {
//   push_operand();
// }

// void lload_3_eval(Frame *f) {
//   push_operand();
// }

// void fload_0_eval(Frame *f) {
//   push_operand();
// }

// void fload_1_eval(Frame *f) {
//   push_operand();
// }

// void fload_2_eval(Frame *f) {
//   push_operand();
// }

// void fload_3_eval(Frame *f) {
//   push_operand();
// }

// void dload_0_eval(Frame *f) {
//   push_operand();
// }

// void dload_1_eval(Frame *f) {
//   push_operand();
// }

// void dload_2_eval(Frame *f) {
//   push_operand();
// }

// void dload_3_eval(Frame *f) {
//   push_operand();
// }

void aload_0_eval(Frame *f) {
  // printf("lvv: %02x\n", f->local_variables[0].value);
  push_operand(&(f->local_variables[0]), f->operands);
}

void aload_1_eval(Frame *f) {
  push_operand(&(f->local_variables[1]), f->operands);
}

void aload_2_eval(Frame *f) {
  push_operand(&(f->local_variables[2]), f->operands);
}

void aload_3_eval(Frame *f) {
  push_operand(&(f->local_variables[3]), f->operands);
}

// void iaload_eval(Frame *f) {
//   push_operand();
// }

// void laload_eval(Frame *f) {
//   push_operand();
// }

// void faload_eval(Frame *f) {
//   push_operand();
// }

// void daload_eval(Frame *f) {
//   push_operand();
// }

// void aaload_eval(Frame *f) {
//   push_operand();
// }

// void baload_eval(Frame *f) {
//   push_operand();
// }

// void caload_eval(Frame *f) {
//   push_operand();
// }

// void saload_eval(Frame *f) {
//   push_operand();
// }

// void istore_eval(Frame *f) {
//   push_operand();
// }

// void lstore_eval(Frame *f) {
//   push_operand();
// }

// void fstore_eval(Frame *f) {
//   push_operand();
// }

// void dstore_eval(Frame *f) {
//   push_operand();
// }

// void astore_eval(Frame *f) {
//   push_operand();
// }

// void istore_0_eval(Frame *f) {
//   push_operand();
// }

// void istore_1_eval(Frame *f) {
//   push_operand();
// }

// void istore_2_eval(Frame *f) {
//   push_operand();
// }

// void istore_3_eval(Frame *f) {
//   push_operand();
// }

// void lstore_0_eval(Frame *f) {
//   push_operand();
// }

// void lstore_1_eval(Frame *f) {
//   push_operand();
// }

// void lstore_2_eval(Frame *f) {
//   push_operand();
// }

// void lstore_3_eval(Frame *f) {
//   push_operand();
// }

// void fstore_0_eval(Frame *f) {
//   push_operand();
// }

// void fstore_1_eval(Frame *f) {
//   push_operand();
// }

// void fstore_2_eval(Frame *f) {
//   push_operand();
// }

// void fstore_3_eval(Frame *f) {
//   push_operand();
// }

// void dstore_0_eval(Frame *f) {
//   push_operand();
// }

// void dstore_1_eval(Frame *f) {
//   push_operand();
// }

// void dstore_2_eval(Frame *f) {
//   push_operand();
// }

// void dstore_3_eval(Frame *f) {
//   push_operand();
// }

// void astore_0_eval(Frame *f) {
//   push_operand();
// }

// void astore_1_eval(Frame *f) {
//   push_operand();
// }

// void astore_2_eval(Frame *f) {
//   push_operand();
// }

// void astore_3_eval(Frame *f) {
//   push_operand();
// }

// void iastore_eval(Frame *f) {
//   push_operand();
// }

// void lastore_eval(Frame *f) {
//   push_operand();
// }

// void fastore_eval(Frame *f) {
//   push_operand();
// }

// void dastore_eval(Frame *f) {
//   push_operand();
// }

// void aastore_eval(Frame *f) {
//   push_operand();
// }

// void bastore_eval(Frame *f) {
//   push_operand();
// }

// void castore_eval(Frame *f) {
//   push_operand();
// }

// void sastore_eval(Frame *f) {
//   push_operand();
// }

// void pop_eval(Frame *f) {
//   push_operand();
// }

// void pop2_eval(Frame *f) {
//   push_operand();
// }

// void dup_eval(Frame *f) {
//   push_operand();
// }

// void dup_x1_eval(Frame *f) {
//   push_operand();
// }

// void dup_x2_eval(Frame *f) {
//   push_operand();
// }

// void dup2_eval(Frame *f) {
//   push_operand();
// }

// void dup2_x1_eval(Frame *f) {
//   push_operand();
// }

// void dup2_x2_eval(Frame *f) {
//   push_operand();
// }

// void swap_eval(Frame *f) {
//   push_operand();
// }

// void iadd_eval(Frame *f) {
//   push_operand();
// }

// void ladd_eval(Frame *f) {
//   push_operand();
// }

// void fadd_eval(Frame *f) {
//   push_operand();
// }

// void dadd_eval(Frame *f) {
//   push_operand();
// }

// void isub_eval(Frame *f) {
//   push_operand();
// }

// void lsub_eval(Frame *f) {
//   push_operand();
// }

// void fsub_eval(Frame *f) {
//   push_operand();
// }

// void dsub_eval(Frame *f) {
//   push_operand();
// }

// void imul_eval(Frame *f) {
//   push_operand();
// }

// void lmul_eval(Frame *f) {
//   push_operand();
// }

// void fmul_eval(Frame *f) {
//   push_operand();
// }

// void dmul_eval(Frame *f) {
//   push_operand();
// }

// void idiv_eval(Frame *f) {
//   push_operand();
// }

// void ldiv_eval(Frame *f) {
//   push_operand();
// }

// void fdiv_eval(Frame *f) {
//   push_operand();
// }

// void ddiv_eval(Frame *f) {
//   push_operand();
// }

// void irem_eval(Frame *f) {
//   push_operand();
// }

// void lrem_eval(Frame *f) {
//   push_operand();
// }

// void frem_eval(Frame *f) {
//   push_operand();
// }

// void drem_eval(Frame *f) {
//   push_operand();
// }

// void ineg_eval(Frame *f) {
//   push_operand();
// }

// void lneg_eval(Frame *f) {
//   push_operand();
// }

// void fneg_eval(Frame *f) {
//   push_operand();
// }

// void dneg_eval(Frame *f) {
//   push_operand();
// }

// void ishl_eval(Frame *f) {
//   push_operand();
// }

// void lshl_eval(Frame *f) {
//   push_operand();
// }

// void ishr_eval(Frame *f) {
//   push_operand();
// }

// void lshr_eval(Frame *f) {
//   push_operand();
// }

// void iushr_eval(Frame *f) {
//   push_operand();
// }

// void lushr_eval(Frame *f) {
//   push_operand();
// }

// void iand_eval(Frame *f) {
//   push_operand();
// }

// void land_eval(Frame *f) {
//   push_operand();
// }

// void ior_eval(Frame *f) {
//   push_operand();
// }

// void lor_eval(Frame *f) {
//   push_operand();
// }

// void ixor_eval(Frame *f) {
//   push_operand();
// }

// void lxor_eval(Frame *f) {
//   push_operand();
// }

// void iinc_eval(Frame *f) {
//   push_operand();
// }

// void i2l_eval(Frame *f) {
//   push_operand();
// }

// void i2f_eval(Frame *f) {
//   push_operand();
// }

// void i2d_eval(Frame *f) {
//   push_operand();
// }

// void l2i_eval(Frame *f) {
//   push_operand();
// }

// void l2f_eval(Frame *f) {
//   push_operand();
// }

// void l2d_eval(Frame *f) {
//   push_operand();
// }

// void f2i_eval(Frame *f) {
//   push_operand();
// }

// void f2l_eval(Frame *f) {
//   push_operand();
// }

// void f2d_eval(Frame *f) {
//   push_operand();
// }

// void d2i_eval(Frame *f) {
//   push_operand();
// }

// void d2l_eval(Frame *f) {
//   push_operand();
// }

// void d2f_eval(Frame *f) {
//   push_operand();
// }

// void i2b_eval(Frame *f) {
//   push_operand();
// }

// void i2c_eval(Frame *f) {
//   push_operand();
// }

// void i2s_eval(Frame *f) {
//   push_operand();
// }

// void lcmp_eval(Frame *f) {
//   push_operand();
// }

// void fcmpl_eval(Frame *f) {
//   push_operand();
// }

// void fcmpg_eval(Frame *f) {
//   push_operand();
// }

// void dcmpl_eval(Frame *f) {
//   push_operand();
// }

// void dcmpg_eval(Frame *f) {
//   push_operand();
// }

// void ifeq_eval(Frame *f) {
//   push_operand();
// }

// void ifne_eval(Frame *f) {
//   push_operand();
// }

// void iflt_eval(Frame *f) {
//   push_operand();
// }

// void ifge_eval(Frame *f) {
//   push_operand();
// }

// void ifgt_eval(Frame *f) {
//   push_operand();
// }

// void ifle_eval(Frame *f) {
//   push_operand();
// }

// void if_icmpeq_eval(Frame *f) {
//   push_operand();
// }

// void if_icmpne_eval(Frame *f) {
//   push_operand();
// }

// void if_icmplt_eval(Frame *f) {
//   push_operand();
// }

// void if_icmpg2_eval(Frame *f) {
//   push_operand();
// }

// void if_icmpgt_eval(Frame *f) {
//   push_operand();
// }

// void if_icmple_eval(Frame *f) {
//   push_operand();
// }

// void if_acmpeq_eval(Frame *f) {
//   push_operand();
// }

// void if_acmpne_eval(Frame *f) {
//   push_operand();
// }

// void goto_eval(Frame *f) {
//   push_operand();
// }

// void jsr_eval(Frame *f) {
//   push_operand();
// }

// void ret_eval(Frame *f) {
//   push_operand();
// }

// void tableswitch_eval(Frame *f) {
//   push_operand();
// }

// void lookupswitch_eval(Frame *f) {
//   push_operand();
// }

// void ireturn_eval(Frame *f) {
//   push_operand();
// }

// void lreturn_eval(Frame *f) {
//   push_operand();
// }

// void freturn_eval(Frame *f) {
//   push_operand();
// }

// void dreturn_eval(Frame *f) {
//   push_operand();
// }

// void areturn_eval(Frame *f) {
//   push_operand();
// }

void return_eval() {
  pop(JvmStack);
  // printf("%02x", JvmStack->f->pc);
}

// void getstatic_eval(Frame *f) {
//   push_operand();
// }

// void putstatic_eval(Frame *f) {
//   push_operand();
// }

// void getfield_eval(Frame *f) {
//   push_operand();
// }

// void putfield_eval(Frame *f) {
//   push_operand();
// }

// void invokevirtual_eval(Frame *f) {
//   push_operand();
// }

void invokespecial_eval(Frame *f, u1 index1byte, u1 index2byte, cp_info *constant_pool) {
  printf("Invocando satanás...\n");
  u2 index = ((index1byte << 8) | index2byte);
}

// void invokestatic_eval(Frame *f) {
//   push_operand();
// }

// void invokeinterface_eval(Frame *f) {
//   push_operand();
// }

// void invokedynamic_eval(Frame *f) {
//   push_operand();
// }

// void new_eval(Frame *f) {
//   push_operand();
// }

// void newarray_eval(Frame *f) {
//   push_operand();
// }

// void anewarray_eval(Frame *f) {
//   push_operand();
// }

// void arraylength_eval(Frame *f) {
//   push_operand();
// }

// void athrow_eval(Frame *f) {
//   push_operand();
// }

// void checkcast_eval(Frame *f) {
//   push_operand();
// }

// void instanceof_eval(Frame *f) {
//   push_operand();
// }

// void monitorenter_eval(Frame *f) {
//   push_operand();
// }

// void monitorexit_eval(Frame *f) {
//   push_operand();
// }

// void wide_eval(Frame *f) {
//   push_operand();
// }

// void multianewarray_eval(Frame *f) {
//   push_operand();
// }

// void ifnull_eval(Frame *f) {
//   push_operand();
// }

// void ifnonnull_eval(Frame *f) {
//   push_operand();
// }

// void goto_w_eval(Frame *f) {
//   push_operand();
// }

// void jsr_w_eval(Frame *f) {
//   push_operand();
// }

// void breakpoint_eval(Frame *f) {
//   push_operand();
// }

// void impdep1_eval(Frame *f) {
//   push_operand();
// }

// void impdep2_eval(Frame *f) {
//   push_operand();
// }
