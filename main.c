/*
Projeto JVM para o Professor Doutor Ladeira
Universidade de Brasília - 01/2019
Software Básico - Turma A
Projeto Leitor e Exibidor de arquivo .class

Alunos: Nicholas Marques - 15/0019343
				Antônio Júnior - 16/0112745
				Yan Galli - 16/0149207
				Vinícius Costa e Silva - 15/0052138
				João Gabriel - 15/0131992
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "structures.h"
#include "instructions.h"

char *GLOBAL_ptr;
u1 code_sep;
char *FILE_NAME;

u1 u1Read(FILE *);
u2 u2Read(FILE *);
u4 u4Read(FILE *);

void read_class_file(ClassFile *, FILE *);
void print_class_file(ClassFile *cf);
void free_class_file(ClassFile *cf);
char *readUtf8(cp_info *cp, u2 index);
attribute_info *readAttributes(cp_info *cp, u2 attr_count, FILE *fp);
void printAttributes(attribute_info *field, cp_info *cp, u2 attr_count);
void recursive_print(cp_info *cp, u2 index, char *str);
char *print_reference(cp_info *cp, u2 index);
verification_type_info *fillVerificationTypeInfo(FILE *fp, u2 verification_type_length);
stack_map_frame *fillStackMapTable(attribute_info *attr, FILE *fp);
void freeAttributes(attribute_info *field, cp_info *cp, u2 attr_count);
void printVerificationTypeInfo(verification_type_info *ver_type, cp_info *cp, u2 verification_type_length);
void printStackMapTable(stack_map_frame *stack_map, cp_info *cp, attribute_info *attr);
void freeStackMapTable(stack_map_frame *stack_map, attribute_info *attr);
void printConstType(u4 high_bytes, u4 low_bytes, u1 type);
char *printFlag(u2 type, u1 flag);
char *printVersion(u2 version);

int main(int argc, char *argv[])
{
  FILE *pFile;
  regex_t reg;
  regcomp(&reg ,"(Test\/)?(.+?)(\.[^.]*$|$)", REG_EXTENDED|REG_NOSUB);

  if (argc != 2)
  {
    printf("Warning: Caminho do arquivo nao fornecido.");
    // pFile = fopen("Teste/multi.class", "rb");
    return 0;
  }
  else
  {
    pFile = fopen(argv[1], "rb");
  }

  ClassFile *cf = (ClassFile *)malloc(sizeof(ClassFile));
  GLOBAL_ptr = (char *)malloc(sizeof(char) * 100);

  read_class_file(cf, pFile);
  fclose(pFile);
  
  if ((regexec(&reg, print_reference(cf->constant_pool, cf->attributes->info->SourceFile_attribute.sourcefile_index), 0, (regmatch_t *)NULL, 0)) == 0)
  {
    initialize_op_codes();
    print_class_file(cf);
  }
  free(GLOBAL_ptr);
  free_class_file(cf);
  free(cf);
  return 0;
}

u1 u1Read(FILE *file)
{
  // u1 buffer;
  // fread(&buffer, sizeof(u1), 1, file);
  // return buffer;
  u1 toReturn = getc(file);
  return toReturn;
}

u2 u2Read(FILE *file)
{
  // u2 buffer;
  // fread(&buffer, sizeof(u2), 1, file);
  // return buffer;
  u2 toReturn = getc(file);
  toReturn = (toReturn << 8) | (getc(file));
  return toReturn;
}

u4 u4Read(FILE *file)
{
  // u4 buffer;
  // fread(&buffer, sizeof(u4), 1, file);
  // return buffer;
  u4 toReturn = getc(file);
  toReturn = (toReturn << 8) | (getc(file));
  toReturn = (toReturn << 8) | (getc(file));
  toReturn = (toReturn << 8) | (getc(file));
  return toReturn;
}

char *printVersion(u2 version)
{
  switch (version)
  {
  case 45:
    return "1.1";
    break;
  case 46:
    return "1.2";
    break;
  case 47:
    return "1.3";
    break;
  case 48:
    return "1.4";
    break;
  case 49:
    return "1.5";
    break;
  case 50:
    return "1.6";
    break;
  case 51:
    return "1.7";
    break;
  case 52:
    return "1.8";
    break;

  default:
    return "< 1.2 || > 1.8";
    break;
  }
}

char *readUtf8(cp_info *cp, u2 index)
{
  return (char *)(cp[index - 1]).Utf8.bytes;
}

void printConstType(u4 high_bytes, u4 low_bytes, u1 type)
{
  // char *str = (char *)malloc(sizeof(char) * 100);
  strcpy(GLOBAL_ptr, "");
  int64_t num;
  switch (type)
  {
  case CONSTANT_Integer:
    // printf("Integer Bytes: %02d \n", cp->Integer.bytes, str);
    snprintf(GLOBAL_ptr, 100, "%d", low_bytes);
    break;
  case CONSTANT_Float:
    // printf("Float Bytes: %02d \n", cp->Float.bytes, str);
    snprintf(GLOBAL_ptr, 100, "%.2f", *(float *)&low_bytes);
    break;
  case CONSTANT_Long:
    // printf("Long High Bytes: %02d \n", cp->Long.high_bytes, str);
    // printf("Long Low Bytes: %02d \n", cp->Long.low_bytes, str);
    num = (int64_t)high_bytes << 32 | (int64_t)low_bytes;
    snprintf(GLOBAL_ptr, 100, "%ld", *(long *)&num);
    break;
  case CONSTANT_Double:
    num = (int64_t)high_bytes << 32 | (int64_t)low_bytes;
    snprintf(GLOBAL_ptr, 100, "%.2lf", *(double *)&num);
    break;
  default:
    break;
  }
  // return str;
}

char *printType(u2 type)
{
  switch (type)
  {
  case T_BOOLEAN:
    return "(boolean)";
    break;
  case T_CHAR:
    return "(char)";
    break;
  case T_FLOAT:
    return "(float)";
    break;
  case T_DOUBLE:
    return "(double)";
    break;
  case T_BYTE:
    return "(byte)";
    break;
  case T_SHORT:
    return "(short)";
    break;
  case T_INT:
    return "(int)";
    break;
  case T_LONG:
    return "(long)";
    break;
  default:
    return "";
    break;
  }
}

char *printFlag(u2 type, u1 flag)
{
  switch (type)
  {
  case 0x0001: //  ACC_PUBLIC:
    return "[public]";
    break;

  case 9:
    return "[public static]";
    break;

  case 0x0010: //  ACC_FINAL:
    return "[final]";
    break;
    // = SYNCRONIZED :
  case 0x0020:
    if (flag) //  ACC_SUPER:
      return "[super]";
    else //  ACC_SYNCHRONIZED:
      return "[synchronized]";

    break;
  case 0x0200: //  ACC_INTERFACE:
    return "[interface]";
    break;
  case 0x0400: //  ACC_ABSTRACT:
    return "[abstract]";
    break;
  // case 0x0001 : //  ACC_PUBLIC:
  //   return "[]";
  //   break;
  case 0x0002: //  ACC_PRIVATE:
    return "[private]";
    break;
  case 0x0004: //  ACC_PROTECTED:
    return "[protect]";
    break;
  case 0x0008: //  ACC_STATIC:
    return "[static]";
    break;
  // case 0x0010 : //  ACC_FINAL:
  //   return "[]";
  //   break;
  case 0x0040: //  ACC_VOLATILE:
    return "[volatile]";
    break;
  case 0x0080: //  ACC_TRANSIENT:
    return "[transient]";
    break;
  // case 0x0001 : //  ACC_PUBLIC:
  //   return "[]";
  //   break;
  // case 0x0002 : //  ACC_PRIVATE:
  //   return "[]";
  //   break;
  // case 0x0004 : //  ACC_PROTECTED:
  //   return "[]";
  //   break;
  // case 0x0008 : //  ACC_STATIC:
  //   return "[]";
  //   break;
  // case 0x0010 : //  ACC_FINAL:
  //   return "[]";
  //   break;

  // = SUPER
  // case 0x0020 : //  ACC_SYNCHRONIZED:
  //   return "[]";
  //   break;
  case 0x0100: //  ACC_NATIVE:
    return "[native]";
    break;
  // case 0x0400 : //  ACC_ABSTRACT:
  //   return "[]";
  //   break;
  case 0x0800: //  ACC_STRICT:
    return "[strict]";
    break;
  default:
    return "[]";
    break;
  }
}

attribute_info *readAttributes(cp_info *cp, u2 attr_count, FILE *fp)
{
  attribute_info *field = (attribute_info *)malloc(sizeof(attribute_info) * attr_count);
  for (attribute_info *attr = field; attr < field + attr_count; attr++)
  {
    attr->attribute_name_index = u2Read(fp);
    attr->attribute_length = u4Read(fp);
    attr->info = (attribute_types *)malloc(sizeof(attribute_types) * attr->attribute_length);

    char *attribute_name = readUtf8(cp, attr->attribute_name_index);

    if (strcmp(attribute_name, "Code") == 0)
    {
      attr->info->Code_attribute.max_stack = u2Read(fp);
      attr->info->Code_attribute.max_locals = u2Read(fp);
      attr->info->Code_attribute.code_length = u4Read(fp);
      attr->info->Code_attribute.code = (u1 *)malloc(sizeof(u1) * attr->info->Code_attribute.code_length);
      for (u1 *i = attr->info->Code_attribute.code; i < attr->info->Code_attribute.code + attr->info->Code_attribute.code_length; i++)
      {
        *i = u1Read(fp);
      }

      attr->info->Code_attribute.exception_table_length = u2Read(fp);
      attr->info->Code_attribute.exception_table = (exception_table_type *)malloc(sizeof(exception_table_type) * attr->info->Code_attribute.exception_table_length);
      for (exception_table_type *i = attr->info->Code_attribute.exception_table; i < attr->info->Code_attribute.exception_table + attr->info->Code_attribute.exception_table_length; i++)
      {
        i->start_pc = u2Read(fp);
        i->end_pc = u2Read(fp);
        i->handler_pc = u2Read(fp);
        i->catch_type = u2Read(fp);
      }
      attr->info->Code_attribute.attributes_count = u2Read(fp);

      attr->info->Code_attribute.attributes = readAttributes(cp, attr->info->Code_attribute.attributes_count, fp);
    }
    else if (strcmp(attribute_name, "Exceptions") == 0)
    {
      attr->info->Exceptions_attribute.number_of_exceptions = u2Read(fp);
      attr->info->Exceptions_attribute.exception_index_table = (u2 *)malloc(sizeof(u2) * attr->info->Exceptions_attribute.number_of_exceptions);
      for (u2 *i = 0; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
      {
        *i = u2Read(fp);
      }
    }
    else if (strcmp(attribute_name, "Deprecated") == 0)
    {
      /* code */
    }
    else if (strcmp(attribute_name, "SourceFile") == 0)
    {
      attr->info->SourceFile_attribute.sourcefile_index = u2Read(fp);
    }
    else if (strcmp(attribute_name, "LineNumberTable") == 0)
    {
      attr->info->LineNumberTable_attribute.line_number_table_length = u2Read(fp);
      attr->info->LineNumberTable_attribute.line_number_table = (line_number_table_type *)malloc(sizeof(line_number_table_type) * attr->info->LineNumberTable_attribute.line_number_table_length);
      for (line_number_table_type *i = attr->info->LineNumberTable_attribute.line_number_table; i < attr->info->LineNumberTable_attribute.line_number_table + attr->info->LineNumberTable_attribute.line_number_table_length; i++)
      {
        i->start_pc = u2Read(fp);
        i->line_number = u2Read(fp);
      }
    }
    // else if (strcmp(attribute_name, "StackMapTable") == 0)
    // {
    //   attr->info->StackMapTable_attribute.number_of_entries = u2Read(fp);
    //   attr->info->StackMapTable_attribute.entries = fillStackMapTable(attr, fp);
    // }
    else if (strcmp(attribute_name, "BootstrapMethods") == 0)
    {
      attr->info->BootstrapMethods_attribute.num_bootstrap_methods = u2Read(fp);
      attr->info->BootstrapMethods_attribute.bootstrap_methods = (Bootstrap_method *)malloc(sizeof(Bootstrap_method) * attr->info->BootstrapMethods_attribute.num_bootstrap_methods);
      for (Bootstrap_method *bm = attr->info->BootstrapMethods_attribute.bootstrap_methods; bm < attr->info->BootstrapMethods_attribute.bootstrap_methods + attr->info->BootstrapMethods_attribute.num_bootstrap_methods; bm++)
      {
        bm->bootstrap_method_ref = u2Read(fp);
        bm->num_bootstrap_arguments = u2Read(fp);
        bm->bootstrap_arguments = (u2 *)malloc(sizeof(u2) * bm->num_bootstrap_arguments);
        for (u2 *i = bm->bootstrap_arguments; i < bm->bootstrap_arguments + bm->num_bootstrap_arguments; i++)
        {
          *i = u2Read(fp);
        }
      }
    }
    else if (strcmp(attribute_name, "ConstantValue") == 0)
    {
      attr->info->ConstantValue_attribute.constantvalue_index = u2Read(fp);
    }
    else if (strcmp(attribute_name, "LocalVariableTable") == 0)
    {
      attr->info->LocalVariableTable_attribute.local_variable_table_length = u2Read(fp);
      attr->info->LocalVariableTable_attribute.local_variable_table = (Local_variable_table *)malloc(sizeof(Local_variable_table) * attr->info->LocalVariableTable_attribute.local_variable_table_length);
      for (Local_variable_table *i = attr->info->LocalVariableTable_attribute.local_variable_table; i < attr->info->LocalVariableTable_attribute.local_variable_table + attr->info->LocalVariableTable_attribute.local_variable_table_length; i++)
      {
        i->start_pc = u2Read(fp);
        i->length = u2Read(fp);
        i->name_index = u2Read(fp);
        i->descriptor_index = u2Read(fp);
        i->index = u2Read(fp);
      }
    }
    else if (strcmp(attribute_name, "Synthetic") == 0)
    {
      /* code */
    }
    else if (strcmp(attribute_name, "InnerClasses") == 0)
    {
      attr->info->InnerClasses_attribute.number_of_classes = u2Read(fp);
      attr->info->InnerClasses_attribute.classes = (Classes *)malloc(sizeof(Classes) * attr->info->InnerClasses_attribute.number_of_classes);
      for (Classes *i = attr->info->InnerClasses_attribute.classes; i < attr->info->InnerClasses_attribute.number_of_classes + attr->info->InnerClasses_attribute.classes; i++)
      {
        i->inner_class_info_index = u2Read(fp);
        i->outer_class_info_index = u2Read(fp);
        i->inner_name_index = u2Read(fp);
        i->inner_class_access_flags = u2Read(fp);
      }
    }
    else // caso o atributo não esteja implementado ele é ignorado.
    {
      printf("Attributo Ignorado! \n");
      fseek(fp, attr->attribute_length, SEEK_CUR);
    }
  }
  return field;
}

