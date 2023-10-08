#include<cstdio>
#include<cstring>
#include<cstdint>
#include<cassert>

#include"stack.h"
#include"security.h"
#include"kernel_func.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"


int Disassembler()
{
    FILE *fp1 = fopen("assembler.txt", "r");
    FILE *fp2 = fopen("disassembler.txt", "w");

    int n_func = 0;

    while(fscanf(fp1, "%d", &n_func) != EOF)
    {
        switch(n_func)
        {
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


            case hlt:
            {
                fprintf(fp2, "%s\n", "hlt");
                break;
            }

            default:
            {
                Define_func(fp1, fp2, n_func);
                break;
            }
        }
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}

void Define_func(FILE *fp1, FILE *fp2, int n_func)
{
    int Arg = 0;

    printf("n_func = %d\n", n_func);
    printf("n_func & 15 = %d\n", n_func & 15);

    switch(n_func & 15)
    {
        case push:
        {
            if((n_func & 240) == 32)
            {
                fscanf(fp1, "%d", &Arg);
                fprintf(fp2, "%s", "push");
                Write_reg(fp2, Arg);
            }

            else
            {
                fscanf(fp1, "%d", &Arg);
                fprintf(fp2, "%s %d\n", "push", Arg);
            }

            break;
        }

        case pop:
        {
            if((n_func & 240) == 32)
            {
                fscanf(fp1, "%d", &Arg);
                fprintf(fp2, "%s", "pop");
                Write_reg(fp2, Arg);
            }

            break;
        }

        default:
        {
            assert(!"Unknown command");
            break;
        }
    }
}



void Write_reg(FILE *fp2, int Arg)
{
    assert(fp2 != NULL);

    switch(Arg)
    {
        case 1:
        {
            fprintf(fp2, " %s\n", "rax");
            break;
        }

        case 2:
        {
            fprintf(fp2, " %s\n", "rbx");
            break;
        }

        case 3:
        {
            fprintf(fp2, " %s\n", "rcx");
            break;
        }

        case 4:
        {
            fprintf(fp2, " %s\n", "rdx");
            break;
        }

        default:
        {
            assert(!"Unknown registr");
            break;
        }
    }
}







