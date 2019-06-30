#ifndef STACK_FRAME_H
#define STACK_FRAME_H
#include "frame.h"

typedef struct StackFrame{
    Frame *f;//Frame * f;
    struct StackFrame * pointer;
} NodeFrame;

typedef struct {
    struct StackFrame *top;
}StackFrame;

// Global
extern StackFrame *JvmStack;

Frame* pop();
void push(Frame *frame);
Frame* top();
void init();
void print_stack();

#endif