#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

u1 u1Read(FILE *);
u2 u2Read(FILE *);
u4 u4Read(FILE *);

void read_class_file(ClassFile *, FILE *);
void print_class_file(ClassFile *cf);

int main(int argc, char const *argv[])
{
  FILE *pFile = fopen("HelloWorld.class", "rb");
  ClassFile *cf = (ClassFile *)malloc(sizeof(ClassFile));
  // printf("%x", u1Read(pFile));
  // printf("%x", readUnsignedShort(pFile));
  // printf("%x", u4Read(pFile));
  read_class_file(cf, pFile);
  print_class_file(cf);
  free(cf);
  fclose(pFile);
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

void read_class_file(ClassFile *cf, FILE *fp)
{
  cf->magic = u4Read(fp);
  cf->minor_version = u2Read(fp);
  cf->major_version = u2Read(fp);
  cf->constant_pool_count = u2Read(fp);
  // constant_pool -> cp_info
  cf->constant_pool = (cp_info *)malloc(sizeof(cp_info) * (cf->constant_pool_count - 1));

  for (cp_info *cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; cp++)
  {
    cp->tag = u1Read(fp);
    // printf("Aqui!:::::%x", cp->tag);
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
      cp->Utf8.bytes = (u1 *)malloc(sizeof(u1) * cp->Utf8.length);
      for (u1 *aux = cp->Utf8.bytes; aux < cp->Utf8.length + cp->Utf8.bytes; aux++)
      {
        // printf("%d", cp);
        *aux = u1Read(fp);
      }
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
    field->attributes = (attribute_info *)malloc(sizeof(attribute_info) * field->attributes_count);
    for (attribute_info *attr = field->attributes; attr < field->attributes + field->attributes_count; attr++)
    {
      attr->attribute_name_index = u2Read(fp);
      attr->attribute_length = u4Read(fp);
      attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
      for (
          u1 *i = attr->info; i < i + attr->attribute_length; i++)
      {
        *i = u1Read(fp);
      }
    }
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
    method->attributes = (attribute_info *)malloc(sizeof(attribute_info) * method->attributes_count);
    for (attribute_info *attr = method->attributes; attr < method->attributes + method->attributes_count; attr++)
    {
      attr->attribute_name_index = u2Read(fp);
      attr->attribute_length = u4Read(fp);
      attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
      for (
          u1 *i = attr->info; i < attr->info + attr->attribute_length; i++)
      {
        *i = u1Read(fp);
      }
    }
  }

  cf->attributes_count = u2Read(fp);
  // attributes -> attributes_info
  cf->attributes = (attribute_info *)malloc(sizeof(attribute_info) * cf->attributes_count);
  for (attribute_info *attr = cf->attributes; attr < cf->attributes + cf->attributes_count; attr++)
  {
    attr->attribute_name_index = u2Read(fp);
    attr->attribute_length = u4Read(fp);
    attr->info = (u1 *)malloc(sizeof(u1 *) * attr->attribute_length);
    for (
        u1 *i = attr->info; i < attr->info + attr->attribute_length; i++)
    {
      *i = u1Read(fp);
    }
  }
}

void print_class_file(ClassFile *cf)
{
  printf("General Info\n");
  printf("< --------------------- >\n");

  printf("Magic: %08X\n", cf->magic);
  printf("Minor Version: %02d\n", cf->minor_version);
  printf("Major Version: %02d\n", cf->major_version);
  printf("Constant Pool Count: %02d\n", cf->constant_pool_count);
  printf("Access Flags: %#04x\n", cf->access_flags);
  printf("This Class: cp_info #%d <%s>\n", cf->this_class, cf->constant_pool[cf->constant_pool[cf->this_class - 1].Class.name_index - 1].Utf8.bytes);
  printf("Super Class: cp_info #%d <%s>\n", cf->super_class, cf->constant_pool[cf->constant_pool[cf->super_class - 1].Class.name_index - 1].Utf8.bytes);
  printf("Interfaces Count: %02d\n", cf->interfaces_count);
  printf("Fields Count: %02d\n", cf->fields_count);
  printf("Methods Count: %02d\n", cf->methods_count);
  printf("Attributes Count: %02d\n", cf->attributes_count);

  printf("< --------------------- >\n");
  printf("Constant Pool \n");
  printf("< --------------------- >\n");

  for (cp_info *cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; cp++)
  {
    printf("TAG: %02d\n", cp->tag);
    switch (cp->tag)
    {
    case CONSTANT_Class:
      printf("Class Name Index: %02d\n", cp->Class.name_index);
      break;
    case CONSTANT_Fieldref:
      printf("Fieldref Class Index: %02d\n", cp->Fieldref.class_index);
      printf("Fieldref Name and Type Index: %02d\n", cp->Fieldref.name_and_type_index);
      break;
    case CONSTANT_Methodref:
      printf("Methodref Class Index: %02d\n", cp->Methodref.class_index);
      printf("Methodref Name and Type Index: %02d\n", cp->Methodref.name_and_type_index);
      break;
    case CONSTANT_InterfaceMethodref:
      printf("InterfaceMethodref Class Index: %02d\n", cp->InterfaceMethodref.class_index);
      printf("InterfaceMethodref Name and Type Index: %02d\n", cp->InterfaceMethodref.name_and_type_index);
      break;
    case CONSTANT_String:
      printf("String Index: %02d\n", cp->String.string_index);
      break;
    case CONSTANT_Integer:
      printf("Integer Bytes: %02d\n", cp->Integer.bytes);
      break;
    case CONSTANT_Float:
      printf("Float Bytes: %02d\n", cp->Float.bytes);
      break;
    case CONSTANT_Long:
      printf("Long High Bytes: %02d\n", cp->Long.high_bytes);
      printf("Long Low Bytes: %02d\n", cp->Long.low_bytes);
      break;
    case CONSTANT_Double:
      printf("Double High Bytes: %02d\n", cp->Double.high_bytes);
      printf("Double Low Bytes: %02d\n", cp->Double.low_bytes);
      break;
    case CONSTANT_NameAndType:
      printf("Name and Type - Name Index: %02d\n", cp->NameAndType.name_index);
      printf("Name and Type - Descriptor Index: %02d\n", cp->NameAndType.descriptor_index);
      break;
    case CONSTANT_Utf8:
      printf("UTF8 Length: %02d\n", cp->Utf8.length);
      printf("Bytes: ");
      for (u1 *i = cp->Utf8.bytes; i < cp->Utf8.bytes + cp->Utf8.length; i++)
      {
        printf("%c", *i);
      }
      printf("\n");
      break;
    case CONSTANT_MethodHandle:
      printf("MethodHandle Reference Kind: %02d\n", cp->MethodHandle.reference_kind);
      printf("MethodHandle Reference Index: %02d\n", cp->MethodHandle.reference_index);
      break;
    case CONSTANT_MethodType:
      printf("MethodType Descriptor Index: %02d\n", cp->MethodType.descriptor_index);
      break;
    case CONSTANT_InvokeDynamic:
      printf("InvokeDynamic - Bootstrap Method Attr Index: %02d\n", cp->InvokeDynamic.bootstrap_method_attr_index);
      printf("InvokeDynamic - Name and Type Index: %02d\n", cp->InvokeDynamic.name_and_type_index);
      break;
    default:
      printf("Ignored\n");
      break;
    }
    printf("< --------------------- >\n");
  }

  printf("Methods \n");
  printf("< --------------------- >\n");

  for (method_info *mi = cf->methods; mi < cf->methods + cf->methods_count; mi++)
  {
    printf("Methods Access Flags: %02d\n", mi->access_flags);
    printf("Methods Name index: %02d\n", mi->name_index);
    printf("Methods Descriptor Index: %02d\n", mi->descriptor_index);
    printf("Methods Attributes Count: %02d\n", mi->attributes_count);

    for (attribute_info *ai = mi->attributes; ai < mi->attributes + mi->attributes_count; ai++)
    {
      printf("Attribute Name Index: %02d\n", ai->attribute_name_index);
      printf("Attribute Length: %02d\n", ai->attribute_length);
      if (ai->attribute_length > 0)
      {
        printf("Attribute Info: ");
        for (u1 *i = (ai->info); i < (ai->info) + (ai->attribute_length); i++)
        {
          printf("%02x ", *i);
        }
        printf("\n");
      }
    }
    printf("< --------------------- >\n");
  }

  printf("Attributes \n");
  printf("< --------------------- >\n");

  for (attribute_info *ai = cf->attributes; ai < cf->attributes + cf->attributes_count; ai++)
  {
    printf("Attribute Name Index: %02d\n", ai->attribute_name_index);
    printf("Attribute Length: %02d\n", ai->attribute_length);
    if (ai->attribute_length > 0)
    {
      printf("Attribute Info: ");
      for (u1 *i = (ai->info); i < (ai->info) + (ai->attribute_length); i++)
      {
        printf("%02x ", *i);
      }
      printf("\n");
    }
    printf("< --------------------- >\n");
  }
}