void printVerificationTypeInfo(verification_type_info *ver_type, cp_info *cp, u2 verification_type_length)
{

  for (verification_type_info *vp = ver_type; vp < ver_type + verification_type_length; vp++)
  {

    switch (vp->tag)
    {

    case ITEM_Top:
      printf("TOP \n");
      break;
    case ITEM_Integer:
      printf("INTEGER \n");
      break;
    case ITEM_Float:
      printf("FLOAT \n");
      break;
    case ITEM_Double:
      printf("FLOAT \n");
      break;
    case ITEM_Long:
      printf("LONG \n");
      break;
    case ITEM_Null:
      printf("NULL \n");
      break;
    case ITEM_UninitializedThis:
      printf("UNINITIALIZED THIS \n");
      break;
    case ITEM_Object:
      printf("OBJECT cp_info #%d <%s> \n", vp->object_variable_info.cpool_index, print_reference(cp, vp->object_variable_info.cpool_index));
      break;
    case ITEM_Uninitialized:
      printf("UNINITIALIZED offset %02x \n", vp->uninitialized_variable_info.offset);
      break;

    default:
      break;
    }
  }
}

verification_type_info *fillVerificationTypeInfo(FILE *fp, u2 verification_type_length)
{
  verification_type_info *ver_type = (verification_type_info *)malloc(sizeof(verification_type_info));

  for (verification_type_info *vp = ver_type; vp < ver_type + verification_type_length; vp++)
  {
    vp->tag = u1Read(fp);

    if (vp->tag == 7) //Object_variable_info
    {
      vp->object_variable_info.cpool_index = u2Read(fp);
    }
    else if (vp->tag == 8) //Uninitialized_variable_info
    {
      vp->uninitialized_variable_info.offset = u2Read(fp);
    }
  }

  return ver_type;
}

