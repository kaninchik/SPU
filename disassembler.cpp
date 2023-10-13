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


int Disassembler(Cpu *prc)
{
    FILE *fp2 = fopen("disassembler.txt", "w");

    for(int i = 0; i < prc->elem_count; i++)
    {
        printf("func = %d\n", prc->func_arr[i]);

        switch(prc->func_arr[i] & cmd)
        {
            case push:
            {
                int arg = prc->func_arr[i + 1];

                if(prc->func_arr[i] & reg)
                    fprintf(fp2, "%s %s\n", "push", prc->regs_name[arg]);

                if(prc->func_arr[i] & imm)
                    fprintf(fp2, "%s %d\n", "push", arg);
                i++;
                break;
            }

            case in:
            {
                fprintf(fp2, "%s\n", "in");
                break;
            }

            case div:
            {
                fprintf(fp2, "%s\n", "div");
                break;
            }

            case sub:
            {
                fprintf(fp2, "%s\n", "sub");
                break;
            }

            case out:
            {
                fprintf(fp2, "%s\n", "out");
                break;
            }

            case add:
            {
                fprintf(fp2, "%s\n", "add");
                break;
            }

            case mul:
            {
                fprintf(fp2, "%s\n", "mul");
                break;
            }

            case pop:
            {
                int arg = prc->func_arr[i + 1];

                if(prc->func_arr[i] & reg)
                    fprintf(fp2, "%s %s\n", "pop", prc->regs_name[arg]);

                i++;
                break;
            }

            case hlt:
            {
                fprintf(fp2, "%s\n", "hlt");
                break;
            }

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








