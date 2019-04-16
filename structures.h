// Structures
#include <stdint.h>
#define u1 uint8_t
#define u2 uint16_t
#define u4 uint32_t

// Para o constante_pool temos que:
// 	-	Informacoes simbolicas sao salvas no constant_pool.
// 	-	Cada entrada em pool tem a forma cp

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
  u1 *info; //[attribute_length];
} attribute_info;

typedef struct
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  attribute_info *attributes; //[attributes_count];
} method_info;

typedef struct
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  attribute_info *attributes; //[attributes_count];
} field_info;

// class type
typedef struct
{
  uint32_t magic;         // 0xCAFEBABE
  uint16_t minor_version; // M.m
  uint16_t major_version;
  uint16_t constant_pool_count;
  cp_info *constant_pool; //[constant_pool - 1];
  uint16_t access_flags;
  uint16_t this_class;
  uint16_t super_class;
  uint16_t interfaces_count;
  uint16_t *interfaces; //[interfaces_count];
  uint16_t fields_count;
  field_info *fields; //[fields_count];
  uint16_t methods_count;
  method_info *methods; //[methods_count];
  uint16_t attributes_count;
  attribute_info *attributes; //[attributes_count];
} ClassFile;