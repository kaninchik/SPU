#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include<cassert>
#include"stack.h"

const uint8_t imm = 1 << 5;
const uint8_t reg = imm << 1;
const uint8_t osu = reg << 1;
const uint8_t cmd = 0b11111;

static_assert((imm & reg) == 0, "imm and reg overlop");
static_assert((imm & cmd) == 0, "imm and cmd overlop");
static_assert((reg & cmd) == 0, "reg and cmd overlop");

static const char *regs_name[] = {"rax",
                                  "rbx",
                                  "rcx",
                                  "rdx",
                                  "rsi",
                                  "rdi",
                                  "rbp",
                                  "rsp",
                                  "r08",
                                  "r09",
                                  "r10",
                                  "r11",
                                  "r12",
                                  "r13",
                                  "r14",
                                  "r15"};

const int reg_amount = sizeof(regs_name)/sizeof(regs_name[0]);

struct Spu
{
    My_stack stk = {};
    My_stack adress = {};
    int regs[reg_amount] = {};
    int *instr = NULL;
    int *osu = NULL;
    int instr_number = 0;
};

#define DEF_CMD(name, num, args, code)  \
    name = num,                         \


#define DEF_JMP(name, num, sign)  \
    name = num,                   \

enum Opcode
{
    #include"commands.h"
    #include"undef.h"
};

int Processor(Spu *prc);
void Print_content(Spu *prc);
void Processor_ctor(Spu *prc);



#endif // PROCESSOR_H_INCLUDED
