#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include"stack.h"

struct Registrs
{
    int rax = 0;
    int rbx = 0;
    int rcx = 0;
    int rdx = 0;
};

/*struct flags
{
    unsigned int is_func : 4;
    unsigned int is_registr : 4;
};*/

int Processor(my_stack *stk, Registrs *rgs /*flags byte*/);
void Print_content(my_stack *stk, Registrs *rgs);
void Push(my_stack *stk, FILE *fp1);
void Choose_func(my_stack *stk, FILE *fp1, Registrs *rgs /*flags byte*/, int func);
void Pop_r(my_stack *stk, FILE *fp1, Registrs *rgs);
void Push_r(my_stack *stk, FILE *fp1, Registrs *rgs);
void Processor_ctor(my_stack *stk, Registrs *rgs);
void In(my_stack *stk);
void Div(my_stack *stk);
void Sub(my_stack *stk);
void Out(my_stack *stk);
void Add(my_stack *stk);
void Mul(my_stack *stk);


#endif // PROCESSOR_H_INCLUDED