void printStackMapTable(stack_map_frame *stack_map, cp_info *cp, attribute_info *attr)
{

  u2 pc = 0;
  for (stack_map_frame *smp = stack_map; smp < stack_map + attr->info->StackMapTable_attribute.number_of_entries; smp++)
  {
    u2 index = smp - stack_map;
    // printf("%02x \n", smp->frame_type);
    if (smp->frame_type < 64) // 0 a 63
    {
      //continue
      printf("%d | SAME(%d), Offset: %d (%d) \n", index, smp->frame_type, (pc += smp->frame_type) + index, smp->frame_type);
    }
    else if (smp->frame_type < 128) // 64 a 127
    {
      printVerificationTypeInfo(stack_map->same_locals_1_stack_item_frame.stack, cp, 2);
    }
    else if (smp->frame_type < 247) // 128 a 246
    {
      // for future use
    }
    else if (smp->frame_type < 248) // = 247
    {
      printf("%02x", stack_map->same_locals_1_stack_item_frame_extended.offset_delta);
      // printf("%02x", );
      printVerificationTypeInfo(stack_map->same_locals_1_stack_item_frame_extended.stack, cp, 2);
    }
    else if (smp->frame_type < 251) // 248 a 250
    {
      printf("%d | CHOP(%d), Offset: %d (%d) \n", (u2)(smp - stack_map), smp->frame_type, (pc += stack_map->chop_frame.offset_delta) + index, stack_map->chop_frame.offset_delta);
    }
    else if (smp->frame_type < 252) // = 251
    {
      printf("%02x \n", stack_map->same_frame_extended.offset_delta);
    }
    else if (smp->frame_type < 255) // 252 a 254
    {
      printf("%d | APPEND(%d), Offset: %d (%d) \n", (u2)(smp - stack_map), smp->frame_type, (pc += stack_map->append_frame.offset_delta) + index, stack_map->append_frame.offset_delta);
      // printf("%02x \n", stack_map->append_frame.offset_delta);
      printf("  Local verifications: \n");
      printVerificationTypeInfo(stack_map->append_frame.locals, cp, smp->frame_type - 251);
    }
    else
    { // = 255

      printf("%d | FULL (%d), Offset: %d (%d) \n", (u2)(smp - stack_map), smp->frame_type, (pc += stack_map->full_frame.offset_delta) + index, stack_map->full_frame.offset_delta);
      // printf("%02x \n", stack_map->full_frame.number_of_locals);
      printf("  Local verifications: \n");
      printVerificationTypeInfo(stack_map->full_frame.locals, cp, stack_map->full_frame.number_of_locals);
      // printf("%02x \n", stack_map->full_frame.number_of_stack_items);
      printf("  Stack verifications: \n");
      printVerificationTypeInfo(stack_map->full_frame.stack, cp, stack_map->full_frame.number_of_stack_items);
    }
  }
}

stack_map_frame *fillStackMapTable(attribute_info *attr, FILE *fp)
{
  stack_map_frame *stack_map = (stack_map_frame *)malloc(sizeof(stack_map_frame) * attr->info->StackMapTable_attribute.number_of_entries);

  for (stack_map_frame *smp = stack_map; smp < stack_map + attr->info->StackMapTable_attribute.number_of_entries; smp++)
  {
    smp->frame_type = u1Read(fp);
    if (smp->frame_type < 64) // 0 a 63
    {
      //continue
    }
    else if (smp->frame_type < 128) // 64 a 127
    {
      stack_map->same_locals_1_stack_item_frame.stack = fillVerificationTypeInfo(fp, 2);
    }
    else if (smp->frame_type < 247) // 128 a 246
    {
      // for future use
    }
    else if (smp->frame_type < 248) // = 247
    {
      stack_map->same_locals_1_stack_item_frame_extended.offset_delta = u2Read(fp);
      stack_map->same_locals_1_stack_item_frame_extended.stack = fillVerificationTypeInfo(fp, 2);
    }
    else if (smp->frame_type < 251) // 248 a 250
    {
      stack_map->chop_frame.offset_delta = u2Read(fp);
    }
    else if (smp->frame_type < 252) // = 251
    {
      stack_map->same_frame_extended.offset_delta = u2Read(fp);
    }
    else if (smp->frame_type < 255) // 252 a 254
    {
      stack_map->append_frame.offset_delta = u2Read(fp);
      stack_map->append_frame.locals = fillVerificationTypeInfo(fp, smp->frame_type - 251);
    }
    else
    { // = 255
      stack_map->full_frame.offset_delta = u2Read(fp);
      stack_map->full_frame.number_of_locals = u2Read(fp);
      stack_map->full_frame.locals = fillVerificationTypeInfo(fp, stack_map->full_frame.number_of_locals);
      stack_map->full_frame.number_of_stack_items = u2Read(fp);
      stack_map->full_frame.stack = fillVerificationTypeInfo(fp, stack_map->full_frame.number_of_stack_items);
    }
  }
  return stack_map;
}

void freeStackMapTable(stack_map_frame *stack_map, attribute_info *attr)
{

  for (stack_map_frame *smp = stack_map; smp < stack_map + attr->info->StackMapTable_attribute.number_of_entries; smp++)
  {
    if (smp->frame_type < 64) // 0 a 63
    {
      //continue
    }
    else if (smp->frame_type < 128) // 64 a 127
    {
      free(stack_map->same_locals_1_stack_item_frame.stack);
    }
    else if (smp->frame_type < 247) // 128 a 246
    {
      // for future use
    }
    else if (smp->frame_type < 248) // = 247
    {
      // stack_map->same_locals_1_stack_item_frame_extended.offset_delta = u2Read(fp);
      free(stack_map->same_locals_1_stack_item_frame_extended.stack);
    }
    else if (smp->frame_type < 251) // 248 a 250
    {
      // stack_map->chop_frame.offset_delta = u2Read(fp);
    }
    else if (smp->frame_type < 252) // = 251
    {
      // stack_map->same_frame_extended.offset_delta = u2Read(fp);
    }
    else if (smp->frame_type < 255) // 252 a 254
    {
      // stack_map->append_frame.offset_delta = u2Read(fp);
      free(stack_map->append_frame.locals);
    }
    else // = 255
    {
      // stack_map->full_frame.offset_delta = u2Read(fp);
      // stack_map->full_frame.number_of_locals = u2Read(fp);
      free(stack_map->full_frame.locals);
      // stack_map->full_frame.number_of_stack_items = u2Read(fp);
      free(stack_map->full_frame.stack);
    }
  }
  free(stack_map);
}

