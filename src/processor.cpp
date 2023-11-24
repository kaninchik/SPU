#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <sys/stat.h>

#include "stack.h"
#include "security.h"
#include "consts.h"
#include "dump.h"
#include "assembler.h"
#include "processor.h"
#include "disassembler.h"

static const char *ASM_FILE = "assembler.bin";

void Print_content(Spu *prc);
void Process_errors(Spu *prc);

enum Opcode
{
    #define DEF_CMD(name, num, args, code)  \
        name = num,


    #define DEF_JMP(name, num, sign)  \
        name = num,


    #include"commands.h"
    #undef DEF_CMD
    #undef DEF_JMP
};

static int Get_file_size(FILE *fp1);
void Get_opcode(Spu *prc);

int Processor(Spu *prc)
{
    Get_opcode(prc);
    int x1 = 0;
    int x2 = 0;

    for(int i = 0; prc->instr[i] != hlt; i++)
    {
        #define PUSH(arg)    Stack_push(&prc->stk, arg)
        #define POP(arg)     Stack_pop(&prc->stk, &arg)
        #define REGS         prc->regs
        #define CMD          prc->instr
        #define INDEX        i
        #define OSU          prc->osu
        #define CMD_ARG      CMD[ARG]
        #define ARG          INDEX + 1

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
                        INDEX = CMD[ARG] - 1;                \
                    }                                        \
                    else                                     \
                    {                                        \
                        POP(x1);                             \
                        POP(x2);                             \
                        if(x2 sign x1)                       \
                            INDEX = CMD_ARG - 1;             \
                        else                                 \
                            INDEX++;                         \
                    }                                        \
                    break;                                   \
                }

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

void Get_opcode(Spu *prc)
{
    FILE *fp1 = fopen(ASM_FILE, "rb");

    /*if(fp1 == NULL)
    {
        prc->status = prc->status | ERROR_WITH_OPENNING_FILE;
        Process_errors(prc);
    }*/

    int instr_number = Get_file_size(fp1)/sizeof(int);
    prc->instr = (int *)calloc(instr_number, sizeof(int));
    int n_sum = fread(prc->instr, sizeof(int), instr_number, fp1);

    /*if(n_sum != prc->instr_number/4)
    {
        prc->status = prc->status | ERROR_WITH_READING_FROM_FILE;
        Process_errors(prc);
    }*/

    fclose(fp1);
}

static int Get_file_size(FILE *fp1)
{
    assert(fp1 != NULL);

    struct stat st = {};
    int fd = fileno(fp1);
    fstat(fd, &st);

    return st.st_size;
}

void Processor_ctor(Spu *prc)
{
    assert(prc != NULL);

    Stack_ctor(&prc->stk);
    prc->instr = NULL;
    prc->regs = (int *)calloc(registers_num, sizeof(int));

    if(prc->regs == NULL)
    {
        prc->status = ARRAY_IS_NULLPTR;
        Process_errors(prc);
    }

    prc->osu = (int *)calloc(100, sizeof(int));

    if(prc->osu == NULL)
    {
        prc->status = ARRAY_IS_NULLPTR;
        Process_errors(prc);
    }
}

void Processor_dtor(Spu *prc)
{
    Stack_dtor(&prc->stk);
    free(prc->regs);
    free(prc->osu);
    free(prc->instr);
}

void Print_content(Spu *prc)
{
    assert(prc != NULL);

    //Print_stack(&prc->stk);

    /*for(int i = 1; i < 3; i++)
        printf("reg %d = %d\n", i, prc->regs[i]);*/

    /*for(int i = 1; i < 100; i++)
        printf("OSU %d = %d\n", i, prc->osu[i]);*/
}

void Process_errors(Spu *prc)
{
    assert(prc != NULL);

    printf("Hello, dear programmer.\n"
           "I am an error-handling function"
           "and I volunteered for a reason,"
           "but at the call of my heart.\n"
           "Your processor is not in the best condition at the moment.\n"
           "Now you'll find out why.\n"
           "Good luck, sweetheart\n");


    Stack_check(&prc->stk);
    STACK_DUMP_IF_ERROR(&prc->stk);

    for(int i = 0; i <= 8; i = i << 1)
    {
        switch(prc->status & i)
        {
            case NO_ERRORS_PRC:
            {
                break;
            }
            case ERROR_WITH_OPENNING_FILE:
            {
                printf("I can't open your file\n");
                break;
            }
            case ERROR_WITH_READING_FROM_FILE:
            {
                printf("I can't reading from your file\n");
                break;
            }
            case ERROR_WITH_WRITING_IN_FILE:
            {
                printf("I can't writing in your file\n");
                break;
            }
            case ARRAY_IS_NULLPTR:
            {
                printf("Calloc calloc hasn't allocated memory");
                break;
            }
            default:
                assert(!"Unknown error");
        }
    }
}

















