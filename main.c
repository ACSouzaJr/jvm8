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
#include "frame.h"
#include "classfile.h"

char *GLOBAL_ptr;
ClassFile *GLOBAL_CLASS;
u1 code_sep = 0;
u1 name_or_type;
char *FILE_NAME;
StackFrame *JvmStack;
Method Mem;
char* PATH;

/**
* @fn      main
* @brief   Função principal do programa
*/
int main(int argc, char *argv[])
{
  // FILE *pFile;
  Mem.num_classes = 0;

  if (argc != 3)
  {
    printf("Warning: Caminho do arquivo nao fornecido.\n");
    printf("Ajuda: ./a.out path/to/file -e (para leitor-exibidor) ou -i (para interpretador)\n");
    // pFile = fopen("Teste/multi.class", "rb");
    return 0;
  }
  // else
  // {
  //   pFile = fopen(argv[1], "rb");
  //   if (!pFile)
  //   {
  //     printf("Error ao abrir arquivo. \n");
  //   }
  // }

  init(); //inicia JvmStack
  GLOBAL_ptr = (char *)malloc(sizeof(char) * 100);
  PATH = (char *)malloc(sizeof(char) * 200);

  // read_class_file(cf, argv[1]);
  ClassFile *cf = Mem.classes_arr[ClassLoader(argv[1])];
  GLOBAL_CLASS = cf;
  // fclose(pFile);

  if (strcmp(removeExtension(print_reference(cf->constant_pool, cf->attributes->info->SourceFile_attribute.sourcefile_index)), findNameFile(removeExtension(argv[1]))) == 0)
  {
    initialize_op_codes();
    char *dash;
    if ((dash = strrchr(argv[1],'\\')) != NULL)
    {
      strncpy(PATH, argv[1], dash-argv[1]);
      #if defined DEBUG
      printf("Path: %s \n", PATH);
      #endif
    }
    else
    {
      strcpy(PATH, ".");
    }
    


    if(strcmp( argv[2], "-e") == 0) { /* para modo leitor-exibidor */
      print_class_file(cf);
    }
    else if(strcmp( argv[2], "-i") == 0) { /* para modo interpretador */
      // Execute Gvm
      method_info *main = find_method(cf, "main","([Ljava/lang/String;)V");
      Frame *frame = cria_frame(cf->constant_pool, main);
      push(frame);
      find_clinit(cf);
      execute_gvm();
    }
    else {
      printf("Modo de execução do programa não fornecido. \n");
    }
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
