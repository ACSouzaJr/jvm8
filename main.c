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
#include "stack_frame.h"
#include "interpreter.h"
#include "classfile.h"

char *GLOBAL_ptr;
u1 code_sep = 0;
u1 name_or_type;
char *FILE_NAME;
StackFrame *JvmStack;
Method Mem;

int main(int argc, char *argv[])
{
  FILE *pFile;
  Mem.num_classes = 0;

  if (argc != 2)
  {
    printf("Warning: Caminho do arquivo nao fornecido.");
    // pFile = fopen("Teste/multi.class", "rb");
    return 0;
  }
  else
  {
    pFile = fopen(argv[1], "rb");
    if (!pFile)
    {
      printf("Error ao abrir arquivo. \n");
    }
  }

  ClassFile *cf;
  GLOBAL_ptr = (char *)malloc(sizeof(char) * 100);

  read_class_file(cf, pFile);
  fclose(pFile);

  if (strcmp(removeExtension(print_reference(cf->constant_pool, cf->attributes->info->SourceFile_attribute.sourcefile_index)), findNameFile(removeExtension(argv[1]))) == 0)
  {
    initialize_op_codes();
    print_class_file(cf);
  }
  else
  {
    printf("Nome do arquivo e do SourceFile diferentes. \n");
  }
  
  free(GLOBAL_ptr);
  free_class_file(cf);
  free(cf);
  return 0;
}