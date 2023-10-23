#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include<cassert>
#include"stack.h"

const uint8_t imm = 1 << 5;
const uint8_t reg = imm << 1;
const uint8_t osu = reg << 1;
const uint8_t cmd = 0b11111;

static_assert((imm & reg) == 0, "imm and reg overlap");
static_assert((imm & cmd) == 0, "imm and cmd overlap");
static_assert((imm & osu) == 0, "imm and osu overlap");
static_assert((reg & osu) == 0, "reg and osu overlap");
static_assert((reg & cmd) == 0, "reg and cmd overlap");
static_assert((osu & cmd) == 0, "osu and cmd overlap");

struct Spu
{
    My_stack stk = {};
    My_stack adress = {};
    int *regs= {};
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
    #undef DEF_CMD
    #undef DEF_JMP
};

int Processor(Spu *prc);
void Print_content(Spu *prc);
void Processor_ctor(Spu *prc);



#endif
