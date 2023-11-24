#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "stack.h"
#include "consts.h"

struct Spu
{
    My_stack stk = {};
    int *regs= NULL;
    int status = NO_ERRORS_PRC;
    int *instr = NULL;
    int *osu = NULL;
};

int Processor(Spu *prc);
void Processor_ctor(Spu *prc);
void Processor_dtor(Spu *prc);

#endif
