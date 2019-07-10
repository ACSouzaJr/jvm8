#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "classfile.h"
#include "instructions.h"
#include "string.h"
#include "interpreter.h"

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
      for (u2 *i = attr->info->Exceptions_attribute.exception_index_table; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
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
#ifdef DEBUG
      printf("Attributo Ignorado! \n");
#endif
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

      for (u2 *i = attr->info->Exceptions_attribute.exception_index_table; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
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
      // printStackMapTable(attr->info->StackMapTable_attribute.entries, cp, attr);
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

void evalAttributes(attribute_info *field, cp_info *cp, u2 attr_count, ClassFile *cf)
{
  Frame *frame = cria_frame(cf->constant_pool, &cf->methods[1]);
  LocalVariable *lv = (LocalVariable *)malloc(sizeof(LocalVariable));
  lv->type = CONSTANT_Integer;
  lv->value = 7;
  frame->pc = 0;
  frame->method = cf->methods; //aki
  frame->cp = cp;
  push_operand(lv, frame->operands);
  lv->value = 9;
  push_operand(lv, frame->operands);
  // frame->local_variables; //[]
  frame->local_variables[0] = *lv;
  lv->value = 1;
  frame->local_variables[1] = *lv;
  lv->value = 2;
  frame->local_variables[2] = *lv;
  lv->value = 3;
  frame->local_variables[3] = *lv;

  // teste frame stack
  push(frame);
  Frame *f2 = pop(frame);
  printf("operand pop frame hahaha: %x \n", top_operand(f2->operands)->value);

  for (attribute_info *attr = field; attr < field + attr_count; attr++)
  {
    char *attribute_name = readUtf8(cp, attr->attribute_name_index);

    // printf("  [%ld] %s \n", attr - field, attribute_name);
    // printf("Attribute Name: cp_info #%d <%s> \n", attr->attribute_name_index, attribute_name);
    // printf("Attribute Length: %d \n", attr->attribute_length);

    if (strcmp(attribute_name, "Code") == 0)
    {
      // printf("Max stack: %d\n", attr->info->Code_attribute.max_stack);
      // printf("Max locals: %d\n", attr->info->Code_attribute.max_locals);
      // printf("Code length: %d\n", attr->info->Code_attribute.code_length);
      code_sep = 1;
      printf("Code: \n");
      for (u1 *i = attr->info->Code_attribute.code; i < attr->info->Code_attribute.code + attr->info->Code_attribute.code_length; i++)
      {
        u2 pc = i - attr->info->Code_attribute.code;
        printf("%02d ", pc);
        // u1 *index;
        printf("Evalutaing %s\n", op_codes_array[*i].value);
        switch (op_codes_array[*i].key)
        {
        case 0: //"nop"
          printf("Evaluating nop...\n");
          break;
        case 1: //"aconst_null"
          printf("Evaluating aconst_null...\n");
          // aconst_null_eval(frame);
          break;
        case 2: //"iconst_m1"
          printf("Evaluating iconst_m1...\n");
          iconst_m1_eval(frame);
          break;
        case 3: //"iconst_0"
          printf("Evaluating iconst_0...\n");
          iconst_0_eval(frame);
          break;
        case 4: //"iconst_1"
          printf("Evaluating iconst_1...\n");
          iconst_1_eval(frame);
          break;
        case 5: //"iconst_2"
          printf("Evaluating iconst_2...\n");
          iconst_2_eval(frame);
          break;
        case 6: //"iconst_3"
          printf("Evaluating iconst_3...\n");
          iconst_3_eval(frame);
          break;
        case 7: //"iconst_4"
          printf("Evaluating iconst_4...\n");
          iconst_4_eval(frame);
          break;
        case 8: //"iconst_5"
          printf("Evaluating iconst_5...\n");
          iconst_5_eval(frame);
          break;
        case 9: //"lconst_0"
          printf("Evaluating lconst_0...\n");
          break;
        case 10: //"lconst_1"
          printf("Evaluating lconst_1...\n");
          break;
        case 11: //"fconst_0"
          printf("Evaluating fconst_0...\n");
          break;
        case 12: //"fconst_1"
          printf("Evaluating fconst_1...\n");
          break;
        case 13: //"fconst_2"
          printf("Evaluating fconst_2...\n");
          break;
        case 14: //"dconst_0"
          printf("Evaluating x.dco\nnst_0.");
          break;
        case 15: //"dconst_1"
          printf("Evaluating x.dco\nnst_1.");
          break;
        case 16: //"bipush"
          printf("Evaluating bipush...\n");
          break;
        case 17: //"sipush"
          printf("Evaluating sipush...\n");
          break;
        case 18: //"ldc"
          printf("Evaluating ldc...\n");
          ldc_eval(frame);
          break;
        case 19: //"ldc_w"
          printf("Evaluating ldc_w...\n");
          break;
        case 20: //"ldc2_w"
          printf("Evaluating ldc2_w...\n");
          break;
        case 21: //"iload"
          printf("Evaluating iload...\n");
          break;
        case 22: //"lload"
          printf("Evaluating lload...\n");
          break;
        case 23: //"fload"
          printf("Evaluating fload...\n");
          break;
        case 24: //"dload"
          printf("Evaluating dload...\n");
          break;
        case 25: //"aload"
          printf("Evaluating aload...\n");
          break;
        case 26: //"iload_0"
          printf("Evaluating iload_0...\n");
          iload_0_eval(frame);
          break;
        case 27: //"iload_1"
          printf("Evaluating iload_1...\n");
          iload_1_eval(frame);
          break;
        case 28: //"iload_2"
          printf("Evaluating iload_2...\n");
          iload_2_eval(frame);
          break;
        case 29: //"iload_3"
          printf("Evaluating iload_3...\n");
          iload_3_eval(frame);
          break;
        case 30: //"lload_0"
          printf("Evaluating lload_0...\n");
          break;
        case 31: //"lload_1"
          printf("Evaluating lload_1...\n");
          break;
        case 32: //"lload_2"
          printf("Evaluating lload_2...\n");
          break;
        case 33: //"lload_3"
          printf("Evaluating lload_3...\n");
          break;
        case 34: //"fload_0"
          printf("Evaluating fload_0...\n");
          break;
        case 35: //"fload_1"
          printf("Evaluating fload_1...\n");
          break;
        case 36: //"fload_2"
          printf("Evaluating fload_2...\n");
          break;
        case 37: //"fload_3"
          printf("Evaluating fload_3...\n");
          break;
        case 38: //"dload_0"
          printf("Evaluating dload_0...\n");
          break;
        case 39: //"dload_1"
          printf("Evaluating dload_1...\n");
          break;
        case 40: //"dload_2"
          printf("Evaluating dload_2...\n");
          break;
        case 41: //"dload_3"
          printf("Evaluating dload_3...\n");
          break;
        case 42: //"aload_0"
          printf("Evaluating aload_0...\n");
          aload_0_eval(frame);
          break;
        case 43: //"aload_1"
          printf("Evaluating aload_1...\n");
          aload_1_eval(frame);
          break;
        case 44: //"aload_2"
          printf("Evaluating aload_2...\n");
          aload_2_eval(frame);
          break;
        case 45: //"aload_3"
          printf("Evaluating aload_3...\n");
          aload_3_eval(frame);
          break;
        case 46: //"iaload"
          printf("Evaluating iaload...\n");
          break;
        case 47: //"laload"
          printf("Evaluating laload...\n");
          break;
        case 48: //"faload"
          printf("Evaluating faload...\n");
          break;
        case 49: //"daload"
          printf("Evaluating daload...\n");
          break;
        case 50: //"aaload"
          printf("Evaluating aaload...\n");
          break;
        case 51: //"baload"
          printf("Evaluating baload...\n");
          break;
        case 52: //"caload"
          printf("Evaluating caload...\n");
          break;
        case 53: //"saload"
          printf("Evaluating saload...\n");
          break;
        case 54: //"istore"
          printf("Evaluating istore...\n");
          break;
        case 55: //"lstore"
          printf("Evaluating lstore...\n");
          break;
        case 56: //"fstore"
          printf("Evaluating fstore...\n");
          break;
        case 57: //"dstore"
          printf("Evaluating dstore...\n");
          break;
        case 58: //"astore"
          printf("Evaluating astore...\n");
          break;
        case 59: //"istore_0"
          printf("Evaluating istore_0...\n");
          istore_0_eval(frame);
          break;
        case 60: //"istore_1"
          printf("Evaluating istore_1...\n");
          istore_1_eval(frame);
          break;
        case 61: //"istore_2"
          printf("Evaluating istore_2...\n");
          istore_2_eval(frame);
          break;
        case 62: //"istore_3"
          printf("Evaluating istore_3...\n");
          istore_3_eval(frame);
          break;
        case 63: //"lstore_0"
          printf("Evaluating lstore_0...\n");
          break;
        case 64: //"lstore_1"
          printf("Evaluating lstore_1...\n");
          break;
        case 65: //"lstore_2"
          printf("Evaluating lstore_2...\n");
          break;
        case 66: //"lstore_3"
          printf("Evaluating lstore_3...\n");
          break;
        case 67: //"fstore_0"
          printf("Evaluating fstore_0...\n");
          break;
        case 68: //"fstore_1"
          printf("Evaluating fstore_1...\n");
          break;
        case 69: //"fstore_2"
          printf("Evaluating fstore_2...\n");
          break;
        case 70: //"fstore_3"
          printf("Evaluating fstore_3...\n");
          break;
        case 71: //"dstore_0"
          printf("Evaluating dstore_0...\n");
          break;
        case 72: //"dstore_1"
          printf("Evaluating dstore_1...\n");
          break;
        case 73: //"dstore_2"
          printf("Evaluating dstore_2...\n");
          break;
        case 74: //"dstore_3"
          printf("Evaluating dstore_3...\n");
          break;
        case 75: //"astore_0"
          printf("Evaluating astore_0...\n");
          break;
        case 76: //"astore_1"
          printf("Evaluating astore_1...\n");
          break;
        case 77: //"astore_2"
          printf("Evaluating astore_2...\n");
          break;
        case 78: //"astore_3"
          printf("Evaluating astore_3...\n");
          break;
        case 79: //"iastore"
          printf("Evaluating iastore...\n");
          break;
        case 80: //"lastore"
          printf("Evaluating lastore...\n");
          break;
        case 81: //"fastore"
          printf("Evaluating fastore...\n");
          break;
        case 82: //"dastore"
          printf("Evaluating dastore...\n");
          break;
        case 83: //"aastore"
          printf("Evaluating aastore...\n");
          break;
        case 84: //"bastore"
          printf("Evaluating bastore...\n");
          break;
        case 85: //"castore"
          printf("Evaluating castore...\n");
          break;
        case 86: //"sastore"
          printf("Evaluating sastore...\n");
          break;
        case 87: //"pop"
          printf("Evaluating pop...\n");
          break;
        case 88: //"pop2"
          printf("Evaluating pop2...\n");
          break;
        case 89: //"dup"
          printf("Evaluating dup...\n");
          break;
        case 90: //"dup_x1"
          printf("Evaluating dup_x1...\n");
          break;
        case 91: //"dup_x2"
          printf("Evaluating dup_x2...\n");
          break;
        case 92: //"dup2"
          printf("Evaluating dup2...\n");
          break;
        case 93: //"dup2_x1"
          printf("Evaluating dup2_x1...\n");
          break;
        case 94: //"dup2_x2"
          printf("Evaluating dup2_x2...\n");
          break;
        case 95: //"swap"
          printf("Evaluating swap...\n");
          break;
        case 96: //"iadd"
          printf("Evaluating iadd...\n");
          iadd_eval(frame);
          break;
        case 97: //"ladd"
          printf("Evaluating ladd...\n");
          break;
        case 98: //"fadd"
          printf("Evaluating fadd...\n");
          break;
        case 99: //"dadd"
          printf("Evaluating dadd...\n");
          break;
        case 100: //"isub"
          printf("Evaluating isub...\n");
          isub_eval(frame);
          break;
        case 101: //"lsub"
          printf("Evaluating lsub...\n");
          break;
        case 102: //"fsub"
          printf("Evaluating fsub...\n");
          break;
        case 103: //"dsub"
          printf("Evaluating dsub...\n");
          break;
        case 104: //"imul"
          printf("Evaluating imul...\n");
          imul_eval(frame);
          break;
        case 105: //"lmul"
          printf("Evaluating lmul...\n");
          break;
        case 106: //"fmul"
          printf("Evaluating fmul...\n");
          break;
        case 107: //"dmul"
          printf("Evaluating dmul...\n");
          break;
        case 108: //"idiv"
          printf("Evaluating idiv...\n");
          idiv_eval(frame);
          break;
        case 109: //"ldiv"
          printf("Evaluating ldiv...\n");
          break;
        case 110: //"fdiv"
          printf("Evaluating fdiv...\n");
          break;
        case 111: //"ddiv"
          printf("Evaluating ddiv...\n");
          break;
        case 112: //"irem"
          printf("Evaluating irem...\n");
          break;
        case 113: //"lrem"
          printf("Evaluating lrem...\n");
          break;
        case 114: //"frem"
          printf("Evaluating frem...\n");
          break;
        case 115: //"drem"
          printf("Evaluating drem...\n");
          break;
        case 116: //"ineg"
          printf("Evaluating ineg...\n");
          break;
        case 117: //"lneg"
          printf("Evaluating lneg...\n");
          break;
        case 118: //"fneg"
          printf("Evaluating fneg...\n");
          break;
        case 119: //"dneg"
          printf("Evaluating dneg...\n");
          break;
        case 120: //"ishl"
          printf("Evaluating ishl...\n");
          break;
        case 121: //"lshl"
          printf("Evaluating lshl...\n");
          break;
        case 122: //"ishr"
          printf("Evaluating ishr...\n");
          break;
        case 123: //"lshr"
          printf("Evaluating lshr...\n");
          break;
        case 124: //"iushr"
          printf("Evaluating iushr...\n");
          break;
        case 125: //"lushr"
          printf("Evaluating lushr...\n");
          break;
        case 126: //"iand"
          printf("Evaluating iand...\n");
          break;
        case 127: //"land"
          printf("Evaluating land...\n");
          break;
        case 128: //"ior"
          printf("Evaluating ior...\n");
          break;
        case 129: //"lor"
          printf("Evaluating lor...\n");
          break;
        case 130: //"ixor"
          printf("Evaluating ixor...\n");
          break;
        case 131: //"lxor"
          printf("Evaluating lxor...\n");
          break;
        case 132: //"iinc"
          printf("Evaluating iinc...\n");
          break;
        case 133: //"i2l"
          printf("Evaluating i2l...\n");
          break;
        case 134: //"i2f"
          printf("Evaluating i2f...\n");
          break;
        case 135: //"i2d"
          printf("Evaluating i2d...\n");
          break;
        case 136: //"l2i"
          printf("Evaluating l2i...\n");
          break;
        case 137: //"l2f"
          printf("Evaluating l2f...\n");
          break;
        case 138: //"l2d"
          printf("Evaluating l2d...\n");
          break;
        case 139: //"f2i"
          printf("Evaluating f2i...\n");
          break;
        case 140: //"f2l"
          printf("Evaluating f2l...\n");
          break;
        case 141: //"f2d"
          printf("Evaluating f2d...\n");
          break;
        case 142: //"d2i"
          printf("Evaluating d2i...\n");
          break;
        case 143: //"d2l"
          printf("Evaluating d2l...\n");
          break;
        case 144: //"d2f"
          printf("Evaluating d2f...\n");
          break;
        case 145: //"i2b"
          printf("Evaluating i2b...\n");
          break;
        case 146: //"i2c"
          printf("Evaluating i2c...\n");
          break;
        case 147: //"i2s"
          printf("Evaluating i2s...\n");
          break;
        case 148: //"lcmp"
          printf("Evaluating lcmp...\n");
          break;
        case 149: //"fcmpl"
          printf("Evaluating fcmpl...\n");
          break;
        case 150: //"fcmpg"
          printf("Evaluating fcmpg...\n");
          break;
        case 151: //"dcmpl"
          printf("Evaluating dcmpl...\n");
          break;
        case 152: //"dcmpg"
          printf("Evaluating dcmpg...\n");
          break;
        case 153: //"ifeq"
          printf("Evaluating ifeq...\n");
          break;
        case 154: //"ifne"
          printf("Evaluating ifne...\n");
          break;
        case 155: //"iflt"
          printf("Evaluating iflt...\n");
          break;
        case 156: //"ifge"
          printf("Evaluating ifge...\n");
          break;
        case 157: //"ifgt"
          printf("Evaluating x...\n");
          break;
        case 158: //"ifle"
          printf("Evaluating x...\n");
          break;
        case 159: //"if_icmpeq"
          printf("Evaluating x...\n");
          break;
        case 160: //"if_icmpne"
          printf("Evaluating x...\n");
          break;
        case 161: //"if_icmplt"
          printf("Evaluating x...\n");
          break;
        case 162: //"if_icmpg2"
          printf("Evaluating x...\n");
          break;
        case 163: //"if_icmpgt"
          printf("Evaluating x...\n");
          break;
        case 164: //"if_icmple"
          printf("Evaluating x...\n");
          break;
        case 165: //"if_acmpeq"
          printf("Evaluating x...\n");
          break;
        case 166: //"if_acmpne"
          printf("Evaluating x...\n");
          break;
        case 167: //"goto"
          printf("Evaluating x...\n");
          break;
        case 168: //"jsr"
          printf("Evaluating x...\n");
          break;
        case 169: //"ret"
          printf("Evaluating x...\n");
          break;
        case 170: //"tableswitch"
          printf("Evaluating x...\n");
          break;
        case 171: //"lookupswitch"
          printf("Evaluating x...\n");
          break;
        case 172: //"ireturn"
          printf("Evaluating x...\n");
          break;
        case 173: //"lreturn"
          printf("Evaluating x...\n");
          break;
        case 174: //"freturn"
          printf("Evaluating x...\n");
          break;
        case 175: //"dreturn"
          printf("Evaluating x...\n");
          break;
        case 176: //"areturn"
          printf("Evaluating x...\n");
          break;
        case 177: //"return"
          printf("Evaluating return...\n");
          return_eval(frame);
          break;
        case 178: //"getstatic"
          printf("Evaluating getstatic...\n");
          getstatic_eval(frame);
          break;
        case 179: //"putstatic"
          printf("Evaluating x...\n");
          break;
        case 180: //"getfield"
          printf("Evaluating x...\n");
          break;
        case 181: //"putfield"
          printf("Evaluating x...\n");
          break;
        case 182: //"invokevirtual"
          printf("Evaluating x...\n");
          break;
        case 183: //"invokespecial"
          printf("Evaluating invokespecial...\n");
          invokespecial_eval(frame);
          break;
        case 184: //"invokestatic"
          printf("Evaluating x...\n");
          break;
        case 185: //"invokeinterface"
          printf("Evaluating x...\n");
          break;
        case 186: //"invokedynamic"
          printf("Evaluating x...\n");
          break;
        case 187: //"new"
          printf("Evaluating x...\n");
          break;
        case 188: //"newarray"
          printf("Evaluating x...\n");
          break;
        case 189: //"anewarray"
          printf("Evaluating x...\n");
          break;
        case 190: //"arraylength"
          printf("Evaluating x...\n");
          break;
        case 191: //"athrow"
          printf("Evaluating x...\n");
          break;
        case 192: //"checkcast"
          printf("Evaluating x...\n");
          break;
        case 193: //"instanceof"
          printf("Evaluating x...\n");
          break;
        case 194: //"monitorenter"
          printf("Evaluating x...\n");
          break;
        case 195: //"monitorexit"
          printf("Evaluating x...\n");
          break;
        case 196: //"wide"
          printf("Evaluating x...\n");
          break;
        case 197: //"multianewarray"
          printf("Evaluating x...\n");
          break;
        case 198: //"ifnull"
          printf("Evaluating x...\n");
          break;
        case 199: //"ifnonnull"
          printf("Evaluating x...\n");
          break;
        case 200: //"goto_w"
          printf("Evaluating x...\n");
          break;
        case 201: //"jsr_w"
          printf("Evaluating x...\n");
          break;
        case 202: //"breakpoint"
          printf("Evaluating x...\n");
          break;
        case 254: //"impdep1"
          printf("Evaluating x...\n");
          break;
        case 255: //"impdep2"
          printf("Evaluating x...\n");
          break;
        default:
          printf("Instrução inválida...");
          break;
        }
        printf("\n");
      }
      code_sep = 0;

      printf("\n");

      // printf("Exception length: %d\n", attr->info->Code_attribute.exception_table_length);
      // printf("Attribute Count: %d\n", attr->info->Code_attribute.attributes_count);

      // printAttributes(attr->info->Code_attribute.attributes, cp, attr->info->Code_attribute.attributes_count);
    }
    // else if (strcmp(attribute_name, "Exceptions") == 0)
    // {
    //   printf("Number of exceptions: %d\n", attr->info->Exceptions_attribute.number_of_exceptions);

    //   for (u2 *i = 0; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
    //   {
    //     printf("Exception index table: %d\n", *i);
    //   }
    // }
    // else if (strcmp(attribute_name, "Deprecated") == 0)
    // {
    //   /* code */
    // }
    // else if (strcmp(attribute_name, "SourceFile") == 0)
    // {
    //   printf("Source file: cp_info #%d <%s> \n", attr->info->SourceFile_attribute.sourcefile_index, print_reference(cp, attr->info->SourceFile_attribute.sourcefile_index));
    // }
    // else if (strcmp(attribute_name, "LineNumberTable") == 0)
    // {
    //   // printf("Line number table length: %d\n", attr->info->LineNumberTable_attribute.line_number_table_length);

    //   for (line_number_table_type *i = attr->info->LineNumberTable_attribute.line_number_table; i < attr->info->LineNumberTable_attribute.line_number_table + attr->info->LineNumberTable_attribute.line_number_table_length; i++)
    //   {
    //     printf("Nr. %d\t", (u2)(i - attr->info->LineNumberTable_attribute.line_number_table));
    //     printf("Start pc: %d\t", i->start_pc);
    //     printf("Line number: %d\t\n", i->line_number);
    //   }
    // }
    // else if (strcmp(attribute_name, "StackMapTable") == 0)
    // {
    //   printStackMapTable(attr->info->StackMapTable_attribute.entries, cp, attr);
    // }
    // else if (strcmp(attribute_name, "ConstantValue") == 0)
    // {
    //   printf("Constant value: cp_info #%d <%s> \n", attr->info->ConstantValue_attribute.constantvalue_index, print_reference(cp, attr->info->ConstantValue_attribute.constantvalue_index));
    // }
    else
    {
      /* code */
      printf("caiu no else\n");
    }
  }
}

void read_class_file(ClassFile *cf, char *file_name)
{
  FILE *fp = fopen(file_name, "rb");
  if (!fp)
  {
    printf("Error ao abrir arquivo. \n");
  }
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
  fclose(fp);
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

void rec_method_name(cp_info *cp, u2 index, char *str)
{
  switch (cp[index - 1].tag)
  {
  case CONSTANT_Class:
    // printf("Class Name Index: %02d \n", cp->Class.name_index);
    rec_method_name(cp, cp[index - 1].Class.name_index, str);
    break;
  case CONSTANT_Fieldref:
    // printf("Fieldref Class Index: %02d \n", cp->Fieldref.class_index);
    rec_method_name(cp, cp[index - 1].Fieldref.class_index, str);
    // printf("Fieldref Name and Type Index: %02d \n", cp->Fieldref.name_and_type_index, str);
    // rec_method_name(cp, cp[index - 1].Fieldref.name_and_type_index, str);
    break;
  case CONSTANT_Methodref:
    // printf("Methodref Class Index: %02d \n", cp->Methodref.class_index, str);
    rec_method_name(cp, cp[index - 1].Methodref.class_index, str);
    // printf("Methodref Name and Type Index: %02d \n", cp->Methodref.name_and_type_inde, strx);
    break;
  case CONSTANT_InterfaceMethodref:
    // printf("InterfaceMethodref Class Index: %02d \n", cp->InterfaceMethodref.class_index, str);
    rec_method_name(cp, cp[index - 1].InterfaceMethodref.class_index, str);
    // printf("InterfaceMethodref Name and Type Index: %02d \n", cp->InterfaceMethodref.name_and_type_index, str);
    // rec_method_name(cp, cp[index - 1].InterfaceMethodref.name_and_type_index, str);
    break;
  case CONSTANT_String:
    // printf("String Index: %02d \n", cp->String.string_index, str);
    rec_method_name(cp, cp[index - 1].String.string_index, str);
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
    rec_method_name(cp, cp[index - 1].NameAndType.name_index, str);
    // printf("Name and Type - Descriptor Index: %02d \n", cp->NameAndType.descriptor_index, str);
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
    rec_method_name(cp, cp[index - 1].MethodHandle.reference_kind, str);
    // printf("MethodHandle Reference Index: %02d \n", cp->MethodHandle.reference_index, str);
    rec_method_name(cp, cp[index - 1].MethodHandle.reference_index, str);
    break;
  case CONSTANT_MethodType:
    // printf("MethodType Descriptor Index: %02d \n", cp->MethodType.descriptor_index, str);
    rec_method_name(cp, cp[index - 1].MethodType.descriptor_index, str);
    break;
  case CONSTANT_InvokeDynamic:
    // printf("InvokeDynamic - Bootstrap Method Attr Index: %02d \n", cp->InvokeDynamic.bootstrap_method_attr_index, str);
    rec_method_name(cp, cp[index - 1].InvokeDynamic.bootstrap_method_attr_index, str);
    // printf("InvokeDynamic - Name and Type Index: %02d \n", cp->InvokeDynamic.name_and_type_index, str);
    rec_method_name(cp, cp[index - 1].InvokeDynamic.name_and_type_index, str);
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

char *ret_method_name(cp_info *cp, u2 index)
{
  // char *str = (char *)malloc(sizeof(char *) * 200);
  strcpy(GLOBAL_ptr, "");
  rec_method_name(cp, index, GLOBAL_ptr);
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
    // printf("CAFEBABEEEEE!\n");
    // evalAttributes(mi->attributes, cf->constant_pool, mi->attributes_count, cf);

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

char *removeExtension(char *string)
{
  int i;
  for (i = 0; i < strlen(string); i++)
  {
    if (string[i] == '.')
    {
      string[i] = '\0';
      return string;
    }
  }
  return string;
}

char *findNameFile(char *string)
{
  int i, j, k = 0, count = 0;
  char aux_string[100];
  for (i = 0; i < strlen(string); i++)
  {
    if (string[i] == '/')
    {
      count++;
    }
  }

  for (i = 0; i < strlen(string); i++)
  {
    if (string[i] == '/')
    {
      count--;
      if (count == 0)
      {
        k = 0;
        for (j = i + 1; j < strlen(string); j++)
        {
          aux_string[k] = string[j];
          k++;
        }
      }
    }
  }
  if (k)
  {

    aux_string[k] = '\0';
    strcpy(GLOBAL_ptr, aux_string);
    return GLOBAL_ptr;
  }
  else
  {
    return string;
  }
}

void execute_gvm()
{
  do
  {
    Frame *current_frame = JvmStack->top->f;
    u1 *bytecode = current_frame->method->attributes->info->Code_attribute.code;
    u2 opcode = bytecode[current_frame->pc++];

    #ifdef DEBUG
        printf("\n----  Evaluando ----\n %d) %s\n-----------------------\n\n", ((current_frame->pc) - 1), op_codes_array[opcode].value);
    #endif
    op_codes_array[opcode].eval(current_frame);
  } while (!empty(JvmStack));
}

method_info *find_method(ClassFile *cf, char *method, char *method_description)
{
  // corrigir depois
  for (method_info *i = cf->methods; i < cf->methods + cf->methods_count; i++)
  {
    char *method_name = readUtf8(cf->constant_pool, i->name_index);
    char *method_desc = readUtf8(cf->constant_pool, i->descriptor_index);
    // printf("Metodo encontrado fora: %s\n",method_name);
    // printf("Metodo desc fora: %s\n",method_desc);
    // char *method_desc = readUtf8(cf->constant_pool, i->descriptor_index);
    if (strcmp(method_name, method) == 0)
    {
      if (strcmp(method_desc, method_description) == 0){
        #ifdef DEBUG
          printf("Metodo encontrado: %s\n",method_name);
          printf("Metodo desc: %s\n",method_desc);
        #endif
        return i;
      }
    }
  }
  #ifdef DEBUG
    printf("Nao achou o methodo! \n");
  #endif
  return NULL;
}

u4 ClassLoader(char *class_name)
{
  ClassFile *cf = (ClassFile *)malloc(sizeof(ClassFile));
  if (strstr(class_name, ".class") != NULL)
  {
    sprintf(GLOBAL_ptr, "%s", class_name);
  }
  else
  {
    sprintf(GLOBAL_ptr, "./%s.class", class_name);
  }
#ifdef DEBUG
  printf("Loaded class: %s \n", GLOBAL_ptr);
#endif
  read_class_file(cf, GLOBAL_ptr);
  Mem.classes_arr[Mem.num_classes++] = cf;
  if (Mem.classes_arr[Mem.num_classes - 1] == NULL)
  {
    printf("Erro ao carregar classe!\n");
    exit(0);
  }
  return Mem.num_classes - 1;
}

u2 find_class(char *class_name)
{
  char *this_class;
  for (size_t i = 0; i < Mem.num_classes; i++)
  {
    this_class = ret_method_name(Mem.classes_arr[i]->constant_pool, Mem.classes_arr[i]->this_class);
#ifdef DEBUG
    printf("This class: %s \n", this_class);
#endif
    if (strcmp(this_class, class_name) == 0)
    {
      return i;
    }
  }
  // Se nao encontrar a classe carrega na memoria;
  return ClassLoader(class_name);
}

field_info *find_field(ClassFile *cf, char *field_name, char *field_desc)
{
  for (field_info *i = cf->fields; i < cf->fields + cf->fields_count; i++)
  {
    char *name = readUtf8(cf->constant_pool, i->name_index);
    char *desc = readUtf8(cf->constant_pool, i->descriptor_index);
    if (strcmp(name, field_name) == 0 && strcmp(desc, field_desc) == 0)
    {
      return i;
    }
  }
  printf("Nao achou o field! \n");
  return NULL;
}

void find_clinit(ClassFile *cf)
{
  method_info *method;
  if ((method = find_method(cf, "<clinit>","()V")) != NULL)
  {
    Frame *frame = cria_frame(cf->constant_pool, method);
    push(frame);
  }
#ifdef DEBUG
    printf("Nao possui init !\n");
#endif
}