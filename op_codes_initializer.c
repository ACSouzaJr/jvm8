#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instructions.h"
#include "interpreter.h"

op_code op_codes_array[300];

void initialize_op_codes()
{
  strcpy(op_codes_array[0].value, "nop");
  op_codes_array[0].arguments = 0;
  // op_codes_array[0].eval = nop_eval;

  strcpy(op_codes_array[1].value, "aconst_null");
  op_codes_array[1].arguments = 0;
  op_codes_array[1].references = 0;
  op_codes_array[1].eval = aconst_null_eval;

  strcpy(op_codes_array[2].value, "iconst_m1");
  op_codes_array[2].arguments = 0;
  op_codes_array[2].references = 0;
  op_codes_array[2].eval = iconst_m1_eval;

  strcpy(op_codes_array[3].value, "iconst_0");
  op_codes_array[3].arguments = 0;
  op_codes_array[3].references = 0;
  op_codes_array[3].eval = iconst_0_eval;

  strcpy(op_codes_array[4].value, "iconst_1");
  op_codes_array[4].arguments = 0;
  op_codes_array[4].references = 0;
  op_codes_array[4].eval = iconst_1_eval;

  strcpy(op_codes_array[5].value, "iconst_2");
  op_codes_array[5].arguments = 0;
  op_codes_array[5].references = 0;
  op_codes_array[5].eval = iconst_2_eval;

  strcpy(op_codes_array[6].value, "iconst_3");
  op_codes_array[6].arguments = 0;
  op_codes_array[6].references = 0;
  op_codes_array[6].eval = iconst_3_eval;

  strcpy(op_codes_array[7].value, "iconst_4");
  op_codes_array[7].arguments = 0;
  op_codes_array[7].references = 0;
  op_codes_array[7].eval = iconst_4_eval;

  strcpy(op_codes_array[8].value, "iconst_5");
  op_codes_array[8].arguments = 0;
  op_codes_array[8].references = 0;
  op_codes_array[8].eval = iconst_5_eval;

  op_codes_array[8].eval = iconst_5_eval;

  strcpy(op_codes_array[9].value, "lconst_0");
  op_codes_array[9].arguments = 0;
  op_codes_array[9].references = 0;
  op_codes_array[9].eval = lconst_0_eval;

  strcpy(op_codes_array[10].value, "lconst_1");
  op_codes_array[10].arguments = 0;
  op_codes_array[10].references = 0;
  op_codes_array[10].eval = lconst_1_eval;

  strcpy(op_codes_array[11].value, "fconst_0");
  op_codes_array[11].arguments = 0;
  op_codes_array[11].references = 0;
  op_codes_array[11].eval = fconst_0_eval;

  strcpy(op_codes_array[12].value, "fconst_1");
  op_codes_array[12].arguments = 0;
  op_codes_array[12].references = 0;
  op_codes_array[12].eval = fconst_1_eval;

  strcpy(op_codes_array[13].value, "fconst_2");
  op_codes_array[13].arguments = 0;
  op_codes_array[13].references = 0;
  op_codes_array[13].eval = fconst_2_eval;

  strcpy(op_codes_array[14].value, "dconst_0");
  op_codes_array[14].arguments = 0;
  op_codes_array[14].references = 0;
  op_codes_array[14].eval = dconst_0_eval;

  strcpy(op_codes_array[15].value, "dconst_1");
  op_codes_array[15].arguments = 0;
  op_codes_array[15].references = 0;
  op_codes_array[15].eval = dconst_1_eval;

  strcpy(op_codes_array[16].value, "bipush");
  op_codes_array[16].arguments = 1;
  op_codes_array[16].references = 0;
  op_codes_array[16].eval = bipush_eval;

  strcpy(op_codes_array[17].value, "sipush");
  op_codes_array[17].arguments = 2;
  op_codes_array[17].references = 0;
  op_codes_array[17].eval = sipush_eval;

  strcpy(op_codes_array[18].value, "ldc");
  op_codes_array[18].arguments = 1;
  op_codes_array[18].references = 1;
  op_codes_array[18].eval = ldc_eval;

  strcpy(op_codes_array[19].value, "ldc_w");
  op_codes_array[19].arguments = 2;
  op_codes_array[19].references = 1;
  op_codes_array[19].eval = ldc_w_eval;

  strcpy(op_codes_array[20].value, "ldc2_w");
  op_codes_array[20].arguments = 2;
  op_codes_array[20].references = 1;
  op_codes_array[20].eval = ldc2_w_eval;

  strcpy(op_codes_array[21].value, "iload");
  op_codes_array[21].arguments = 1;
  op_codes_array[21].references = 0;
  op_codes_array[21].eval = iload_eval;

  strcpy(op_codes_array[22].value, "lload");
  op_codes_array[22].arguments = 1;
  op_codes_array[22].references = 0;
  op_codes_array[22].eval = lload_eval;

  strcpy(op_codes_array[23].value, "fload");
  op_codes_array[23].arguments = 1;
  op_codes_array[23].references = 0;
  op_codes_array[23].eval = fload_eval;

  strcpy(op_codes_array[24].value, "dload");
  op_codes_array[24].arguments = 1;
  op_codes_array[24].references = 0;
  op_codes_array[24].eval = dload_eval;

  strcpy(op_codes_array[25].value, "aload");
  op_codes_array[25].arguments = 1;
  op_codes_array[25].references = 0;
  op_codes_array[25].eval = aload_eval;

  strcpy(op_codes_array[26].value, "iload_0");
  op_codes_array[26].arguments = 0;
  op_codes_array[26].references = 0;
  op_codes_array[26].eval = iload_0_eval;

  strcpy(op_codes_array[27].value, "iload_1");
  op_codes_array[27].arguments = 0;
  op_codes_array[27].references = 0;
  op_codes_array[27].eval = iload_1_eval;

  strcpy(op_codes_array[28].value, "iload_2");
  op_codes_array[28].arguments = 0;
  op_codes_array[28].references = 0;
  op_codes_array[28].eval = iload_2_eval;

  strcpy(op_codes_array[29].value, "iload_3");
  op_codes_array[29].arguments = 0;
  op_codes_array[29].references = 0;
  op_codes_array[29].eval = iload_3_eval;

  strcpy(op_codes_array[30].value, "lload_0");
  op_codes_array[30].arguments = 0;
  op_codes_array[30].references = 0;
  op_codes_array[30].eval = lload_0_eval;

  strcpy(op_codes_array[31].value, "lload_1");
  op_codes_array[31].arguments = 0;
  op_codes_array[31].references = 0;
  op_codes_array[31].eval = lload_1_eval;

  strcpy(op_codes_array[32].value, "lload_2");
  op_codes_array[32].arguments = 0;
  op_codes_array[32].references = 0;
  op_codes_array[32].eval = lload_2_eval;

  strcpy(op_codes_array[33].value, "lload_3");
  op_codes_array[33].arguments = 0;
  op_codes_array[33].references = 0;
  op_codes_array[33].eval = lload_3_eval;

  strcpy(op_codes_array[34].value, "fload_0");
  op_codes_array[34].arguments = 0;
  op_codes_array[34].references = 0;
  op_codes_array[34].eval = fload_0_eval;

  strcpy(op_codes_array[35].value, "fload_1");
  op_codes_array[35].arguments = 0;
  op_codes_array[35].references = 0;
  op_codes_array[35].eval = fload_1_eval;

  strcpy(op_codes_array[36].value, "fload_2");
  op_codes_array[36].arguments = 0;
  op_codes_array[36].references = 0;
  op_codes_array[36].eval = fload_2_eval;

  strcpy(op_codes_array[37].value, "fload_3");
  op_codes_array[37].arguments = 0;
  op_codes_array[37].references = 0;
  op_codes_array[37].eval = fload_3_eval;

  strcpy(op_codes_array[38].value, "dload_0");
  op_codes_array[38].arguments = 0;
  op_codes_array[38].references = 0;
  op_codes_array[38].eval = dload_0_eval;

  strcpy(op_codes_array[39].value, "dload_1");
  op_codes_array[39].arguments = 0;
  op_codes_array[39].references = 0;
  op_codes_array[39].eval = dload_1_eval;

  strcpy(op_codes_array[40].value, "dload_2");
  op_codes_array[40].arguments = 0;
  op_codes_array[40].references = 0;
  op_codes_array[40].eval = dload_2_eval;

  strcpy(op_codes_array[41].value, "dload_3");
  op_codes_array[41].arguments = 0;
  op_codes_array[41].references = 0;
  op_codes_array[41].eval = dload_3_eval;

  strcpy(op_codes_array[42].value, "aload_0");
  op_codes_array[42].arguments = 0;
  op_codes_array[42].references = 0;
  op_codes_array[42].eval = aload_0_eval;

  strcpy(op_codes_array[43].value, "aload_1");
  op_codes_array[43].arguments = 0;
  op_codes_array[43].references = 0;
  op_codes_array[43].eval = aload_1_eval;

  strcpy(op_codes_array[44].value, "aload_2");
  op_codes_array[44].arguments = 0;
  op_codes_array[40].references = 0;
  op_codes_array[40].eval = aload_2_eval;

  strcpy(op_codes_array[45].value, "aload_3");
  op_codes_array[45].arguments = 0;
  op_codes_array[45].references = 0;
  op_codes_array[45].eval = aload_3_eval;

  strcpy(op_codes_array[46].value, "iaload");
  op_codes_array[46].arguments = 0;
  op_codes_array[46].references = 0;
  op_codes_array[46].eval = iaload_eval;

  strcpy(op_codes_array[47].value, "laload");
  op_codes_array[47].arguments = 0;
  op_codes_array[47].references = 0;
  op_codes_array[47].eval = laload_eval;

  strcpy(op_codes_array[48].value, "faload");
  op_codes_array[48].arguments = 0;
  op_codes_array[48].references = 0;
  op_codes_array[48].eval = faload_eval;

  strcpy(op_codes_array[49].value, "daload");
  op_codes_array[49].arguments = 0;
  op_codes_array[49].references = 0;
  op_codes_array[49].eval = daload_eval;

  strcpy(op_codes_array[50].value, "aaload");
  op_codes_array[50].arguments = 0;
  op_codes_array[50].references = 0;
  op_codes_array[50].eval = aaload_eval;

  strcpy(op_codes_array[51].value, "baload");
  op_codes_array[51].arguments = 0;
  op_codes_array[51].references = 0;
  op_codes_array[51].eval = baload_eval;

  strcpy(op_codes_array[52].value, "caload");
  op_codes_array[52].arguments = 0;
  op_codes_array[52].references = 0;
  op_codes_array[52].eval = caload_eval;

  strcpy(op_codes_array[53].value, "saload");
  op_codes_array[53].arguments = 0;
  op_codes_array[53].references = 0;
  op_codes_array[53].eval = saload_eval;

  strcpy(op_codes_array[54].value, "istore");
  op_codes_array[54].arguments = 1;
  op_codes_array[54].references = 0;
  op_codes_array[54].eval = istore_eval;

  strcpy(op_codes_array[55].value, "lstore");
  op_codes_array[55].arguments = 1;
  op_codes_array[55].references = 0;
  op_codes_array[55].eval = lstore_eval;

  strcpy(op_codes_array[56].value, "fstore");
  op_codes_array[56].arguments = 1;
  op_codes_array[56].references = 0;
  op_codes_array[56].eval = fstore_eval;

  strcpy(op_codes_array[57].value, "dstore");
  op_codes_array[57].arguments = 1;
  op_codes_array[57].references = 0;
  op_codes_array[57].eval = dstore_eval;

  strcpy(op_codes_array[58].value, "astore");
  op_codes_array[58].arguments = 1;
  op_codes_array[58].references = 0;
  op_codes_array[58].eval = astore_eval;

  strcpy(op_codes_array[59].value, "istore_0");
  op_codes_array[59].arguments = 0;
  op_codes_array[59].references = 0;
  op_codes_array[59].eval = istore_0_eval;

  strcpy(op_codes_array[60].value, "istore_1");
  op_codes_array[60].arguments = 0;
  op_codes_array[60].references = 0;
  op_codes_array[60].eval = istore_1_eval;

  strcpy(op_codes_array[61].value, "istore_2");
  op_codes_array[61].arguments = 0;
  op_codes_array[61].references = 0;
  op_codes_array[61].eval = istore_2_eval;

  strcpy(op_codes_array[62].value, "istore_3");
  op_codes_array[62].arguments = 0;
  op_codes_array[62].references = 0;
  op_codes_array[62].eval = istore_3_eval;

  strcpy(op_codes_array[63].value, "lstore_0");
  op_codes_array[63].arguments = 0;
  op_codes_array[63].references = 0;
  op_codes_array[63].eval = lstore_0_eval;

  strcpy(op_codes_array[64].value, "lstore_1");
  op_codes_array[64].arguments = 0;
  op_codes_array[64].references = 0;
  op_codes_array[64].eval = lstore_1_eval;

  strcpy(op_codes_array[65].value, "lstore_2");
  op_codes_array[65].arguments = 0;
  op_codes_array[65].references = 0;
  op_codes_array[65].eval = lstore_2_eval;

  strcpy(op_codes_array[66].value, "lstore_3");
  op_codes_array[66].arguments = 0;
  op_codes_array[66].references = 0;
  op_codes_array[66].eval = lstore_3_eval;

  strcpy(op_codes_array[67].value, "fstore_0");
  op_codes_array[67].arguments = 0;
  op_codes_array[67].references = 0;
  op_codes_array[67].eval = fstore_0_eval;

  strcpy(op_codes_array[68].value, "fstore_1");
  op_codes_array[68].arguments = 0;
  op_codes_array[68].references = 0;
  op_codes_array[68].eval = fstore_1_eval;

  strcpy(op_codes_array[69].value, "fstore_2");
  op_codes_array[69].arguments = 0;
  op_codes_array[69].references = 0;
  op_codes_array[69].eval = fstore_2_eval;

  strcpy(op_codes_array[70].value, "fstore_3");
  op_codes_array[70].arguments = 0;
  op_codes_array[70].references = 0;
  op_codes_array[70].eval = fstore_3_eval;

  strcpy(op_codes_array[71].value, "dstore_0");
  op_codes_array[71].arguments = 0;
  op_codes_array[71].references = 0;
  op_codes_array[71].eval = dstore_0_eval;

  strcpy(op_codes_array[72].value, "dstore_1");
  op_codes_array[72].arguments = 0;
  op_codes_array[72].references = 0;
  op_codes_array[72].eval = dstore_1_eval;

  strcpy(op_codes_array[73].value, "dstore_2");
  op_codes_array[73].arguments = 0;
  op_codes_array[73].references = 0;
  op_codes_array[73].eval = dstore_2_eval;

  strcpy(op_codes_array[74].value, "dstore_3");
  op_codes_array[74].arguments = 0;
  op_codes_array[74].references = 0;
  op_codes_array[74].eval = dstore_3_eval;

  strcpy(op_codes_array[75].value, "astore_0");
  op_codes_array[75].arguments = 0;
  op_codes_array[75].references = 0;
  op_codes_array[75].eval = astore_0_eval;

  strcpy(op_codes_array[76].value, "astore_1");
  op_codes_array[76].arguments = 0;
  op_codes_array[76].references = 0;
  op_codes_array[76].eval = astore_1_eval;

  strcpy(op_codes_array[77].value, "astore_2");
  op_codes_array[77].arguments = 0;
  op_codes_array[77].references = 0;
  op_codes_array[77].eval = astore_2_eval;

  strcpy(op_codes_array[78].value, "astore_3");
  op_codes_array[78].arguments = 0;
  op_codes_array[78].references = 0;
  op_codes_array[78].eval = astore_3_eval;

  strcpy(op_codes_array[79].value, "iastore");
  op_codes_array[79].arguments = 0;
  op_codes_array[79].references = 0;
  op_codes_array[79].eval = iastore_eval;

  strcpy(op_codes_array[80].value, "lastore");
  op_codes_array[80].arguments = 0;
  op_codes_array[80].references = 0;
  op_codes_array[80].eval = lastore_eval;

  strcpy(op_codes_array[81].value, "fastore");
  op_codes_array[81].arguments = 0;
  op_codes_array[81].references = 0;
  op_codes_array[81].eval = fastore_eval;

  strcpy(op_codes_array[82].value, "dastore");
  op_codes_array[82].arguments = 0;
  op_codes_array[82].references = 0;
  op_codes_array[82].eval = dastore_eval;

  strcpy(op_codes_array[83].value, "aastore");
  op_codes_array[83].arguments = 0;
  op_codes_array[83].references = 0;
  op_codes_array[83].eval = aastore_eval;

  strcpy(op_codes_array[84].value, "bastore");
  op_codes_array[84].arguments = 0;
  op_codes_array[84].references = 0;
  op_codes_array[84].eval = bastore_eval;

  strcpy(op_codes_array[85].value, "castore");
  op_codes_array[85].arguments = 0;
  op_codes_array[85].references = 0;
  op_codes_array[85].eval = castore_eval;

  strcpy(op_codes_array[86].value, "sastore");
  op_codes_array[86].arguments = 0;
  op_codes_array[86].references = 0;
  op_codes_array[86].eval = sastore_eval;

  strcpy(op_codes_array[87].value, "pop");
  op_codes_array[87].arguments = 0;
  op_codes_array[87].references = 0;
  op_codes_array[87].eval = pop_eval;

  strcpy(op_codes_array[88].value, "pop2");
  op_codes_array[88].arguments = 0;
  op_codes_array[88].references = 0;
  op_codes_array[88].eval = pop2_eval;

  strcpy(op_codes_array[89].value, "dup");
  op_codes_array[89].arguments = 0;
  op_codes_array[89].references = 0;
  op_codes_array[89].eval = dup_eval;

  strcpy(op_codes_array[90].value, "dup_x1");
  op_codes_array[90].arguments = 0;
  op_codes_array[90].references = 0;
  op_codes_array[90].eval = dup_x1_eval;

  strcpy(op_codes_array[91].value, "dup_x2");
  op_codes_array[91].arguments = 0;
  op_codes_array[91].references = 0;
  op_codes_array[91].eval = dup_x2_eval;

  strcpy(op_codes_array[92].value, "dup2");
  op_codes_array[92].arguments = 0;
  op_codes_array[92].references = 0;
  op_codes_array[92].eval = dup2_eval;

  strcpy(op_codes_array[93].value, "dup2_x1");
  op_codes_array[93].arguments = 0;
  op_codes_array[93].references = 0;
  op_codes_array[93].eval = dup2_x1_eval;

  strcpy(op_codes_array[94].value, "dup2_x2");
  op_codes_array[94].arguments = 0;
  op_codes_array[94].references = 0;
  op_codes_array[94].eval = dup2_x2_eval;

  strcpy(op_codes_array[95].value, "swap");
  op_codes_array[95].arguments = 0;
  op_codes_array[95].references = 0;
  op_codes_array[95].eval = swap_eval;

  strcpy(op_codes_array[96].value, "iadd");
  op_codes_array[96].arguments = 0;
  op_codes_array[96].references = 0;
  op_codes_array[96].eval = iadd_eval;

  strcpy(op_codes_array[97].value, "ladd");
  op_codes_array[97].arguments = 0;
  op_codes_array[97].references = 0;
  op_codes_array[97].eval = ladd_eval;

  strcpy(op_codes_array[98].value, "fadd");
  op_codes_array[98].arguments = 0;
  op_codes_array[98].references = 0;
  op_codes_array[98].eval = fadd_eval;

  strcpy(op_codes_array[99].value, "dadd");
  op_codes_array[99].arguments = 0;
  op_codes_array[99].references = 0;
  op_codes_array[99].eval = dadd_eval;

  strcpy(op_codes_array[100].value, "isub");
  op_codes_array[100].arguments = 0;
  op_codes_array[100].references = 0;
  op_codes_array[100].eval = isub_eval;

  strcpy(op_codes_array[101].value, "lsub");
  op_codes_array[101].arguments = 0;
  op_codes_array[101].references = 0;
  op_codes_array[101].eval = lsub_eval;

  strcpy(op_codes_array[102].value, "fsub");
  op_codes_array[102].arguments = 0;
  op_codes_array[102].references = 0;
  op_codes_array[102].eval = fsub_eval;

  strcpy(op_codes_array[103].value, "dsub");
  op_codes_array[103].arguments = 0;
  op_codes_array[103].references = 0;
  op_codes_array[103].eval = dsub_eval;

  strcpy(op_codes_array[104].value, "imul");
  op_codes_array[104].arguments = 0;
  op_codes_array[104].references = 0;
  op_codes_array[104].eval = imul_eval;

  strcpy(op_codes_array[105].value, "lmul");
  op_codes_array[105].arguments = 0;
  op_codes_array[105].references = 0;
  op_codes_array[105].eval = lmul_eval;

  strcpy(op_codes_array[106].value, "fmul");
  op_codes_array[106].arguments = 0;
  op_codes_array[106].references = 0;
  op_codes_array[106].eval = fmul_eval;

  strcpy(op_codes_array[107].value, "dmul");
  op_codes_array[107].arguments = 0;
  op_codes_array[107].references = 0;
  op_codes_array[107].eval = dmul_eval;

  strcpy(op_codes_array[108].value, "idiv");
  op_codes_array[108].arguments = 0;
  op_codes_array[108].references = 0;
  op_codes_array[108].eval = idiv_eval;

  strcpy(op_codes_array[109].value, "ldiv");
  op_codes_array[109].arguments = 0;
  op_codes_array[109].references = 0;
  op_codes_array[109].eval = ldiv_eval;

  strcpy(op_codes_array[110].value, "fdiv");
  op_codes_array[110].arguments = 0;
  op_codes_array[110].references = 0;
  op_codes_array[110].eval = fdiv_eval;

  strcpy(op_codes_array[111].value, "ddiv");
  op_codes_array[111].arguments = 0;
  op_codes_array[111].references = 0;
  op_codes_array[111].eval = ddiv_eval;

  strcpy(op_codes_array[112].value, "irem");
  op_codes_array[112].arguments = 0;
  op_codes_array[112].references = 0;
  op_codes_array[112].eval = irem_eval;

  strcpy(op_codes_array[113].value, "lrem");
  op_codes_array[113].arguments = 0;
  op_codes_array[113].references = 0;
  op_codes_array[113].eval = lrem_eval;

  strcpy(op_codes_array[114].value, "frem");
  op_codes_array[114].arguments = 0;
  op_codes_array[114].references = 0;
  op_codes_array[114].eval = frem_eval;

  strcpy(op_codes_array[115].value, "drem");
  op_codes_array[115].arguments = 0;
  op_codes_array[115].references = 0;
  op_codes_array[115].eval = drem_eval;

  strcpy(op_codes_array[116].value, "ineg");
  op_codes_array[116].arguments = 0;
  op_codes_array[116].references = 0;
  op_codes_array[116].eval = ineg_eval;

  strcpy(op_codes_array[117].value, "lneg");
  op_codes_array[117].arguments = 0;
  op_codes_array[117].references = 0;
  op_codes_array[117].eval = lneg_eval;

  strcpy(op_codes_array[118].value, "fneg");
  op_codes_array[118].arguments = 0;
  op_codes_array[118].references = 0;
  op_codes_array[118].eval = fneg_eval;

  strcpy(op_codes_array[119].value, "dneg");
  op_codes_array[119].arguments = 0;
  op_codes_array[119].references = 0;
  op_codes_array[119].eval = dneg_eval;

  strcpy(op_codes_array[120].value, "ishl");
  op_codes_array[120].arguments = 0;
  op_codes_array[120].references = 0;
  op_codes_array[120].eval = ishl_eval;

  strcpy(op_codes_array[121].value, "lshl");
  op_codes_array[121].arguments = 0;
  op_codes_array[121].references = 0;
  op_codes_array[121].eval = lshl_eval;

  strcpy(op_codes_array[122].value, "ishr");
  op_codes_array[122].arguments = 0;
  op_codes_array[122].references = 0;
  op_codes_array[122].eval = ishr_eval;

  strcpy(op_codes_array[123].value, "lshr");
  op_codes_array[123].arguments = 0;
  op_codes_array[123].references = 0;
  op_codes_array[123].eval = lshr_eval;

  strcpy(op_codes_array[124].value, "iushr");
  op_codes_array[124].arguments = 0;
  op_codes_array[124].references = 0;
  op_codes_array[124].eval = iushr_eval;

  strcpy(op_codes_array[125].value, "lushr");
  op_codes_array[125].arguments = 0;
  op_codes_array[125].references = 0;
  op_codes_array[125].eval = lushr_eval;

  strcpy(op_codes_array[126].value, "iand");
  op_codes_array[126].arguments = 0;
  op_codes_array[126].references = 0;
  op_codes_array[126].eval = iand_eval;

  strcpy(op_codes_array[127].value, "land");
  op_codes_array[127].arguments = 0;
  op_codes_array[127].references = 0;
  op_codes_array[127].eval = land_eval;

  strcpy(op_codes_array[128].value, "ior");
  op_codes_array[128].arguments = 0;
  op_codes_array[128].references = 0;
  op_codes_array[128].eval = ior_eval;

  strcpy(op_codes_array[129].value, "lor");
  op_codes_array[129].arguments = 0;
  op_codes_array[129].references = 0;
  op_codes_array[129].eval = lor_eval;

  strcpy(op_codes_array[130].value, "ixor");
  op_codes_array[130].arguments = 0;
  op_codes_array[130].references = 0;
  op_codes_array[130].eval = ixor_eval;

  strcpy(op_codes_array[131].value, "lxor");
  op_codes_array[131].arguments = 0;
  op_codes_array[131].references = 0;
  op_codes_array[131].eval = lxor_eval;

  strcpy(op_codes_array[132].value, "iinc");
  op_codes_array[132].arguments = 2;
  op_codes_array[132].references = 0;
  op_codes_array[132].eval = iinc_eval;

  strcpy(op_codes_array[133].value, "i2l");
  op_codes_array[133].arguments = 0;
  op_codes_array[133].references = 0;
  op_codes_array[133].eval = i2l_eval;

  strcpy(op_codes_array[134].value, "i2f");
  op_codes_array[134].arguments = 0;
  op_codes_array[134].references = 0;
  op_codes_array[134].eval = i2f_eval;

  strcpy(op_codes_array[135].value, "i2d");
  op_codes_array[135].arguments = 0;
  op_codes_array[135].references = 0;
  op_codes_array[135].eval = i2d_eval;

  strcpy(op_codes_array[136].value, "l2i");
  op_codes_array[136].arguments = 0;
  op_codes_array[136].references = 0;
  op_codes_array[136].eval = l2i_eval;

  strcpy(op_codes_array[137].value, "l2f");
  op_codes_array[137].arguments = 0;
  op_codes_array[137].references = 0;
  op_codes_array[137].eval = l2f_eval;

  strcpy(op_codes_array[138].value, "l2d");
  op_codes_array[138].arguments = 0;
  op_codes_array[138].references = 0;
  op_codes_array[138].eval = l2d_eval;

  strcpy(op_codes_array[139].value, "f2i");
  op_codes_array[139].arguments = 0;
  op_codes_array[139].references = 0;
  op_codes_array[139].eval = f2i_eval;

  strcpy(op_codes_array[140].value, "f2l");
  op_codes_array[140].arguments = 0;
  op_codes_array[140].references = 0;
  op_codes_array[140].eval = f2l_eval;

  strcpy(op_codes_array[141].value, "f2d");
  op_codes_array[141].arguments = 0;
  op_codes_array[141].references = 0;
  op_codes_array[141].eval = f2d_eval;

  strcpy(op_codes_array[142].value, "d2i");
  op_codes_array[142].arguments = 0;
  op_codes_array[142].references = 0;
  op_codes_array[142].eval = d2i_eval;

  strcpy(op_codes_array[143].value, "d2l");
  op_codes_array[143].arguments = 0;
  op_codes_array[143].references = 0;
  op_codes_array[143].eval = d2l_eval;

  strcpy(op_codes_array[144].value, "d2f");
  op_codes_array[144].arguments = 0;
  op_codes_array[144].references = 0;
  op_codes_array[144].eval = d2f_eval;

  strcpy(op_codes_array[145].value, "i2b");
  op_codes_array[145].arguments = 0;
  op_codes_array[145].references = 0;
  op_codes_array[145].eval = i2b_eval;

  strcpy(op_codes_array[146].value, "i2c");
  op_codes_array[146].arguments = 0;
  op_codes_array[146].references = 0;
  op_codes_array[146].eval = i2c_eval;

  strcpy(op_codes_array[147].value, "i2s");
  op_codes_array[147].arguments = 0;
  op_codes_array[146].references = 0;
  op_codes_array[146].eval = i2s_eval;

  strcpy(op_codes_array[148].value, "lcmp");
  op_codes_array[148].arguments = 0;
  op_codes_array[148].references = 0;
  op_codes_array[148].eval = lcmp_eval;

  strcpy(op_codes_array[149].value, "fcmpl");
  op_codes_array[149].arguments = 0;
  op_codes_array[149].references = 0;
  op_codes_array[149].eval = fcmpl_eval;

  strcpy(op_codes_array[150].value, "fcmpg");
  op_codes_array[150].arguments = 0;
  op_codes_array[150].references = 0;
  op_codes_array[150].eval = fcmpg_eval;

  strcpy(op_codes_array[151].value, "dcmpl");
  op_codes_array[151].arguments = 0;
  op_codes_array[151].references = 0;
  op_codes_array[151].eval = dcmpl_eval;

  strcpy(op_codes_array[152].value, "dcmpg");
  op_codes_array[152].arguments = 0;
  op_codes_array[152].references = 0;
  op_codes_array[152].eval = dcmpg_eval;

  strcpy(op_codes_array[153].value, "ifeq");
  op_codes_array[153].arguments = 2;
  op_codes_array[153].references = 0;
  op_codes_array[153].eval = ifeq_eval;

  strcpy(op_codes_array[154].value, "ifne");
  op_codes_array[154].arguments = 2;
  op_codes_array[154].references = 0;
  op_codes_array[154].eval = ifne_eval;

  strcpy(op_codes_array[155].value, "iflt");
  op_codes_array[155].arguments = 2;
  op_codes_array[155].references = 0;
  op_codes_array[155].eval = iflt_eval;

  strcpy(op_codes_array[156].value, "ifge");
  op_codes_array[156].arguments = 2;
  op_codes_array[156].references = 0;
  op_codes_array[156].eval = ifge_eval;

  strcpy(op_codes_array[157].value, "ifgt");
  op_codes_array[157].arguments = 2;
  op_codes_array[157].references = 0;
  op_codes_array[157].eval = ifgt_eval;

  strcpy(op_codes_array[158].value, "ifle");
  op_codes_array[158].arguments = 2;
  op_codes_array[158].references = 0;
  op_codes_array[158].eval = ifle_eval;

  strcpy(op_codes_array[159].value, "if_icmpeq");
  op_codes_array[159].arguments = 2;
  op_codes_array[159].references = 0;
  op_codes_array[159].eval = if_icmpeq_eval;

  strcpy(op_codes_array[160].value, "if_icmpne");
  op_codes_array[160].arguments = 2;
  op_codes_array[160].references = 0;
  op_codes_array[160].eval = if_icmpne_eval;

  strcpy(op_codes_array[161].value, "if_icmplt");
  op_codes_array[161].arguments = 2;
  op_codes_array[161].references = 0;
  op_codes_array[161].eval = if_icmplt_eval;

  strcpy(op_codes_array[162].value, "if_icmpge");
  op_codes_array[162].arguments = 2;
  op_codes_array[162].references = 0;
  op_codes_array[162].eval = if_icmpge_eval;

  strcpy(op_codes_array[163].value, "if_icmpgt");
  op_codes_array[163].arguments = 2;
  op_codes_array[163].references = 0;
  op_codes_array[163].eval = if_icmpgt_eval;

  strcpy(op_codes_array[164].value, "if_icmple");
  op_codes_array[164].arguments = 2;
  op_codes_array[164].references = 0;
  op_codes_array[164].eval = if_icmple_eval;

  strcpy(op_codes_array[165].value, "if_acmpeq");
  op_codes_array[165].arguments = 2;
  op_codes_array[165].references = 0;
  op_codes_array[165].eval = if_acmpeq_eval;

  strcpy(op_codes_array[166].value, "if_acmpne");
  op_codes_array[166].arguments = 2;
  op_codes_array[166].references = 0;
  op_codes_array[166].eval = if_acmpne_eval;

  strcpy(op_codes_array[167].value, "goto");
  op_codes_array[167].arguments = 2;
  op_codes_array[167].references = 0;
  op_codes_array[167].eval = goto_eval;

  strcpy(op_codes_array[168].value, "jsr");
  op_codes_array[168].arguments = 2;
  op_codes_array[168].references = 0;
  op_codes_array[168].eval = jsr_eval;

  strcpy(op_codes_array[169].value, "ret");
  op_codes_array[169].arguments = 1;
  op_codes_array[169].references = 0;
  op_codes_array[169].eval = ret_eval;

  strcpy(op_codes_array[170].value, "tableswitch");
  // instrução tem tamanho variável
  op_codes_array[170].arguments = 1;
  op_codes_array[170].eval = tableswitch_eval;

  op_codes_array[170].references = 0;

  strcpy(op_codes_array[171].value, "lookupswitch");
  // instrução tem tamanho variável
  op_codes_array[171].arguments = 1;
  op_codes_array[171].eval = lookupswitch_eval;

  op_codes_array[171].references = 0;

  strcpy(op_codes_array[172].value, "ireturn");
  op_codes_array[172].arguments = 0;
  op_codes_array[172].references = 0;
  op_codes_array[172].eval = ireturn_eval;

  strcpy(op_codes_array[173].value, "lreturn");
  op_codes_array[173].arguments = 0;
  op_codes_array[173].references = 0;
  op_codes_array[173].eval = lreturn_eval;

  strcpy(op_codes_array[174].value, "freturn");
  op_codes_array[174].arguments = 0;
  op_codes_array[174].references = 0;
  op_codes_array[174].eval = freturn_eval;

  strcpy(op_codes_array[175].value, "dreturn");
  op_codes_array[175].arguments = 0;
  op_codes_array[175].references = 0;
  op_codes_array[175].eval = dreturn_eval;

  strcpy(op_codes_array[176].value, "areturn");
  op_codes_array[176].arguments = 0;
  op_codes_array[176].references = 0;
  op_codes_array[176].eval = areturn_eval;

  strcpy(op_codes_array[177].value, "return");
  op_codes_array[177].arguments = 0;
  op_codes_array[177].references = 0;
  op_codes_array[177].eval = return_eval;

  strcpy(op_codes_array[178].value, "getstatic");
  op_codes_array[178].arguments = 2;
  op_codes_array[178].references = 1;
  op_codes_array[178].eval = getstatic_eval;

  strcpy(op_codes_array[179].value, "putstatic");
  op_codes_array[179].arguments = 2;
  op_codes_array[179].references = 1;
  op_codes_array[179].eval = putstatic_eval;

  strcpy(op_codes_array[180].value, "getfield");
  op_codes_array[180].arguments = 2;
  op_codes_array[180].references = 1;
  op_codes_array[180].eval = getfield_eval;

  strcpy(op_codes_array[181].value, "putfield");
  op_codes_array[181].arguments = 2;
  op_codes_array[181].references = 1;
  op_codes_array[181].eval = putfield_eval;

  strcpy(op_codes_array[182].value, "invokevirtual");
  op_codes_array[182].arguments = 2;
  op_codes_array[182].references = 1;
  op_codes_array[182].eval = invokevirtual_eval;

  strcpy(op_codes_array[183].value, "invokespecial");
  op_codes_array[183].arguments = 2;
  op_codes_array[183].references = 1;
  op_codes_array[183].eval = invokespecial_eval;

  strcpy(op_codes_array[184].value, "invokestatic");
  op_codes_array[184].arguments = 2;
  op_codes_array[184].references = 1;
  op_codes_array[184].eval = invokestatic_eval;

  strcpy(op_codes_array[185].value, "invokeinterface");
  op_codes_array[185].arguments = 4;
  op_codes_array[185].references = 1;
  op_codes_array[185].eval = invokeinterface_eval;

  strcpy(op_codes_array[186].value, "invokedynamic");
  op_codes_array[186].arguments = 4;
  op_codes_array[186].references = 1;
  op_codes_array[186].eval = invokedynamic_eval;

  strcpy(op_codes_array[187].value, "new");
  op_codes_array[187].arguments = 2;
  op_codes_array[187].references = 1;
  op_codes_array[187].eval = new_eval;

  strcpy(op_codes_array[188].value, "newarray");
  op_codes_array[188].arguments = 1;
  op_codes_array[188].references = 0;
  op_codes_array[188].eval = newarray_eval;

  strcpy(op_codes_array[189].value, "anewarray");
  op_codes_array[189].arguments = 2;
  op_codes_array[189].references = 1;
  op_codes_array[189].eval = anewarray_eval;

  strcpy(op_codes_array[190].value, "arraylength");
  op_codes_array[190].arguments = 0;
  op_codes_array[190].references = 0;
  op_codes_array[190].eval = arraylength_eval;

  strcpy(op_codes_array[191].value, "athrow");
  op_codes_array[191].arguments = 0;
  op_codes_array[191].references = 0;
  op_codes_array[191].eval = athrow_eval;

  strcpy(op_codes_array[192].value, "checkcast");
  op_codes_array[192].arguments = 2;
  op_codes_array[192].references = 1;
  op_codes_array[192].eval = checkcast_eval;

  strcpy(op_codes_array[193].value, "instanceof");
  op_codes_array[193].arguments = 2;
  op_codes_array[193].references = 1;
  op_codes_array[193].eval = instanceof_eval;

  strcpy(op_codes_array[194].value, "monitorenter");
  op_codes_array[194].arguments = 0;
  op_codes_array[194].references = 0;
  op_codes_array[194].eval = monitorenter_eval;

  strcpy(op_codes_array[195].value, "monitorexit");
  op_codes_array[195].arguments = 0;
  op_codes_array[195].references = 0;
  op_codes_array[195].eval = monitorexit_eval;

  strcpy(op_codes_array[196].value, "wide");
  op_codes_array[196].arguments = 3;
  op_codes_array[196].references = 0;
  op_codes_array[196].eval = wide_eval;

  strcpy(op_codes_array[197].value, "multianewarray");
  op_codes_array[197].arguments = 3;
  op_codes_array[197].references = 1;
  op_codes_array[197].eval = multianewarray_eval;

  strcpy(op_codes_array[198].value, "ifnull");
  op_codes_array[198].arguments = 2;
  op_codes_array[198].references = 0;
  op_codes_array[198].eval = ifnull_eval;

  strcpy(op_codes_array[199].value, "ifnonnull");
  op_codes_array[199].arguments = 2;
  op_codes_array[199].references = 0;
  op_codes_array[199].eval = ifnonnull_eval;

  strcpy(op_codes_array[200].value, "goto_w");
  op_codes_array[200].arguments = 4; //4
  op_codes_array[200].references = 0;
  op_codes_array[200].eval = goto_w_eval;

  strcpy(op_codes_array[201].value, "jsr_w");
  op_codes_array[201].arguments = 4; //4
  op_codes_array[201].references = 0;
  op_codes_array[201].eval = jsr_w_eval;

  // reservados
  strcpy(op_codes_array[202].value, "breakpoint");
  op_codes_array[202].arguments = 0;

  strcpy(op_codes_array[254].value, "impdep1");
  op_codes_array[254].arguments = 0;

  strcpy(op_codes_array[255].value, "impdep2");
  op_codes_array[255].arguments = 0;

  int i;
  for(i = 0; i < 256; i++)
  {
    op_codes_array[i].key = i;
  }
};