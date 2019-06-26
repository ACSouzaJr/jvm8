#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "structures.h"
#include "frame.h"
#include "stack_operands.h"

void aconst_null_eval(Frame *f);
void iconst_m1_eval(Frame *f);
void iconst_0_eval(Frame *f);
void iconst_1_eval(Frame *f);
void iconst_2_eval(Frame *f);
void iconst_3_eval(Frame *f);
void iconst_4_eval(Frame *f);
void iconst_5_eval(Frame *f);
void lconst_0_eval(Frame *f);
void lconst_1_eval(Frame *f);
void fconst_0_eval(Frame *f);
void fconst_1_eval(Frame *f);
void fconst_2_eval(Frame *f);
void dconst_0_eval(Frame *f);
void dconst_1_eval(Frame *f);
void bipush_eval(Frame *f);
void sipush_eval(Frame *f);
void ldc_eval(Frame *f);
void ldc_w_eval(Frame *f);
void ldc2_w_eval(Frame *f);
void iload_eval(Frame *f);
void lload_eval(Frame *f);
void fload_eval(Frame *f);
void dload_eval(Frame *f);
void aload_eval(Frame *f);
void iload_0_eval(Frame *f);
void iload_1_eval(Frame *f);
void iload_2_eval(Frame *f);
void iload_3_eval(Frame *f);
void lload_0_eval(Frame *f);
void lload_1_eval(Frame *f);
void lload_2_eval(Frame *f);
void lload_3_eval(Frame *f);
void fload_0_eval(Frame *f);
void fload_1_eval(Frame *f);
void fload_2_eval(Frame *f);
void fload_3_eval(Frame *f);
void dload_0_eval(Frame *f);
void dload_1_eval(Frame *f);
void dload_2_eval(Frame *f);
void dload_3_eval(Frame *f);
void aload_0_eval(Frame *f);
void aload_1_eval(Frame *f);
void aload_2_eval(Frame *f);
void aload_3_eval(Frame *f);
void iaload_eval(Frame *f);
void laload_eval(Frame *f);
void faload_eval(Frame *f);
void daload_eval(Frame *f);
void aaload_eval(Frame *f);
void baload_eval(Frame *f);
void caload_eval(Frame *f);
void saload_eval(Frame *f);
void istore_eval(Frame *f);
void lstore_eval(Frame *f);
void fstore_eval(Frame *f);
void dstore_eval(Frame *f);
void astore_eval(Frame *f);
void istore_0_eval(Frame *f);
void istore_1_eval(Frame *f);
void istore_2_eval(Frame *f);
void istore_3_eval(Frame *f);
void lstore_0_eval(Frame *f);
void lstore_1_eval(Frame *f);
void lstore_2_eval(Frame *f);
void lstore_3_eval(Frame *f);
void fstore_0_eval(Frame *f);
void fstore_1_eval(Frame *f);
void fstore_2_eval(Frame *f);
void fstore_3_eval(Frame *f);
void dstore_0_eval(Frame *f);
void dstore_1_eval(Frame *f);
void dstore_2_eval(Frame *f);
void dstore_3_eval(Frame *f);
void astore_0_eval(Frame *f);
void astore_1_eval(Frame *f);
void astore_2_eval(Frame *f);
void astore_3_eval(Frame *f);
void iastore_eval(Frame *f);
void lastore_eval(Frame *f);
void fastore_eval(Frame *f);
void dastore_eval(Frame *f);
void aastore_eval(Frame *f);
void bastore_eval(Frame *f);
void castore_eval(Frame *f);
void sastore_eval(Frame *f);
void pop_eval(Frame *f);
void pop2_eval(Frame *f);
void dup_eval(Frame *f);
void dup_x1_eval(Frame *f);
void dup_x2_eval(Frame *f);
void dup2_eval(Frame *f);
void dup2_x1_eval(Frame *f);
void dup2_x2_eval(Frame *f);
void swap_eval(Frame *f);
void iadd_eval(Frame *f);
void ladd_eval(Frame *f);
void fadd_eval(Frame *f);
void dadd_eval(Frame *f);
void isub_eval(Frame *f);
void lsub_eval(Frame *f);
void fsub_eval(Frame *f);
void dsub_eval(Frame *f);
void imul_eval(Frame *f);
void lmul_eval(Frame *f);
void fmul_eval(Frame *f);
void dmul_eval(Frame *f);
void idiv_eval(Frame *f);
void ldiv_eval(Frame *f);
void fdiv_eval(Frame *f);
void ddiv_eval(Frame *f);
void irem_eval(Frame *f);
void lrem_eval(Frame *f);
void frem_eval(Frame *f);
void drem_eval(Frame *f);
void ineg_eval(Frame *f);
void lneg_eval(Frame *f);
void fneg_eval(Frame *f);
void dneg_eval(Frame *f);
void ishl_eval(Frame *f);
void lshl_eval(Frame *f);
void ishr_eval(Frame *f);
void lshr_eval(Frame *f);
void iushr_eval(Frame *f);
void lushr_eval(Frame *f);
void iand_eval(Frame *f);
void land_eval(Frame *f);
void ior_eval(Frame *f);
void lor_eval(Frame *f);
void ixor_eval(Frame *f);
void lxor_eval(Frame *f);
void iinc_eval(Frame *f);
void i2l_eval(Frame *f);
void i2f_eval(Frame *f);
void i2d_eval(Frame *f);
void l2i_eval(Frame *f);
void l2f_eval(Frame *f);
void l2d_eval(Frame *f);
void f2i_eval(Frame *f);
void f2l_eval(Frame *f);
void f2d_eval(Frame *f);
void d2i_eval(Frame *f);
void d2l_eval(Frame *f);
void d2f_eval(Frame *f);
void i2b_eval(Frame *f);
void i2c_eval(Frame *f);
void i2s_eval(Frame *f);
void lcmp_eval(Frame *f);
void fcmpl_eval(Frame *f);
void fcmpg_eval(Frame *f);
void dcmpl_eval(Frame *f);
void dcmpg_eval(Frame *f);
void ifeq_eval(Frame *f);
void ifne_eval(Frame *f);
void iflt_eval(Frame *f);
void ifge_eval(Frame *f);
void ifgt_eval(Frame *f);
void ifle_eval(Frame *f);
void if_icmpeq_eval(Frame *f);
void if_icmpne_eval(Frame *f);
void if_icmplt_eval(Frame *f);
void if_icmpg2_eval(Frame *f);
void if_icmpgt_eval(Frame *f);
void if_icmple_eval(Frame *f);
void if_acmpeq_eval(Frame *f);
void if_acmpne_eval(Frame *f);
void goto_eval(Frame *f);
void jsr_eval(Frame *f);
void ret_eval(Frame *f);
void tableswitch_eval(Frame *f);
void lookupswitch_eval(Frame *f);
void ireturn_eval(Frame *f);
void lreturn_eval(Frame *f);
void freturn_eval(Frame *f);
void dreturn_eval(Frame *f);
void areturn_eval(Frame *f);
void return_eval(Frame *f);
void getstatic_eval(Frame *f);
void putstatic_eval(Frame *f);
void getfield_eval(Frame *f);
void putfield_eval(Frame *f);
void invokevirtual_eval(Frame *f);
void invokespecial_eval(Frame *f);
void invokestatic_eval(Frame *f);
void invokeinterface_eval(Frame *f);
void invokedynamic_eval(Frame *f);
void new_eval(Frame *f);
void newarray_eval(Frame *f);
void anewarray_eval(Frame *f);
void arraylength_eval(Frame *f);
void athrow_eval(Frame *f);
void checkcast_eval(Frame *f);
void instanceof_eval(Frame *f);
void monitorenter_eval(Frame *f);
void monitorexit_eval(Frame *f);
void wide_eval(Frame *f);
void multianewarray_eval(Frame *f);
void ifnull_eval(Frame *f);
void ifnonnull_eval(Frame *f);
void goto_w_eval(Frame *f);
void jsr_w_eval(Frame *f);
void breakpoint_eval(Frame *f);
void impdep1_eval(Frame *f);
void impdep2_eval(Frame *f);

#endif