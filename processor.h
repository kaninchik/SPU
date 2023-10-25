#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include"stack.h"
#include"consts.h"

struct Spu
{
    My_stack stk = {};
    My_stack adress = {};
    int *regs= NULL;
    int status = NO_ERRORS_PRC;
    int *instr = NULL;
    int *osu = NULL;
    int instr_number = 0;
};

int Processor(Spu *prc);
void Print_content(Spu *prc);
void Processor_ctor(Spu *prc);
void Processor_dtor(Spu *prc);
void Process_errors(Spu *prc);

#endif
