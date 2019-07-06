#ifndef STRUCTURES_H
#define STRUCTURES_H

// Structures
#include <stdint.h>
#define u1 uint8_t
#define u2 uint16_t
#define u4 uint32_t
#define DEBUG

extern char *GLOBAL_ptr;
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

typedef struct attribute_info attribute_info;

typedef struct
{
  u2 start_pc;
  u2 line_number;
} line_number_table_type;

typedef struct
{
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  u2 catch_type;
} exception_table_type;

enum
{
  ITEM_Top,
  ITEM_Integer,
  ITEM_Float,
  ITEM_Double,
  ITEM_Long,
  ITEM_Null,
  ITEM_UninitializedThis,
  ITEM_Object,
  ITEM_Uninitialized
};

enum
{
  T_BOOLEAN = 4,
  T_CHAR = 5,
  T_FLOAT = 6,
  T_DOUBLE = 7,
  T_BYTE = 8,
  T_SHORT = 9,
  T_INT = 10,
  T_LONG = 11
};

typedef struct
{
  // u1 tag = ITEM_Top; /* 0 */
} Top_variable_info;

typedef struct
{
  // u1 tag = ITEM_Integer; /* 1 */
} Integer_variable_info;

typedef struct
{
  // u1 tag = ITEM_Float; /* 2 */
} Float_variable_info;

typedef struct
{
  // u1 tag = ITEM_Null; /* 5 */
} Null_variable_info;

typedef struct
{
  // u1 tag = ITEM_UninitializedThis; /* 6 */
} UninitializedThis_variable_info;

typedef struct
{
  // u1 tag = ITEM_Object; /* 7 */
  u2 cpool_index;
} Object_variable_info;

typedef struct
{
  // u1 tag = ITEM_Uninitialized; /* 8 */
  u2 offset;
} Uninitialized_variable_info;

typedef struct
{
  // u1 tag = ITEM_Long; /* 4 */
} Long_variable_info;

typedef struct
{
  // u1 tag = ITEM_Double; /* 3 */
} Double_variable_info;

typedef struct
{
  u1 tag;
  union {
    Top_variable_info top_variable_info;
    Integer_variable_info integer_variable_info;
    Float_variable_info float_variable_info;
    Long_variable_info long_variable_info;
    Double_variable_info double_variable_info;
    Null_variable_info null_variable_info;
    UninitializedThis_variable_info uninitializedThis_variable_info;
    Object_variable_info object_variable_info;
    Uninitialized_variable_info uninitialized_variable_info;
  };
} verification_type_info;

typedef struct
{
  //u1 frame_type; /* = SAME 0-63 */
} Same_frame;

typedef struct
{
  //u1 frame_type;// = SAME_LOCALS_1_STACK_ITEM; /* 64-127 */
  verification_type_info *stack; //[1];
} Same_locals_1_stack_item_frame;

typedef struct
{
  //u1 frame_type;// = SAME_LOCALS_1_STACK_ITEM_EXTENDED; /* 247 */
  u2 offset_delta;
  verification_type_info *stack; //[1];
} Same_locals_1_stack_item_frame_extended;

typedef struct
{
  //u1 frame_type; // = CHOP; /* 248-250 */
  u2 offset_delta;
} Chop_frame;

typedef struct
{
  //u1 frame_type; // = SAME_FRAME_EXTENDED; /* 251 */
  u2 offset_delta;
} Same_frame_extended;

typedef struct
{
  //u1 frame_type; // = APPEND; /* 252-254 */
  u2 offset_delta;
  verification_type_info *locals; //[frame_type - 251];
} Append_frame;

typedef struct
{
  //u1 frame_type; // = FULL_FRAME; /* 255 */
  u2 offset_delta;
  u2 number_of_locals;
  verification_type_info *locals; //[number_of_locals];
  u2 number_of_stack_items;
  verification_type_info *stack; //[number_of_stack_items];
} Full_frame;

typedef struct
{
  u1 frame_type;
  union {
    Same_frame same_frame;
    Same_locals_1_stack_item_frame same_locals_1_stack_item_frame;
    Same_locals_1_stack_item_frame_extended same_locals_1_stack_item_frame_extended;
    Chop_frame chop_frame;
    Same_frame_extended same_frame_extended;
    Append_frame append_frame;
    Full_frame full_frame;
  };
} stack_map_frame;

typedef struct
{
  u2 bootstrap_method_ref;
  u2 num_bootstrap_arguments;
  u2 *bootstrap_arguments; //[num_bootstrap_arguments];
} Bootstrap_method;

typedef struct
{
  u2 inner_class_info_index;
  u2 outer_class_info_index;
  u2 inner_name_index;
  u2 inner_class_access_flags;
} Classes;

typedef struct
{
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 descriptor_index;
  u2 index;
} Local_variable_table;

typedef union {
  struct
  {
    u2 constantvalue_index;
  } ConstantValue_attribute;

  struct
  {
    u2 line_number_table_length;
    line_number_table_type *line_number_table;
    //[line_number_table_length];
  } LineNumberTable_attribute;

  struct
  {
    u2 number_of_entries;
    stack_map_frame *entries; //[number_of_entries];
  } StackMapTable_attribute;

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
    u2 number_of_classes;
    Classes *classes; //[number_of_classes];
  } InnerClasses_attribute;

  struct
  {
  } Synthetic_attribute;

  struct
  {
    u2 local_variable_table_length;
    Local_variable_table *local_variable_table; // [local_variable_table_length];
  } LocalVariableTable_attribute;
  struct
  {
    u2 num_bootstrap_methods;
    Bootstrap_method *bootstrap_methods; //[num_bootstrap_methods];
  } BootstrapMethods_attribute;
  struct
  {
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code; //[code_length];
    u2 exception_table_length;
    //[exception_table_length];
    exception_table_type *exception_table;
    u2 attributes_count;
    attribute_info *attributes; //[attributes_count];
  } Code_attribute;
} attribute_types;

struct attribute_info
{
  u2 attribute_name_index;
  u4 attribute_length;
  attribute_types *info;
  //[attribute_length];
};

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

typedef struct LocalVariable LocalVariable;
typedef struct {
    void *array;
    u2 size;
} TypeArray;

struct LocalVariable
{
  u1 type;
  union 
  {
    u4 value; //cat1
    uint64_t type_long;
    uint64_t type_double;
    TypeArray type_array;
  };
  
};

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

typedef struct
{
  // array de classfiles, contem as classes ja carregadas
  ClassFile *classes_arr[20];
  // numero de classes carregadas em memoria
  u2 num_classes;
} Method;

extern ClassFile *GLOBAL_CLASS;

#endif