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
#include "structures.h"
#include "instructions.h"

u1 u1Read(FILE *);
u2 u2Read(FILE *);
u4 u4Read(FILE *);

void read_class_file(ClassFile *, FILE *);
void print_class_file(ClassFile *cf);
void free_class_file(ClassFile *cf);
char *readUtf8(cp_info *cp, u2 index);
attribute_info *readAttributes(cp_info *cp, u2 attr_count, FILE *fp);
void printAttributes(attribute_info *field, cp_info *cp, u2 attr_count);
void recursive_print(cp_info *cp, u2 index);
verification_type_info *fillVerificationTypeInfo(FILE *fp, u2 verification_type_length);
stack_map_frame *fillStackMapTable(attribute_info *attr, FILE *fp);
void freeAttributes(attribute_info *field, cp_info *cp, u2 attr_count);

int main(int argc, char const *argv[])
{
  FILE *pFile = fopen("Fibonacci.class", "rb");
  ClassFile *cf = (ClassFile *)malloc(sizeof(ClassFile));
  // printf("%x", u1Read(pFile));
  // printf("%x", readUnsignedShort(pFile));
  // printf("%x", u4Read(pFile));
  read_class_file(cf, pFile);
  fclose(pFile);
  initialize_op_codes();
  // printf("%s\n", op_codes_array[0].value);
  // printf("%s\n", op_codes_array[42].value);
  print_class_file(cf);
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

char *readUtf8(cp_info *cp, u2 index)
{
  return (char *)(cp[index - 1]).Utf8.bytes;
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
    else if (strcmp(attribute_name, "StackMapTable") == 0)
    {
      attr->info->StackMapTable_attribute.number_of_entries = u2Read(fp);
      attr->info->StackMapTable_attribute.entries = fillStackMapTable(attr, fp);
      // fseek(fp, attr->attribute_length, SEEK_CUR);
    }
    else if (strcmp(attribute_name, "ConstantValue") == 0)
    {
      attr->info->ConstantValue_attribute.constantvalue_index = u2Read(fp);
    }
    else
    {
      /* code */
    }
  }
  return field;
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
  // attribute_info *field = (attribute_info *)malloc(sizeof(attribute_info) * attr_count);
  for (attribute_info *attr = field; attr < field + attr_count; attr++)
  {
    printf("Attribute Name Index: %02d\n", attr->attribute_name_index);
    printf("Attribute Length: %02d \n", attr->attribute_length);
    // printf("Attribute Name: %s \n", attr->constant_pool[ai->attribute_name_index - 1].Utf8.bytes);

    char *attribute_name = readUtf8(cp, attr->attribute_name_index);

    if (strcmp(attribute_name, "Code") == 0)
    {
      printf("Max stack: %d\n", attr->info->Code_attribute.max_stack);
      printf("Max locals: %d\n", attr->info->Code_attribute.max_locals);
      printf("Code length: %d\n", attr->info->Code_attribute.code_length);

      printf("Code: \n");
      for (u1 *i = attr->info->Code_attribute.code; i < attr->info->Code_attribute.code + attr->info->Code_attribute.code_length; i++)
      {
        // printf("%02x ", *i);
        u1 index;
        printf("%s ", op_codes_array[*i].value);
        if (op_codes_array[*i].arguments)
        {
          // args_num = op_codes_array[*i].arguments;
          index = *i;
          for (size_t j = 0; j < op_codes_array[index].arguments; j++)
          {
            i++;
            printf("%02x ", *i);
            if (*i && op_codes_array[index].references)
            {
              // printf("%s", readUtf8(cp, ));
              // printf("Aqui!! %02x %s", *i, );
              recursive_print(cp, *i);
            }
          }
        }
        printf("\n");
      }

      printf("\n");

      printf("Exception length: %d\n", attr->info->Code_attribute.exception_table_length);
      // attr->info->Code_attribute.exception_table = (exception_table_type *)malloc(sizeof(exception_table_type) * attr->info->Code_attribute.exception_table_length);
      for (exception_table_type *i = attr->info->Code_attribute.exception_table; i < attr->info->Code_attribute.exception_table + attr->info->Code_attribute.exception_table_length; i++)
      {
        printf("Start pc: %d\n", i->start_pc);
        printf("End pc: %d\n", i->end_pc);
        printf("handler Pc: %d\n", i->handler_pc);
        printf("Catch type: %d\n", i->catch_type);
      }
      printf("Attributr Count: %d\n", attr->info->Code_attribute.attributes_count);

      printAttributes(attr->info->Code_attribute.attributes, cp, attr->info->Code_attribute.attributes_count);
    }
    else if (strcmp(attribute_name, "Exceptions") == 0)
    {
      printf("Number of exceptions: %d\n", attr->info->Exceptions_attribute.number_of_exceptions);
      // attr->info->Exceptions_attribute.exception_index_table = (u2 *)malloc(sizeof(u2) * attr->info->Exceptions_attribute.number_of_exceptions);
      for (u2 *i = 0; i < attr->info->Exceptions_attribute.exception_index_table + attr->info->Exceptions_attribute.number_of_exceptions; i++)
      {
        // *i = u2Read(fp);
        printf("Exception index table: %d\n", *i);
      }
    }
    else if (strcmp(attribute_name, "Deprecated") == 0)
    {
      /* code */
    }
    else if (strcmp(attribute_name, "SourceFile") == 0)
    {
      printf("Source file index: %d\n", attr->info->SourceFile_attribute.sourcefile_index);
    }
    else if (strcmp(attribute_name, "LineNumberTable") == 0)
    {
      printf("Line number table name: %d\n", attr->info->LineNumberTable_attribute.line_number_table_length);
      // attr->info->LineNumberTable_attribute.line_number_table = (line_number_table_type *)malloc(sizeof(line_number_table_type) * attr->info->LineNumberTable_attribute.line_number_table_length);
      for (line_number_table_type *i = attr->info->LineNumberTable_attribute.line_number_table; i < attr->info->LineNumberTable_attribute.line_number_table + attr->info->LineNumberTable_attribute.line_number_table_length; i++)
      {
        printf("Start pc: %d\n", i->start_pc);
        printf("Line number: %d\n", i->line_number);
      }
    }
    else if (strcmp(attribute_name, "ConstantValue") == 0)
    {
      printf("Constant value index: %d\n", attr->info->ConstantValue_attribute.constantvalue_index);
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
    printf("Esse arquivo nao é .class");
    return;
  }
  cf->minor_version = u2Read(fp);
  cf->major_version = u2Read(fp);
  cf->constant_pool_count = u2Read(fp);
  // constant_pool -> cp_info
  cf->constant_pool = (cp_info *)malloc(sizeof(cp_info) * (cf->constant_pool_count - 1));

  for (cp_info *cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; cp++)
  {
    cp->tag = u1Read(fp);
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
      break;
    case CONSTANT_Double:
      cp->Double.high_bytes = u4Read(fp);
      cp->Double.low_bytes = u4Read(fp);
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

void recursive_print(cp_info *cp, u2 index)
{
  switch (cp[index - 1].tag)
  {
  case CONSTANT_Class:
    // printf("Class Name Index: %02d \n", cp->Class.name_index);
    recursive_print(cp, cp[index - 1].Class.name_index);
    break;
  case CONSTANT_Fieldref:
    // printf("Fieldref Class Index: %02d \n", cp->Fieldref.class_index);
    recursive_print(cp, cp[index - 1].Fieldref.class_index);
    // printf("Fieldref Name and Type Index: %02d \n", cp->Fieldref.name_and_type_index);
    recursive_print(cp, cp[index - 1].Fieldref.name_and_type_index);
    break;
  case CONSTANT_Methodref:
    // printf("Methodref Class Index: %02d \n", cp->Methodref.class_index);
    recursive_print(cp, cp[index - 1].Methodref.class_index);
    // printf("Methodref Name and Type Index: %02d \n", cp->Methodref.name_and_type_index);
    recursive_print(cp, cp[index - 1].Methodref.name_and_type_index);
    break;
  case CONSTANT_InterfaceMethodref:
    // printf("InterfaceMethodref Class Index: %02d \n", cp->InterfaceMethodref.class_index);
    recursive_print(cp, cp[index - 1].InterfaceMethodref.class_index);
    // printf("InterfaceMethodref Name and Type Index: %02d \n", cp->InterfaceMethodref.name_and_type_index);
    recursive_print(cp, cp[index - 1].InterfaceMethodref.name_and_type_index);
    break;
  case CONSTANT_String:
    // printf("String Index: %02d \n", cp->String.string_index);
    recursive_print(cp, cp[index - 1].String.string_index);
    break;
  case CONSTANT_Integer:
    // printf("Integer Bytes: %02d \n", cp->Integer.bytes);
    break;
  case CONSTANT_Float:
    // printf("Float Bytes: %02d \n", cp->Float.bytes);
    break;
  case CONSTANT_Long:
    // printf("Long High Bytes: %02d \n", cp->Long.high_bytes);
    // printf("Long Low Bytes: %02d \n", cp->Long.low_bytes);
    break;
  case CONSTANT_Double:
    // printf("Double High Bytes: %02d \n", cp->Double.high_bytes);
    // printf("Double Low Bytes: %02d \n", cp->Double.low_bytes);
    break;
  case CONSTANT_NameAndType:
    // printf("Name and Type - Name Index: %02d \n", cp->NameAndType.name_index);
    recursive_print(cp, cp[index - 1].NameAndType.name_index);
    // printf("Name and Type - Descriptor Index: %02d \n", cp->NameAndType.descriptor_index);
    recursive_print(cp, cp[index - 1].NameAndType.descriptor_index);
    break;
  case CONSTANT_Utf8:
    // printf("UTF8 Length: %02d \n", cp->Utf8.length);
    // printf("Bytes: ");
    // for (u1 *i = cp->Utf8.bytes; i < cp->Utf8.bytes + cp->Utf8.length; i++)
    // {
    //   printf("%c", *i);
    // }
    // printf(" \n");
    printf("%s\n", cp[index - 1].Utf8.bytes);
    break;
  case CONSTANT_MethodHandle:
    // printf("MethodHandle Reference Kind: %02d \n", cp->MethodHandle.reference_kind);
    recursive_print(cp, cp[index - 1].MethodHandle.reference_kind);
    // printf("MethodHandle Reference Index: %02d \n", cp->MethodHandle.reference_index);
    recursive_print(cp, cp[index - 1].MethodHandle.reference_index);
    break;
  case CONSTANT_MethodType:
    // printf("MethodType Descriptor Index: %02d \n", cp->MethodType.descriptor_index);
    recursive_print(cp, cp[index - 1].MethodType.descriptor_index);
    break;
  case CONSTANT_InvokeDynamic:
    // printf("InvokeDynamic - Bootstrap Method Attr Index: %02d \n", cp->InvokeDynamic.bootstrap_method_attr_index);
    recursive_print(cp, cp[index - 1].InvokeDynamic.bootstrap_method_attr_index);
    // printf("InvokeDynamic - Name and Type Index: %02d \n", cp->InvokeDynamic.name_and_type_index);
    recursive_print(cp, cp[index - 1].InvokeDynamic.name_and_type_index);
    break;
  default:
    printf("No Ecxiste ese datapoole \n");
    break;
  }
}

void print_class_file(ClassFile *cf)
{
  printf("General Info \n");
  printf("< --------------------- > \n");

  printf("Magic: %08X \n", cf->magic);
  printf("Minor Version: %02d \n", cf->minor_version);
  printf("Major Version: %02d \n", cf->major_version);
  printf("Constant Pool Count: %02d \n", cf->constant_pool_count);
  printf("Access Flags: %#04x \n", cf->access_flags);
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
    printf("TAG: %02d \n", cp->tag);
    switch (cp->tag)
    {
    case CONSTANT_Class:
      recursive_print(cf->constant_pool, cp->Class.name_index);
      break;
    case CONSTANT_Fieldref:
      printf("Fieldref Class Index: %02d \n", cp->Fieldref.class_index);
      recursive_print(cf->constant_pool, cp->Fieldref.class_index);
      printf("Fieldref Name and Type Index: %02d \n", cp->Fieldref.name_and_type_index);
      recursive_print(cf->constant_pool, cp->Fieldref.name_and_type_index);
      break;
    case CONSTANT_Methodref:
      printf("Methodref Class Index: %02d \n", cp->Methodref.class_index);
      recursive_print(cf->constant_pool, cp->Methodref.class_index);
      printf("Methodref Name and Type Index: %02d \n", cp->Methodref.name_and_type_index);
      recursive_print(cf->constant_pool, cp->Methodref.name_and_type_index);
      break;
    case CONSTANT_InterfaceMethodref:
      printf("InterfaceMethodref Class Index: %02d \n", cp->InterfaceMethodref.class_index);
      recursive_print(cf->constant_pool, cp->InterfaceMethodref.class_index);
      printf("InterfaceMethodref Name and Type Index: %02d \n", cp->InterfaceMethodref.name_and_type_index);
      recursive_print(cf->constant_pool, cp->InterfaceMethodref.name_and_type_index);
      break;
    case CONSTANT_String:
      printf("String Index: %02d \n", cp->String.string_index);
      break;
    case CONSTANT_Integer:
      printf("Integer Bytes: %02d \n", cp->Integer.bytes);
      break;
    case CONSTANT_Float:
      printf("Float Bytes: %02d \n", cp->Float.bytes);
      break;
    case CONSTANT_Long:
      printf("Long High Bytes: %02d \n", cp->Long.high_bytes);
      printf("Long Low Bytes: %02d \n", cp->Long.low_bytes);
      break;
    case CONSTANT_Double:
      printf("Double High Bytes: %02d \n", cp->Double.high_bytes);
      printf("Double Low Bytes: %02d \n", cp->Double.low_bytes);
      break;
    case CONSTANT_NameAndType:
      printf("Name and Type - Name Index: %02d \n", cp->NameAndType.name_index);
      recursive_print(cf->constant_pool, cp->NameAndType.name_index);
      printf("Name and Type - Descriptor Index: %02d \n", cp->NameAndType.descriptor_index);
      recursive_print(cf->constant_pool, cp->NameAndType.descriptor_index);
      break;
    case CONSTANT_Utf8:
      printf("UTF8 Length: %02d \n", cp->Utf8.length);
      printf("Bytes: ");
      for (u1 *i = cp->Utf8.bytes; i < cp->Utf8.bytes + cp->Utf8.length; i++)
      {
        printf("%c", *i);
      }
      printf(" \n");
      break;
    case CONSTANT_MethodHandle:
      printf("MethodHandle Reference Kind: %02d \n", cp->MethodHandle.reference_kind);
      recursive_print(cf->constant_pool, cp->MethodHandle.reference_kind);
      printf("MethodHandle Reference Index: %02d \n", cp->MethodHandle.reference_index);
      recursive_print(cf->constant_pool, cp->MethodHandle.reference_index);
      break;
    case CONSTANT_MethodType:
      printf("MethodType Descriptor Index: %02d \n", cp->MethodType.descriptor_index);
      recursive_print(cf->constant_pool, cp->MethodType.descriptor_index);
      break;
    case CONSTANT_InvokeDynamic:
      printf("InvokeDynamic - Bootstrap Method Attr Index: %02d \n", cp->InvokeDynamic.bootstrap_method_attr_index);
      recursive_print(cf->constant_pool, cp->InvokeDynamic.bootstrap_method_attr_index);
      printf("InvokeDynamic - Name and Type Index: %02d \n", cp->InvokeDynamic.name_and_type_index);
      recursive_print(cf->constant_pool, cp->InvokeDynamic.name_and_type_index);
      break;
    default:
      printf("Ignored \n");
      break;
    }
    printf("< --------------------- > \n");
  }

  printf("Methods \n");
  printf("< --------------------- >\n");

  for (method_info *mi = cf->methods; mi < cf->methods + cf->methods_count; mi++)
  {
    printf("Methods Access Flags: %02d\n", mi->access_flags);
    printf("Methods Name index: %02d\n", mi->name_index);
    printf("Methods Descriptor Index: %02d\n", mi->descriptor_index);
    printf("Methods Attributes Count: %02d\n", mi->attributes_count);

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
