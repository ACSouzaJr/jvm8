#include "stack_frame.h"
#include <stdlib.h>
StackFrame *JvmStack;

int pop(){
    int item;
    StackFrame *ptr;
    if (JvmStack == NULL)
    {
        printf("not able to pop the element. No elements in stack\n");
    }
    else
    {
        item = JvmStack->f;
        ptr = JvmStack;
        JvmStack = JvmStack->pointer;
        free(ptr);
        printf("Item popped (cafebabe)\n");
    }
    printf("Item poped with success! Congratulations!\n");
    return item;
}

void push(int * frame)
{
  StackFrame *ptr = (StackFrame *)malloc(sizeof(StackFrame));
  if (ptr == NULL)
  {
    printf("not able to push the element");
  }
  else
  {
   
    if (JvmStack == NULL)
    {
      ptr->f = frame;
      ptr->pointer = NULL;
      JvmStack = ptr;
    }
    else
    {
      ptr->f = frame;
      ptr->pointer = JvmStack;
      JvmStack = ptr;
    }
    printf("Item pushed");
  }
}

int top(){
    return JvmStack->f;
}

void init(){
    printf("Criando pilha...\n");
    JvmStack = NULL;
    printf("Pilha criada.\n");
}

void print_stack(){
    printf("Cafebabe\n");
}