#ifndef STACK_FRAME_H
#define STACK_FRAME_H
#include "frame.h"

typedef struct StackFrame{
    Frame *f; // Frame dentro da pilha de Frames 
    struct StackFrame * pointer; // Aponta para o próximo frame
} NodeFrame;

typedef struct {
    struct StackFrame *top; // Topo do StackFrame
}StackFrame;

// Global
extern StackFrame *JvmStack;

Frame* pop();
void push(Frame *frame);
Frame* top();
void init();
void print_stack();
int empty();

#endif