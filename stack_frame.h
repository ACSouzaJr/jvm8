#ifndef STACK_FRAME_H
#define STACK_FRAME_H

typedef struct{
    int * f;//Frame * f;
    StackFrame * pointer;
} StackFrame;

int pop();
void push(int * frame);
int top();
void init();
void print_stack();

#endif