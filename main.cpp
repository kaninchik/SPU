#include<cstdio>
#include<cstring>
#include<cstdint>

#include"stack.h"
#include"security.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"

int main()
{
    My_stack stk = {};

    Cpu prc;

    Processor_ctor(&stk, &prc);

    Assembler(&prc);

    Processor(&stk, &prc);

    Disassembler(&prc);

    Stack_dtor(&stk);

    return 0;
}
