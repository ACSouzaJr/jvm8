// Structures
#include <stdint.h>
#define u1 uint8_t
#define u2 uint16_t
#define u4 uint32_t

/**
  Tabela contendo o valor para cada tag da constante (tipo id)
*/
enum
{
  CONSTANT_Class = 7,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_String = 8,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_NameAndType = 12,
  CONSTANT_Utf8 = 1,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_InvokeDynamic = 18
} pool_tags;

/**
  Struct para indicar o tipo de constant_pool
  (tendo uma tag associada e mais atributos em função da constante)
  
  Union: serve para alocar espaço pra maior struct
*/
typedef struct
{
  u1 tag;
  union {
    struct
    {
      u2 name_index;
    } Class;

    struct
    {
      u2 class_index;
      u2 name_and_type_index;
    } Fieldref;

    struct
    {
      u2 class_index;
      u2 name_and_type_index;
    } Methodref;

    struct
    {
      u2 class_index;
      u2 name_and_type_index;
    } InterfaceMethodref;

    struct
    {
      u2 string_index;
    } String;

    struct
    {
      u4 bytes;
    } Integer;

    struct
    {
      u4 bytes;
    } Float;

    struct
    {
      u4 high_bytes;
      u4 low_bytes;
    } Long;

    struct
    {
      u4 high_bytes;
      u4 low_bytes;
    } Double;

    struct
    {
      u2 name_index;
      u2 descriptor_index;
    } NameAndType;

    struct
    {
      u2 length;
      u1 *bytes; //[length];
    } Utf8;

    struct
    {
      u1 reference_kind;
      u2 reference_index;
    } MethodHandle;

    struct
    {
      u2 descriptor_index;
    } MethodType;

    struct
    {
      u2 bootstrap_method_attr_index;
      u2 name_and_type_index;
    } InvokeDynamic;
  };

} cp_info;

typedef struct
{
  u2 attribute_name_index;
  u4 attribute_length;
  union {
    struct
    {
      u2 constantvalue_index;
    } ConstantValue_attribute;

    struct
    {
      u2 line_number_table_length;
      struct
      {
        u2 start_pc;
        u2 line_number;
      } * line_number_table; //[line_number_table_length];
    } LineNumberTable_attribute;

    struct
    {
      u2 sourcefile_index;
    } SourceFile_attribute;

    struct
    {
    } Deprecated_attribute;

    struct
    {
      u2 number_of_exceptions;
      u2 *exception_index_table; //[number_of_exceptions];
    } Exceptions_attribute;
    struct
    {
      u2 max_stack;
      u2 max_locals;
      u4 code_length;
      u1 *code; //[code_length];
      u2 exception_table_length;
      struct
      {
        u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
      } * exception_table; //[exception_table_length];
      u2 attributes_count;
      attribute_info *attributes; //[attributes_count];
    } Code_attribute;
  } * info; //[attribute_length];
} attribute_info;

typedef struct
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  attribute_info *attributes; //[attributes_count];
  // code, exception, etc
} method_info;

typedef struct
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  // Cosntant value
  attribute_info *attributes; //[attributes_count];
} field_info;

// class type
typedef struct
{
  u4 magic;         // 0xCAFEBABE
  u2 minor_version; // M.m
  u2 major_version;
  u2 constant_pool_count;
  cp_info *constant_pool; //[constant_pool - 1];
  u2 access_flags;
  u2 this_class;
  u2 super_class;
  u2 interfaces_count;
  u2 *interfaces; //[interfaces_count];
  u2 fields_count;
  field_info *fields; //[fields_count];
  u2 methods_count;
  method_info *methods; //[methods_count];
  u2 attributes_count;
  attribute_info *attributes; //[attributes_count];
} ClassFile;