void freeAttributes(attribute_info *field, cp_info *cp, u2 attr_count)
{
  // attribute_info *field = (attribute_info *)malloc(sizeof(attribute_info) * attr_count);
  for (attribute_info *attr = field; attr < field + attr_count; attr++)
  {
    // attr->info = (attribute_types *)malloc(sizeof(attribute_types) * attr->attribute_length);

    char *attribute_name = readUtf8(cp, attr->attribute_name_index);

    if (strcmp(attribute_name, "Code") == 0)
    {
      // attr->info->Code_attribute.code = (u1 *)malloc(sizeof(u1) * attr->info->Code_attribute.code_length);
      free(attr->info->Code_attribute.code);

      // for (u1 *i = attr->info->Code_attribute.code; i < attr->info->Code_attribute.code + attr->info->Code_attribute.code_length; i++)
      // {
      //   *i = u1Read(fp);
      // }

      // attr->info->Code_attribute.exception_table_length = u2Read(fp);
      // attr->info->Code_attribute.exception_table = (exception_table_type *)malloc(sizeof(exception_table_type) * attr->info->Code_attribute.exception_table_length);
      free(attr->info->Code_attribute.exception_table);

      // for (exception_table_type *i = attr->info->Code_attribute.exception_table; i < attr->info->Code_attribute.exception_table + attr->info->Code_attribute.exception_table_length; i++)
      // {
      //   i->start_pc = u2Read(fp);
      //   i->end_pc = u2Read(fp);
      //   i->handler_pc = u2Read(fp);
      //   i->catch_type = u2Read(fp);
      // }
      // attr->info->Code_attribute.attributes_count = u2Read(fp);

      freeAttributes(attr->info->Code_attribute.attributes, cp, attr->info->Code_attribute.attributes_count);
    }
    else if (strcmp(attribute_name, "Exceptions") == 0)
    {
      // attr->info->Exceptions_attribute.number_of_exceptions = u2Read(fp);
      // attr->info->Exceptions_attribute.exception_index_table = (u2 *)malloc(sizeof(u2) * attr->info->Exceptions_attribute.number_of_exceptions);
      free(attr->info->Exceptions_attribute.exception_index_table);

      // for (u2 *i = 0; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
      // {
      //   *i = u2Read(fp);
      // }
    }
    else if (strcmp(attribute_name, "Deprecated") == 0)
    {
      /* code */
    }
    else if (strcmp(attribute_name, "SourceFile") == 0)
    {
      // attr->info->SourceFile_attribute.sourcefile_index = u2Read(fp);
    }
    else if (strcmp(attribute_name, "LineNumberTable") == 0)
    {
      // attr->info->LineNumberTable_attribute.line_number_table_length = u2Read(fp);
      // attr->info->LineNumberTable_attribute.line_number_table = (line_number_table_type *)malloc(sizeof(line_number_table_type) * attr->info->LineNumberTable_attribute.line_number_table_length);
      free(attr->info->LineNumberTable_attribute.line_number_table);

      // for (line_number_table_type *i = attr->info->LineNumberTable_attribute.line_number_table; i < attr->info->LineNumberTable_attribute.line_number_table + attr->info->LineNumberTable_attribute.line_number_table_length; i++)
      // {
      //   i->start_pc = u2Read(fp);
      //   i->line_number = u2Read(fp);
      // }
    }
    else if (strcmp(attribute_name, "ConstantValue") == 0)
    {
      // attr->info->ConstantValue_attribute.constantvalue_index = u2Read(fp);
    }
    else if (strcmp(attribute_name, "StackMapTable") == 0)
    {
      // attr->info->StackMapTable_attribute.number_of_entries = u2Read(fp);
      freeStackMapTable(attr->info->StackMapTable_attribute.entries, attr);
    }
    else
    {
      /* code */
    }
    free(attr->info);
  }
  free(field);
}

