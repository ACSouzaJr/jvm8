#include <stdio.h>
#include "structures.h"
#include "stack_frame.h"

extern char *GLOBAL_ptr;
extern u1 code_sep;
extern u1 name_or_type;
extern char *FILE_NAME;
extern StackFrame *JvmStack;
extern Method Mem;

u1 u1Read(FILE *);
u2 u2Read(FILE *);
u4 u4Read(FILE *);

void read_class_file(ClassFile *cf, char *file_name);
void print_class_file(ClassFile *cf);
void free_class_file(ClassFile *cf);
char *readUtf8(cp_info *cp, u2 index);
attribute_info *readAttributes(cp_info *cp, u2 attr_count, FILE *fp);
void printAttributes(attribute_info *field, cp_info *cp, u2 attr_count);
void evalAttributes(attribute_info *field, cp_info *cp, u2 attr_count, ClassFile * cf);
void recursive_print(cp_info *cp, u2 index, char *str);
void rec_method_name(cp_info *cp, u2 index, char *str);
char *ret_method_name(cp_info *cp, u2 index);
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
char *removeExtension(char *string);
char *findNameFile(char *string);
u4 ClassLoader(char *class_name);
void execute_gvm();
method_info* find_method(ClassFile *cf, char* method);
u2 find_class(char* class_name);
field_info* find_field(ClassFile *cf, char *field_name, char* field_desc);