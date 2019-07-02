#include <stdlib.h>
#include "stack_frame.h"
#include "frame.h"

Frame* pop(){
    Frame* item;
    NodeFrame *ptr;

    // Se o ponteiro for nulo faz nada
    if (JvmStack ==  NULL)
    {
    }
    else
    { 
        // Se a pilha estiver vazia
        if (JvmStack->top == NULL)
        {
          // printf("not able to pop the element. No elements in stack\n");
          // return JvmStack->top;
          return NULL;
        }
        
        ptr = JvmStack->top;
        item = JvmStack->top->f;
        JvmStack->top = ptr->pointer;
        free(ptr);
        // printf("Item popped (cafebabe)\n");
    }
    // printf("Item poped with success! Congratulations!\n");
    return item;
}

void push(Frame *frame)
{
  NodeFrame *ptr = (NodeFrame *)malloc(sizeof(NodeFrame));
  if (ptr == NULL)
  {
    // printf("not able to push the element");
  }
  else
  {
    if (JvmStack == NULL)
    {
    }
    else
    {
      /* Cria um novo elemento e coloca ele no topo da pilha */
      ptr->f = frame;
      ptr->pointer = JvmStack->top;
      JvmStack->top = ptr;
    }
    // printf("Item pushed");
  }
}

Frame* top(){
    return JvmStack->top->f;
}

void init(){
    // printf("Criando pilha...\n");
    JvmStack = (StackFrame *) malloc(sizeof(StackFrame));
    if (!JvmStack)
    {
      JvmStack = NULL;
    }
    
    // printf("Pilha criada.\n");
}

void print_stack(){
    // printf("Cafebabe\n");
}

// int stack_destroy(){
//    NodeFrame * e;
//    if(!JvmStack){
//       printf("Pilha_destroi: parametro nulo");
//       return 0;
//    }
//    e = NULL;
//    while((e=pop(JvmStack))!=NULL){
//       free(e);
//    }
//    free(JvmStack);
//    return 1;
// }

int empty(){
  return JvmStack->top == NULL;
}