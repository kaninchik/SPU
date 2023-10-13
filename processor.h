#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include"stack.h"

const int reg_amount = 5;
const uint8_t imm = 0x10;
const uint8_t reg = 0x20;
const uint8_t cmd = 0xF;

struct Cpu
{
    int regs[reg_amount] = {};
    int *func_arr = NULL;
    const char *regs_name[reg_amount] = {"rax", "rbx", "rcx", "rdx"};
    int elem_count = 0;

};

enum Functions
{
    push = 1,
    in = 2,
    div = 3,
    sub = 4,
    add = 5,
    mul = 6,
    out = 7,
    pop = 11,
    hlt = -1
};

int Processor(My_stack *stk, Cpu *prc);
void Print_content(My_stack *stk, Cpu *prc);
void Push(My_stack *stk, int Elem);
void Pop_r(My_stack *stk, Cpu *prc, int num_reg);
void Push_r(My_stack *stk, Cpu *prc, int num_reg);
void Processor_ctor(My_stack *stk, Cpu *prc);
void Process_file(Cpu *prc);
void In(My_stack *stk);
void Div(My_stack *stk);
void Sub(My_stack *stk);
void Out(My_stack *stk);
void Add(My_stack *stk);
void Mul(My_stack *stk);


#endif // PROCESSOR_H_INCLUDED
