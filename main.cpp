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
    Spu prc = {};

    Processor_ctor(&prc);

    Assembler(&prc);

    Processor(&prc);

    //Disassembler(&prc);

    return 0;
}


