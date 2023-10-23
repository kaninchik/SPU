
#include<cstdio>
#include<cstring>
#include<cstdint>
#include<cassert>

#include"stack.h"
#include"security.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"


#define DEF_CMD(name, num, args, code)                                  \
    case(num):                                                          \
    {                                                                   \
        if(args > 0)                                                    \
        {                                                               \
            int arg = prc->func_arr[i + 1];                             \
                                                                        \
            if(prc->func_arr[i] & reg)                                  \
                fprintf(fp2, "%s %s\n", #name, regs_name[arg - 1]);     \
                                                                        \
            if(prc->func_arr[i] & imm)                                  \
                fprintf(fp2, "%s %d\n", #name , arg);                   \
                                                                        \
             i++;                                                       \
        }                                                               \
        else                                                            \
            fprintf(fp2, "%s\n", #name);                                \
                                                                        \
        break;                                                          \
    }


int Disassembler(Spu *prc)
{
    FILE *fp2 = fopen("disassembler.txt", "w");

    for(int i = 0; i < prc->elem_count; i++)
    {
        switch(prc->func_arr[i] & cmd)
        {
            #include"commands.h"
            #undef DEF_CMD

            default:
            {
                assert(!"Unknown command!");
                break;
            }
        }
    }

    fclose(fp2);

    return 0;
}








