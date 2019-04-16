#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

uint8_t u1Read(FILE *);
uint16_t u2Read(FILE *);
uint32_t u4Read(FILE *);

void read_class_file(ClassFile *, FILE *);
void print_class_file(ClassFile *arquivoClass);

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

uint8_t u1Read(FILE *file)
{
  // u1 buffer;
  // fread(&buffer, sizeof(u1), 1, file);
  // return buffer;
  u1 toReturn = getc(file);
  return toReturn;
}

uint16_t u2Read(FILE *file)
{
  // u2 buffer;
  // fread(&buffer, sizeof(u2), 1, file);
  // return buffer;
  u2 toReturn = getc(file);
  toReturn = (toReturn << 8) | (getc(file));
  return toReturn;
}

uint32_t u4Read(FILE *file)
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
  // cp_info
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
      cp->NameAndType.descriptor_index = u2Read(fp);
      cp->NameAndType.name_index = u2Read(fp);
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
  // interface
  cf->fields_count = u2Read(fp);
  // fields
  cf->methods_count = u2Read(fp);
  // methods
  cf->attributes_count = u2Read(fp);
  // attributes
}

void print_class_file(ClassFile *arquivoClass)
{

  cp_info *aux;
  method_info *auxMethod;
  attribute_info *auxAttribute;

  printf("Magic: %08x\n", arquivoClass->magic);
  printf("Minor Version: %04x\n", arquivoClass->minor_version);
  printf("Major Version: %04x\n", arquivoClass->major_version);
  printf("Constant Pool Count: %04x\n", arquivoClass->constant_pool_count);

  for (aux = arquivoClass->constant_pool; aux < arquivoClass->constant_pool + arquivoClass->constant_pool_count - 1; aux++)
  {
    printf("TAG: %02x\n", aux->tag);
    switch (aux->tag)
    {
    case CONSTANT_Class:
      printf("Class Name Index: %04x\n", aux->Class.name_index);
      break;
    case CONSTANT_Fieldref:
      printf("Fieldref Class Index: %04x\n", aux->Fieldref.class_index);
      printf("Fieldref Name and Type Index: %04x\n", aux->Fieldref.name_and_type_index);
      break;
    case CONSTANT_Methodref:
      printf("Methodref Class Index: %04x\n", aux->Methodref.class_index);
      printf("Methodref Name and Type Index: %04x\n", aux->Methodref.name_and_type_index);
      break;
    case CONSTANT_InterfaceMethodref:
      printf("InterfaceMethodref Class Index: %04x\n", aux->InterfaceMethodref.class_index);
      printf("InterfaceMethodref Name and Type Index: %04x\n", aux->InterfaceMethodref.name_and_type_index);
      break;
    case CONSTANT_String:
      printf("String Index: %04x\n", aux->String.string_index);
      break;
    case CONSTANT_Integer:
      printf("Integer Bytes: %04x\n", aux->Integer.bytes);
      break;
    case CONSTANT_Float:
      printf("Float Bytes: %04x\n", aux->Float.bytes);
      break;
    case CONSTANT_Long:
      printf("Long High Bytes: %04x\n", aux->Long.high_bytes);
      printf("Long Low Bytes: %04x\n", aux->Long.low_bytes);
      break;
    case CONSTANT_Double:
      printf("Double High Bytes: %04x\n", aux->Double.high_bytes);
      printf("Double Low Bytes: %04x\n", aux->Double.low_bytes);
      break;
    case CONSTANT_NameAndType:
      printf("Name and Type - Name Index: %04x\n", aux->NameAndType.name_index);
      printf("Name and Type - Descriptor Index: %04x\n", aux->NameAndType.descriptor_index);
      break;
    case CONSTANT_Utf8:
      printf("UTF8 Length: %02x\n", aux->Utf8.length);
      printf("Bytes: ");
      for (u1 *i = aux->Utf8.bytes; i < aux->Utf8.bytes + aux->Utf8.length; i++)
      {
        printf("%02x ", *i);
      }
      printf("\n");
      break;
    case CONSTANT_MethodHandle:
      printf("MethodHandle Reference Kind: %02x\n", aux->MethodHandle.reference_kind);
      printf("MethodHandle Reference Index: %04x\n", aux->MethodHandle.reference_index);
      break;
    case CONSTANT_MethodType:
      printf("MethodType Descriptor Index: %04x\n", aux->MethodType.descriptor_index);
      break;
    case CONSTANT_InvokeDynamic:
      printf("InvokeDynamic - Bootstrap Method Attr Index: %04x\n", aux->InvokeDynamic.bootstrap_method_attr_index);
      printf("InvokeDynamic - Name and Type Index: %04x\n", aux->InvokeDynamic.name_and_type_index);
      break;
    default:
      printf("Default\n");
      break;
    }
  }

  printf("Access Flags: %04x\n", arquivoClass->access_flags);
  printf("This Class: %04x\n", arquivoClass->this_class);
  printf("Super Class: %04x\n", arquivoClass->super_class);
  printf("Interfaces Count: %04x\n", arquivoClass->interfaces_count);
  printf("Fields Count: %04x\n", arquivoClass->fields_count);
  printf("Methods Count: %04x\n", arquivoClass->methods_count);

  for (auxMethod = arquivoClass->methods; auxMethod < arquivoClass->methods + arquivoClass->methods_count; auxMethod++)
  {
    printf("Access Flags do método: %04x\n", auxMethod->access_flags);
    printf("Name index do método: %04x\n", auxMethod->name_index);
    printf("Descriptor Index do método: %04x\n", auxMethod->descriptor_index);
    printf("Attributes Count do método: %04x\n", auxMethod->attributes_count);

    for (auxAttribute = auxMethod->attributes; auxAttribute < auxMethod->attributes + auxMethod->attributes_count; auxAttribute++)
    {
      printf("Attribute Name Index: %04x\n", auxAttribute->attribute_name_index);
      printf("Attribute Length: %08x\n", auxAttribute->attribute_length);
      if (auxAttribute->attribute_length > 0)
      {
        printf("Attribute Info: ");
        for (u1 *c = (auxAttribute->info); c < (auxAttribute->info) + (auxAttribute->attribute_length); c++)
        {
          printf("%02x ", *c);
        }
        printf("\n");
      }
    }
  }

  printf("Atributos da Classe: %02x\n", arquivoClass->attributes_count);
  for (auxAttribute = arquivoClass->attributes; auxAttribute < arquivoClass->attributes + arquivoClass->attributes_count; auxAttribute++)
  {
    printf("Attribute Name Index: %04x\n", auxAttribute->attribute_name_index);
    printf("Attribute Length: %08x\n", auxAttribute->attribute_length);
    if (auxAttribute->attribute_length > 0)
    {
      printf("Attribute Info: ");
      for (u1 *c = (auxAttribute->info); c < (auxAttribute->info) + (auxAttribute->attribute_length); c++)
      {
        printf("%02x ", *c);
      }
      printf("\n");
    }
  }
}