#include<cstdio>
#include<cstring>
#include<cstdint>

#include"stack.h"
#include"security.h"
#include"kernel_func.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"

int main()
{
    my_stack stk = {};

    Registrs rgs = {};

    /*flags byte = {15, 15};*/

    Processor_ctor(&stk, &rgs);

    Assembler();

    Disassembler();

    Processor(&stk, &rgs /*byte*/);


    Stack_dtor(&stk);

    return 0;
}