void printAttributes(attribute_info *field, cp_info *cp, u2 attr_count)
{
  for (attribute_info *attr = field; attr < field + attr_count; attr++)
  {
    char *attribute_name = readUtf8(cp, attr->attribute_name_index);

    printf("  [%ld] %s \n", attr - field, attribute_name);
    printf("Attribute Name: cp_info #%d <%s> \n", attr->attribute_name_index, attribute_name);
    printf("Attribute Length: %d \n", attr->attribute_length);

    if (strcmp(attribute_name, "Code") == 0)
    {
      printf("Max stack: %d\n", attr->info->Code_attribute.max_stack);
      printf("Max locals: %d\n", attr->info->Code_attribute.max_locals);
      printf("Code length: %d\n", attr->info->Code_attribute.code_length);
      code_sep = 1;
      printf("Code: \n");
      for (u1 *i = attr->info->Code_attribute.code; i < attr->info->Code_attribute.code + attr->info->Code_attribute.code_length; i++)
      {
        u2 pc = i - attr->info->Code_attribute.code;
        printf("%02d ", pc);
        u1 *index;
        printf("%s ", op_codes_array[*i].value);
        if (op_codes_array[*i].arguments)
        {
          // args_num = op_codes_array[*i].arguments;
          index = i;
          for (size_t j = 0; j < op_codes_array[*index].arguments; j++)
          {
            i++;

            if (op_codes_array[*index].references && op_codes_array[*index].arguments == 2)
            {
              // printf("%s", readUtf8(cp, ));
              // printf("Aqui!! %02x %s", *i, );
              u2 arg = *i << 8 | *(++i);
              printf("#%d <%s> ", arg, print_reference(cp, arg));
              j++;
            }
            else if (*index == wide)
            {
              uint8_t opcode = *i++;
              uint16_t indexbyte1 = *i++;
              uint16_t indexbyte2 = *i++;
              uint16_t result = (indexbyte1 << 8) | indexbyte2;
              printf("%s ", op_codes_array[opcode].value);
              printf("%d ", result);
              if (opcode == iinc)
              {
                uint16_t constbyte1 = *i++;
                uint16_t constbyte2 = *i++;
                result = (constbyte1 << 8) | constbyte2;
                printf(" by %d ", (int16_t)result);
              }
            }
            else if (op_codes_array[*index].arguments == 4)
            {

              u4 arg = 0; // = *i << 8 | *(++i);
              for (size_t k = 0; k < 4; k++)
              {
                arg = arg << 8 | *i++;
              }
              printf("#%d ", arg);
              if (op_codes_array[*index].references)
              {
                printf("<%s> ", print_reference(cp, arg));
              }
            }
            else if (*index == ldc)
            {
              u2 arg = 0x0 << 8 | *i;
              printf("#%d <%s> ", arg, print_reference(cp, arg));
              j++;
            }
            else if (*index == tableswitch)
            {
              u1 padding = pc % 4;
              i += padding + 1;
              int32_t default_v = 0, low_v = 0, high_v = 0, bytes = 0;
              for (size_t k = 0; k < 4; k++)
              {
                default_v = default_v << 8 | *i++;
              }
              for (size_t k = 0; k < 4; k++)
              {
                low_v = low_v << 8 | *i++;
              }
              for (size_t k = 0; k < 4; k++)
              {
                high_v = high_v << 8 | *i++;
              }

              printf("%d to %d \n", low_v, high_v);

              for (u2 w = 0; w < high_v - low_v + 1; w++)
              {
                for (size_t v = 0; v < 4; v++)
                {
                  bytes = bytes << 8 | *i++;
                }
                printf("  %d: %d  (%d) \n", w, pc + bytes, bytes);
              }
              printf("  default: %d  (%d) ", pc + default_v, default_v);
              // printf(" Aqui> %d %d %d \n", default_v, low_v, high_v);
            }
            else if (*index == lookupswitch)
            {
              u1 padding = pc % 4;
              i += padding + 1;
              int32_t default_v = 0, n_pairs = 0, bytes = 0, offset = 0;

              for (size_t k = 0; k < 4; k++)
              {
                default_v = default_v << 8 | *i++;
              }
              // printf(" %d ", default_v);
              for (size_t k = 0; k < 4; k++)
              {
                n_pairs = n_pairs << 8 | *i++;
              }
              printf(" %d \n", n_pairs);
              for (u2 w = 0; w < n_pairs; w++)
              {
                for (size_t v = 0; v < 4; v++)
                {
                  bytes = bytes << 8 | *i++;
                }
                for (size_t v = 0; v < 4; v++)
                {
                  offset = offset << 8 | *i++;
                }
                printf("  %d: %d  (%d) \n", bytes, pc + offset, offset);
              }
              printf("  default: %d  (%d) ", pc + default_v, default_v);
            }
            else if (*index == multianewarray)
            {
              u2 arg = *i << 8 | *(++i);
              printf("#%d <%s> dim %d ", arg, print_reference(cp, arg), *(++i));
              j += 2;
            }
            else if ((*index >= ifeq && *index <= jsr) || *index == ifnonnull || *index == ifnull)
            {
              int16_t addr = *i << 8 | *(++i);
              printf("%d (%d) ", (u2)(index - attr->info->Code_attribute.code) + addr, addr);
              j++;
            }
            else
            {
              if (*index == bipush)
              {
                printf("%02d ", (int8_t)*i);
              }
              else
              {

                printf("%02d ", *i);
              }
            }
            if (*index == newarray)
            {
              printf("%s ", printType(*i));
            }
          }
        }
        printf("\n");
      }
      code_sep = 0;

      printf("\n");

      // printf("Exception length: %d\n", attr->info->Code_attribute.exception_table_length);

      for (exception_table_type *i = attr->info->Code_attribute.exception_table; i < attr->info->Code_attribute.exception_table + attr->info->Code_attribute.exception_table_length; i++)
      {
        printf("Start pc: %d \n", i->start_pc);
        printf("End pc: %d \n", i->end_pc);
        printf("handler Pc: %d \n", i->handler_pc);
        printf("Catch type: %d \n", i->catch_type);
      }
      // printf("Attribute Count: %d\n", attr->info->Code_attribute.attributes_count);

      printAttributes(attr->info->Code_attribute.attributes, cp, attr->info->Code_attribute.attributes_count);
    }
    else if (strcmp(attribute_name, "Exceptions") == 0)
    {
      printf("Number of exceptions: %d\n", attr->info->Exceptions_attribute.number_of_exceptions);

      for (u2 *i = 0; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
      {
        printf("Exception index table: %d\n", *i);
      }
    }
    else if (strcmp(attribute_name, "Deprecated") == 0)
    {
      /* code */
    }
    else if (strcmp(attribute_name, "SourceFile") == 0)
    {
      printf("Source file: cp_info #%d <%s> \n", attr->info->SourceFile_attribute.sourcefile_index, print_reference(cp, attr->info->SourceFile_attribute.sourcefile_index));
    }
    else if (strcmp(attribute_name, "LineNumberTable") == 0)
    {
      // printf("Line number table length: %d\n", attr->info->LineNumberTable_attribute.line_number_table_length);

      for (line_number_table_type *i = attr->info->LineNumberTable_attribute.line_number_table; i < attr->info->LineNumberTable_attribute.line_number_table + attr->info->LineNumberTable_attribute.line_number_table_length; i++)
      {
        printf("Nr. %d\t", (u2)(i - attr->info->LineNumberTable_attribute.line_number_table));
        printf("Start pc: %d\t", i->start_pc);
        printf("Line number: %d\t\n", i->line_number);
      }
    }
    else if (strcmp(attribute_name, "StackMapTable") == 0)
    {
      printStackMapTable(attr->info->StackMapTable_attribute.entries, cp, attr);
    }
    else if (strcmp(attribute_name, "ConstantValue") == 0)
    {
      printf("Constant value: cp_info #%d <%s> \n", attr->info->ConstantValue_attribute.constantvalue_index, print_reference(cp, attr->info->ConstantValue_attribute.constantvalue_index));
    }
    else
    {
      /* code */
    }
  }
}

void read_class_file(ClassFile *cf, FILE *fp)
{
  cf->magic = u4Read(fp);
  if (cf->magic != 0xCAFEBABE)
  {
    printf("Esse arquivo nao é .class \n");
    free(cf);
    fclose(fp);
    exit(0);
  }
  cf->minor_version = u2Read(fp);
  cf->major_version = u2Read(fp);
  if (cf->major_version > 52)
  {
    printf("Versão do Java superior a 8 \n");
    free(cf);
    fclose(fp);
    exit(1);
  }

  cf->constant_pool_count = u2Read(fp);
  // constant_pool -> cp_info
  cf->constant_pool = (cp_info *)malloc(sizeof(cp_info) * (cf->constant_pool_count - 1));

  for (cp_info *cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; cp++)
  {
    cp->tag = u1Read(fp);
    // printf(" %d \n", cp->tag);
    switch (cp->tag)
    {
    case CONSTANT_Class:
      cp->Class.name_index = u2Read(fp);
      break;
    case CONSTANT_Fieldref:
      cp->Fieldref.class_index = u2Read(fp);
      cp->Fieldref.name_and_type_index = u2Read(fp);
      break;
    case CONSTANT_Methodref:
      cp->Methodref.class_index = u2Read(fp);
      cp->Methodref.name_and_type_index = u2Read(fp);
      break;
    case CONSTANT_InterfaceMethodref:
      cp->InterfaceMethodref.class_index = u2Read(fp);
      cp->InterfaceMethodref.name_and_type_index = u2Read(fp);
      break;
    case CONSTANT_String:
      cp->String.string_index = u2Read(fp);
      break;
    case CONSTANT_Integer:
      cp->Integer.bytes = u4Read(fp);
      break;
    case CONSTANT_Float:
      cp->Float.bytes = u4Read(fp);
      break;
    case CONSTANT_Long:
      cp->Long.high_bytes = u4Read(fp);
      cp->Long.low_bytes = u4Read(fp);
      cp++;
      break;
    case CONSTANT_Double:
      cp->Double.high_bytes = u4Read(fp);
      cp->Double.low_bytes = u4Read(fp);
      cp++;
      break;
    case CONSTANT_NameAndType:
      cp->NameAndType.name_index = u2Read(fp);
      cp->NameAndType.descriptor_index = u2Read(fp);
      break;
    case CONSTANT_Utf8:
      cp->Utf8.length = u2Read(fp);
      cp->Utf8.bytes = (u1 *)malloc(sizeof(u1) * (cp->Utf8.length + 1));
      for (u1 *aux = cp->Utf8.bytes; aux < cp->Utf8.length + cp->Utf8.bytes; aux++)
      {
        *aux = u1Read(fp);
      }
      cp->Utf8.bytes[cp->Utf8.length] = '\0';
      break;
    case CONSTANT_MethodHandle:
      cp->MethodHandle.reference_kind = u1Read(fp);
      cp->MethodHandle.reference_index = u2Read(fp);
      break;
    case CONSTANT_MethodType:
      cp->MethodType.descriptor_index = u2Read(fp);
      break;
    case CONSTANT_InvokeDynamic:
      cp->InvokeDynamic.bootstrap_method_attr_index = u2Read(fp);
      cp->InvokeDynamic.name_and_type_index = u2Read(fp);
      break;
    default:
      printf("Constant Pool nao encontrada \n");
      break;
    }
  }

  cf->access_flags = u2Read(fp);
  cf->this_class = u2Read(fp);
  cf->super_class = u2Read(fp);
  cf->interfaces_count = u2Read(fp);
  // interface -> u2
  cf->interfaces = (u2 *)malloc(sizeof(u2) * cf->interfaces_count);
  for (u2 *interface = cf->interfaces; interface < cf->interfaces + cf->interfaces_count; interface++)
  {
    *interface = u2Read(fp);
  }

  cf->fields_count = u2Read(fp);
  // fields -> field_info
  cf->fields = (field_info *)malloc(sizeof(field_info) * cf->fields_count);
  for (field_info *field = cf->fields; field < cf->fields + cf->fields_count; field++)
  {
    field->access_flags = u2Read(fp);
    field->name_index = u2Read(fp);
    field->descriptor_index = u2Read(fp);
    field->attributes_count = u2Read(fp);

    field->attributes = readAttributes(cf->constant_pool, field->attributes_count, fp);
  }

  cf->methods_count = u2Read(fp);
  // methods -> method_info
  cf->methods = (method_info *)malloc(sizeof(method_info) * cf->methods_count);
  for (method_info *method = cf->methods; method < cf->methods + cf->methods_count; method++)
  {
    method->access_flags = u2Read(fp);
    method->name_index = u2Read(fp);
    method->descriptor_index = u2Read(fp);
    method->attributes_count = u2Read(fp);
    method->attributes = readAttributes(cf->constant_pool, method->attributes_count, fp);
    // method->attributes = (attribute_info *)malloc(sizeof(attribute_info) * method->attributes_count);
    // for (attribute_info *attr = method->attributes; attr < method->attributes + method->attributes_count; attr++)
    // {
    //   attr->attribute_name_index = u2Read(fp);
    //   attr->attribute_length = u4Read(fp);
    //   attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
    //   for (
    //       u1 *i = attr->info; i < attr->info + attr->attribute_length; i++)
    //   {
    //     *i = u1Read(fp);
    //   }
    // }
  }

  cf->attributes_count = u2Read(fp);
  // attributes -> attributes_info
  cf->attributes = readAttributes(cf->constant_pool, cf->attributes_count, fp);

  // cf->attributes = (attribute_info *)malloc(sizeof(attribute_info) * cf->attributes_count);
  // for (attribute_info *attr = cf->attributes; attr < cf->attributes + cf->attributes_count; attr++)
  // {
  //   attr->attribute_name_index = u2Read(fp);
  //   attr->attribute_length = u4Read(fp);
  //   attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
  //   for (
  //       u1 *i = attr->info; i < attr->info + attr->attribute_length; i++)
  //   {
  //     *i = u1Read(fp);
  //   }
  // }
}

void recursive_print(cp_info *cp, u2 index, char *str)
{
  switch (cp[index - 1].tag)
  {
  case CONSTANT_Class:
    // printf("Class Name Index: %02d \n", cp->Class.name_index);
    recursive_print(cp, cp[index - 1].Class.name_index, str);
    if (code_sep)
      strcat(str, ".");
    break;
  case CONSTANT_Fieldref:
    // printf("Fieldref Class Index: %02d \n", cp->Fieldref.class_index);
    recursive_print(cp, cp[index - 1].Fieldref.class_index, str);
    // printf("Fieldref Name and Type Index: %02d \n", cp->Fieldref.name_and_type_index, str);
    recursive_print(cp, cp[index - 1].Fieldref.name_and_type_index, str);
    break;
  case CONSTANT_Methodref:
    // printf("Methodref Class Index: %02d \n", cp->Methodref.class_index, str);
    recursive_print(cp, cp[index - 1].Methodref.class_index, str);
    // printf("Methodref Name and Type Index: %02d \n", cp->Methodref.name_and_type_inde, strx);
    recursive_print(cp, cp[index - 1].Methodref.name_and_type_index, str);
    break;
  case CONSTANT_InterfaceMethodref:
    // printf("InterfaceMethodref Class Index: %02d \n", cp->InterfaceMethodref.class_index, str);
    recursive_print(cp, cp[index - 1].InterfaceMethodref.class_index, str);
    // printf("InterfaceMethodref Name and Type Index: %02d \n", cp->InterfaceMethodref.name_and_type_index, str);
    recursive_print(cp, cp[index - 1].InterfaceMethodref.name_and_type_index, str);
    break;
  case CONSTANT_String:
    // printf("String Index: %02d \n", cp->String.string_index, str);
    recursive_print(cp, cp[index - 1].String.string_index, str);
    break;
  case CONSTANT_Integer:
    // printf("Integer Bytes: %02d \n", cp->Integer.bytes, str);
    printConstType(0, cp[index - 1].Integer.bytes, CONSTANT_Integer);
    break;
  case CONSTANT_Float:
    // printf("Float Bytes: %02d \n", cp->Float.bytes, str);
    printConstType(0, cp[index - 1].Float.bytes, CONSTANT_Float);
    break;
  case CONSTANT_Long:
    // printf("Long High Bytes: %02d \n", cp->Long.high_bytes, str);
    // printf("Long Low Bytes: %02d \n", cp->Long.low_bytes, str);
    printConstType(cp[index - 1].Long.high_bytes, cp[index - 1].Long.low_bytes, CONSTANT_Long);
    break;
  case CONSTANT_Double:
    // printf("Double High Bytes: %02d \n", cp->Double.high_bytes, str);
    // printf("Double Low Bytes: %02d \n", cp->Double.low_bytes, str);

    printConstType(cp[index - 1].Double.high_bytes, cp[index - 1].Double.low_bytes, CONSTANT_Double);
    break;
  case CONSTANT_NameAndType:
    // printf("Name and Type - Name Index: %02d \n", cp->NameAndType.name_index, str);
    recursive_print(cp, cp[index - 1].NameAndType.name_index, str);
    // printf("Name and Type - Descriptor Index: %02d \n", cp->NameAndType.descriptor_index, str);
    if (!code_sep)
    {
      strcat(str, " : ");
      recursive_print(cp, cp[index - 1].NameAndType.descriptor_index, str);
    }
    break;
  case CONSTANT_Utf8:
    // printf("UTF8 Length: %02d \n", cp->Utf8.length, str);
    // printf("Bytes: ", str);
    // for (u1 *i = cp->Utf8.bytes; i < cp->Utf8.bytes + cp->Utf8.length; i++)
    // {
    //   printf("%c", *i, str);
    // }
    // printf(" \n", str);
    // printf("%s ", cp[index - 1].Utf8.bytes, str);
    strcat(str, (char *)cp[index - 1].Utf8.bytes);
    break;
  case CONSTANT_MethodHandle:
    // printf("MethodHandle Reference Kind: %02d \n", cp->MethodHandle.reference_kind, str);
    recursive_print(cp, cp[index - 1].MethodHandle.reference_kind, str);
    // printf("MethodHandle Reference Index: %02d \n", cp->MethodHandle.reference_index, str);
    recursive_print(cp, cp[index - 1].MethodHandle.reference_index, str);
    break;
  case CONSTANT_MethodType:
    // printf("MethodType Descriptor Index: %02d \n", cp->MethodType.descriptor_index, str);
    recursive_print(cp, cp[index - 1].MethodType.descriptor_index, str);
    break;
  case CONSTANT_InvokeDynamic:
    // printf("InvokeDynamic - Bootstrap Method Attr Index: %02d \n", cp->InvokeDynamic.bootstrap_method_attr_index, str);
    recursive_print(cp, cp[index - 1].InvokeDynamic.bootstrap_method_attr_index, str);
    // printf("InvokeDynamic - Name and Type Index: %02d \n", cp->InvokeDynamic.name_and_type_index, str);
    recursive_print(cp, cp[index - 1].InvokeDynamic.name_and_type_index, str);
    break;
  default:
    printf("No Ecxiste ese datapoole \n");
    break;
  }
}

char *print_reference(cp_info *cp, u2 index)
{
  // char *str = (char *)malloc(sizeof(char *) * 200);
  strcpy(GLOBAL_ptr, "");
  recursive_print(cp, index, GLOBAL_ptr);
  return GLOBAL_ptr;
}

void print_class_file(ClassFile *cf)
{
  printf("General Info \n");
  printf("< --------------------- > \n");

  printf("Magic: %08X \n", cf->magic);
  printf("Minor Version: %02d \n", cf->minor_version);
  printf("Major Version: %02d [%s] \n", cf->major_version, printVersion(cf->major_version));
  printf("Constant Pool Count: %02d \n", cf->constant_pool_count);
  printf("Access Flags: %#04x %s \n", cf->access_flags, printFlag(cf->access_flags, 1));
  printf("This Class: cp_info #%d <%s> \n", cf->this_class, cf->constant_pool[cf->constant_pool[cf->this_class - 1].Class.name_index - 1].Utf8.bytes);
  printf("Super Class: cp_info #%d <%s> \n", cf->super_class, cf->constant_pool[cf->constant_pool[cf->super_class - 1].Class.name_index - 1].Utf8.bytes);
  printf("Interfaces Count: %02d \n", cf->interfaces_count);
  printf("Fields Count: %02d \n", cf->fields_count);
  printf("Methods Count: %02d \n", cf->methods_count);
  printf("Attributes Count: %02d \n", cf->attributes_count);

  printf("< --------------------- > \n");
  printf("Constant Pool  \n");
  printf("< --------------------- > \n");

  for (cp_info *cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; cp++)
  {
    switch (cp->tag)
    {
    case CONSTANT_Class:
      printf("  [%d] CONSTANT_Class_info \n", (u2)(cp - cf->constant_pool));
      printf("Class Name: cp_info #%d <%s> \n", cp->Class.name_index, print_reference(cf->constant_pool, cp->Class.name_index));
      break;
    case CONSTANT_Fieldref:
      printf("  [%d] CONSTANT_Fieldref_info \n", (u2)(cp - cf->constant_pool));
      printf("Fieldref Class Name: cp_info #%d <%s> \n", cp->Fieldref.class_index, print_reference(cf->constant_pool, cp->Fieldref.class_index));

      printf("Fieldref Name and Type: cp_info #%d <%s> \n", cp->Fieldref.name_and_type_index, print_reference(cf->constant_pool, cp->Fieldref.name_and_type_index));

      break;
    case CONSTANT_Methodref:
      printf("  [%d] CONSTANT_Methodref_info \n", (u2)(cp - cf->constant_pool));
      printf("Methodref Class Name: cp_info #%d <%s> \n", cp->Methodref.class_index, print_reference(cf->constant_pool, cp->Methodref.class_index));

      printf("Methodref Name and Type: cp_info #%d <%s> \n", cp->Methodref.name_and_type_index, print_reference(cf->constant_pool, cp->Methodref.name_and_type_index));

      break;
    case CONSTANT_InterfaceMethodref:
      printf("  [%d] CONSTANT_InterfaceMethodref_info \n", (u2)(cp - cf->constant_pool));
      printf("InterfaceMethodref Class Name: cp_info #%d <%s> \n", cp->InterfaceMethodref.class_index, print_reference(cf->constant_pool, cp->InterfaceMethodref.class_index));

      printf("InterfaceMethodref Name and Type: cp_info #%d <%s> \n", cp->InterfaceMethodref.name_and_type_index, print_reference(cf->constant_pool, cp->InterfaceMethodref.name_and_type_index));

      break;
    case CONSTANT_String:
      printf("  [%d] CONSTANT_String_info \n", (u2)(cp - cf->constant_pool));
      printf("String Name: cp_info #%d <%s> \n", cp->String.string_index, print_reference(cf->constant_pool, cp->String.string_index));
      break;
    case CONSTANT_Integer:
      printf("  [%d] CONSTANT_Integer_info \n", (u2)(cp - cf->constant_pool));
      printf("Integer Bytes: %02d \n", cp->Integer.bytes);
      break;
    case CONSTANT_Float:
      printf("  [%d] CONSTANT_Float_info \n", (u2)(cp - cf->constant_pool));
      printf("Float Bytes: %#x \n", cp->Float.bytes);
      printConstType(0, cp->Float.bytes, CONSTANT_Float);
      printf("Float: %s \n", GLOBAL_ptr);
      break;
    case CONSTANT_Long:
      printf("  [%d] CONSTANT_Long_info \n", (u2)(cp - cf->constant_pool));
      printf("Long High Bytes: %#x \n", cp->Long.high_bytes);
      printf("Long Low Bytes: %#x \n", cp->Long.low_bytes);
      printConstType(cp->Long.high_bytes, cp->Long.low_bytes, CONSTANT_Long);
      printf("Long: %s \n", GLOBAL_ptr);
      break;
    case CONSTANT_Double:
      printf("  [%d] CONSTANT_Double_info \n", (u2)(cp - cf->constant_pool));
      printf("Double High Bytes: %#x \n", cp->Double.high_bytes);
      printf("Double Low Bytes: %#x \n", cp->Double.low_bytes);
      // int64_t num = (int64_t)cp->Long.high_bytes << 32 | (int64_t)cp->Long.low_bytes;
      printConstType(cp->Double.high_bytes, cp->Double.low_bytes, CONSTANT_Double);
      printf("Double: %s \n", GLOBAL_ptr);
      break;
    case CONSTANT_NameAndType:
      printf("  [%d] CONSTANT_NameAndType_info \n", (u2)(cp - cf->constant_pool));
      printf("Name: cp_info #%d <%s> \n", cp->NameAndType.name_index, print_reference(cf->constant_pool, cp->NameAndType.name_index));

      printf("Descriptor: cp_info #%d <%s> \n", cp->NameAndType.descriptor_index, print_reference(cf->constant_pool, cp->NameAndType.descriptor_index));

      break;
    case CONSTANT_Utf8:
      printf("  [%d] CONSTANT_Utf8_info \n", (u2)(cp - cf->constant_pool));
      printf("UTF8 Length: %02d \n", cp->Utf8.length);
      printf("Bytes: %s \n", cp->Utf8.bytes);
      // for (u1 *i = cp->Utf8.bytes; i < cp->Utf8.bytes + cp->Utf8.length; i++)
      // {
      //   printf("%c", *i);
      // }
      // printf(" \n");
      break;
    case CONSTANT_MethodHandle:
      printf("  [%d] CONSTANT_MethodHandle_info \n", (u2)(cp - cf->constant_pool));
      printf("MethodHandle Reference Kind: cp_info #%d <%s> \n", cp->MethodHandle.reference_kind, print_reference(cf->constant_pool, cp->MethodHandle.reference_kind));

      printf("MethodHandle Reference Name: cp_info #%d <%s> \n", cp->MethodHandle.reference_index, print_reference(cf->constant_pool, cp->MethodHandle.reference_index));

      break;
    case CONSTANT_MethodType:
      printf("  [%d] CONSTANT_MethodType_info \n", (u2)(cp - cf->constant_pool));
      printf("MethodType Descriptor: cp_info #%d <%s> \n", cp->MethodType.descriptor_index, print_reference(cf->constant_pool, cp->MethodType.descriptor_index));

      break;
    case CONSTANT_InvokeDynamic:
      printf("  [%d] CONSTANT_InvokeDynamic_info \n", (u2)(cp - cf->constant_pool));
      printf("InvokeDynamic - Bootstrap Method Attr Index: cp_info #%d <%s> \n", cp->InvokeDynamic.bootstrap_method_attr_index, print_reference(cf->constant_pool, cp->InvokeDynamic.bootstrap_method_attr_index));

      printf("InvokeDynamic - Name and Type Index: cp_info #%d <%s> \n", cp->InvokeDynamic.name_and_type_index, print_reference(cf->constant_pool, cp->InvokeDynamic.name_and_type_index));

      break;
    default:
      printf("  [%d] (large numeric continued) \n", (u2)(cp - cf->constant_pool));
      // printf("Ignored \n");
      break;
    }
    printf("< --------------------- > \n");
  }

  // interface -> u2
  printf("Interface  \n");
  printf("< --------------------- > \n");
  for (u2 *interface = cf->interfaces; interface < cf->interfaces + cf->interfaces_count; interface++)
  {
    printf("%d \n", *interface);
  }

  printf("< --------------------- > \n");
  printf("Fields  \n");
  printf("< --------------------- > \n");

  // fields -> field_info

  for (field_info *field = cf->fields; field < cf->fields + cf->fields_count; field++)
  {
    printf("  [%d] %s \n", (u2)(field - cf->fields), print_reference(cf->constant_pool, field->name_index));
    printf("Field Name: cp_info #%d <%s> \n", field->name_index, print_reference(cf->constant_pool, field->name_index));
    printf("Field Descriptor: cp_info #%d <%s> \n", field->descriptor_index, print_reference(cf->constant_pool, field->descriptor_index));
    printf("Access Flags: %#04x %s \n", field->access_flags, printFlag(field->access_flags, 0));
    // printf("%d \n", field->attributes_count);

    printAttributes(field->attributes, cf->constant_pool, field->attributes_count);
  }

  printf("< --------------------- >\n");
  printf("Methods \n");
  printf("< --------------------- >\n");

  for (method_info *mi = cf->methods; mi < cf->methods + cf->methods_count; mi++)
  {
    printf("  [%d] %s \n", (u2)(mi - cf->methods), print_reference(cf->constant_pool, mi->name_index));
    printf("Methods Name: cp_info #%d <%s> \n", mi->name_index, print_reference(cf->constant_pool, mi->name_index));
    printf("Methods Descriptor: cp_info #%d <%s> \n", mi->descriptor_index, print_reference(cf->constant_pool, mi->descriptor_index));
    printf("Methods Access Flags: %#04x %s \n", mi->access_flags, printFlag(mi->access_flags, 0));
    // printf("Methods Attributes Count: %02d\n", mi->attributes_count);

    printAttributes(mi->attributes, cf->constant_pool, mi->attributes_count);

    // for (attribute_info *ai = mi->attributes; ai < mi->attributes + mi->attributes_count; ai++)
    // {
    //   printf("Attribute Name Index: %02d\n", ai->attribute_name_index);
    //   printf("Attribute Name: %s \n", cf->constant_pool[ai->attribute_name_index - 1].Utf8.bytes);
    //   printf("Attribute Length: %02d \n", ai->attribute_length);
    //   if (ai->attribute_length > 0)
    //   {
    //     printf("Attribute Info: ");
    //     for (u1 *i = (ai->info); i < (ai->info) + (ai->attribute_length); i++)
    //     {
    //       printf("%02x ", *i);
    //     }
    //     printf(" \n");
    //   }
    // }
    printf("< --------------------- > \n");
  }

  printf("Attributes  \n");
  printf("< --------------------- > \n");

  printAttributes(cf->attributes, cf->constant_pool, cf->attributes_count);

  // for (attribute_info *ai = cf->attributes; ai < cf->attributes + cf->attributes_count; ai++)
  // {
  //   printf("Attribute Name Index: %02d \n", ai->attribute_name_index);
  //   printf("Attribute Name: %s \n", cf->constant_pool[ai->attribute_name_index - 1].Utf8.bytes);
  //   printf("Attribute Length: %02d \n", ai->attribute_length);
  //   if (ai->attribute_length > 0)
  //   {
  //     printf("Attribute Info: ");
  //     for (u1 *i = (ai->info); i < (ai->info) + (ai->attribute_length); i++)
  //     {
  //       printf("%02x ", *i);
  //     }
  //     printf(" \n");
  //   }
  //   printf("< --------------------- > \n");
  // }
}

void free_class_file(ClassFile *cf)
{
  // interface -> u2
  // cf->interfaces = (u2 *)malloc(sizeof(u2) * cf->interfaces_count);
  free(cf->interfaces);

  for (field_info *field = cf->fields; field < cf->fields + cf->fields_count; field++)
  {

    // for (attribute_info *attr = field->attributes; attr < field->attributes + field->attributes_count; attr++)
    // {

    //   // attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
    //   free(attr->info);
    // }

    // field->attributes = (attribute_info *)malloc(sizeof(attribute_info) * field->attributes_count);
    // free(field->attributes);
    freeAttributes(field->attributes, cf->constant_pool, field->attributes_count);
  }

  // fields -> field_info
  // cf->fields = (field_info *)malloc(sizeof(field_info) * cf->fields_count);
  free(cf->fields);

  // methods -> method_info
  for (method_info *method = cf->methods; method < cf->methods + cf->methods_count; method++)
  {
    // for (attribute_info *attr = method->attributes; attr < method->attributes + method->attributes_count; attr++)
    // {
    //   // attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
    //   free(attr->info);
    // }
    freeAttributes(method->attributes, cf->constant_pool, method->attributes_count);

    // method->attributes = (attribute_info *)malloc(sizeof(attribute_info) * method->attributes_count);
    // free(method->attributes);
  }
  // cf->methods = (method_info *)malloc(sizeof(method_info) * cf->methods_count);
  free(cf->methods);

  // attributes -> attributes_info
  // for (attribute_info *attr = cf->attributes; attr < cf->attributes + cf->attributes_count; attr++)
  // {
  //   // attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
  //   free(attr->info);
  // }
  // cf->attributes = (attribute_info *)malloc(sizeof(attribute_info) * cf->attributes_count);
  // free(cf->attributes);
  freeAttributes(cf->attributes, cf->constant_pool, cf->attributes_count);

  // constant_pool -> cp_info
  for (cp_info *cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; cp++)
  {
    switch (cp->tag)
    {
    case CONSTANT_Class:
      break;
    case CONSTANT_Fieldref:
      break;
    case CONSTANT_Methodref:
      break;
    case CONSTANT_InterfaceMethodref:
      break;
    case CONSTANT_String:
      break;
    case CONSTANT_Integer:
      break;
    case CONSTANT_Float:
      break;
    case CONSTANT_Long:
      break;
    case CONSTANT_Double:
      break;
    case CONSTANT_NameAndType:
      break;
    case CONSTANT_Utf8:
      // cp->Utf8.bytes = (u1 *)malloc(sizeof(u1) * cp->Utf8.length);
      free(cp->Utf8.bytes);
      break;
    case CONSTANT_MethodHandle:
      break;
    case CONSTANT_MethodType:
      break;
    case CONSTANT_InvokeDynamic:
      break;
    default:
      break;
    }
  }

  // cf->constant_pool = (cp_info *)malloc(sizeof(cp_info) * (cf->constant_pool_count - 1));
  free(cf->constant_pool);
}
