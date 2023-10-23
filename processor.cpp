#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cstdint>
#include<cassert>

#include"stack.h"
#include"security.h"
#include"consts.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"

#define PUSH(arg)    Stack_push(&prc->stk, arg)
#define POP(arg)     Stack_pop(&prc->stk, &arg)
#define REGS         prc->regs
#define CMD          prc->instr
#define INDEX        i
#define OSU          prc->osu
#define ARG          INDEX + 1

int Processor(Spu *prc)
{
    int x1 = 0;
    int x2 = 0;

    for(int i = 0; prc->instr[i] != hlt; i++)
    {
        switch(prc->instr[i] & cmd)
        {
            #define DEF_CMD(name, num, args, code)        \
                case(num):                                \
                    code                                  \
                    break;

            #define DEF_JMP(name, num, sign)                 \
                case(num):                                   \
                {                                            \
                    if(num == call)                          \
                    {                                        \
                        PUSH(INDEX + 1);                     \
                        INDEX = CMD[ARG] - 1;          \
                                                             \
                    }                                        \
                    else if(num == jump)                     \
                    {                                        \
                        INDEX = CMD[ARG] - 1;          \
                    }                                        \
                    else                                     \
                    {                                        \
                        POP(x1);                             \
                        POP(x2);                             \
                        if(x2 sign x1)                       \
                                                             \
                            INDEX = CMD[ARG] - 1;      \
                        else                                 \
                            INDEX++;                         \
                    }                                        \
                    break;                                   \
                }                                            \

            #include"commands.h"

            #undef DEF_CMD
            #undef DEF_JMP
            #undef PUSH
            #undef POP
            #undef REGS
            #undef CMD
            #undef INDEX
            #undef OSU
            #undef ARG

            default:
            {
                assert(!"Unknown function");
                break;
            }
        }
    }

    return 0;
}

void Processor_ctor(Spu *prc)
{
    assert(prc != NULL);

    Stack_ctor(&prc->stk);
    prc->instr_number = 0;
    prc->instr = NULL;
    prc->regs = (int *)calloc(reg_amount, sizeof(int));
    prc->osu = (int *)calloc(100, sizeof(int));
}

void Print_content(Spu *prc)
{
    assert(prc != NULL);

    Print_stack(&prc->stk);

    for(int i = 1; i < reg_amount; i++)
        printf("reg %d = %d\n", i, prc->regs[i]);

    for(int i = 0; i < 100; i++)
        printf("OSU %d = %d\n", i, prc->osu[i]);
}

















