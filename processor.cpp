#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cstdint>
#include<cassert>
#include<sys/stat.h>

#include"stack.h"
#include"security.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"


int Processor(My_stack *stk, Cpu *prc)
{
    Process_file(prc);

    for(int i = 0; prc->func_arr[i] != hlt; i++)
    {
        switch(prc->func_arr[i] & cmd)
        {
            case push:
            {
                if(prc->func_arr[i] & reg)
                    Push_r(stk, prc, prc->func_arr[i + 1]);

                if(prc->func_arr[i] & imm)
                    Push(stk, prc->func_arr[i + 1]);

                i++;
                break;
            }

            case in:
            {
                In(stk);
                break;
            }

            case div:
            {
                Div(stk);
                break;
            }

            case sub:
            {
                Sub(stk);
                break;
            }

            case out:
            {
                Out(stk);
                break;
            }

            case add:
            {
                Add(stk);
                break;
            }

            case mul:
            {
                Mul(stk);
                break;
            }

            case pop:
            {
                if(prc->func_arr[i] & reg)
                {
                    Pop_r(stk, prc, prc->func_arr[i + 1]);
                    i++;
                    break;
                }
            }

            default:
            {
                assert(!"Unknown function");
                break;
            }
        }
    }

    return 0;
}

void Push(My_stack *stk, int Elem)
{
    assert(stk != NULL);

    Stack_push(stk, Elem);
    Print_stack(stk);
}

void Push_r(My_stack *stk, Cpu *prc, int num_reg)
{
    assert(stk != NULL);

    Stack_push(stk, prc->regs[num_reg]);

    Print_content(stk, prc);
}

void In(My_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;

    printf("Enter a number:\n");
    scanf("%d", &x1);
    Stack_push(stk, x1);
    Print_stack(stk);
}

void Div(My_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2/x1);
    Print_stack(stk);
}

void Sub(My_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2 - x1);
    Print_stack(stk);
}

void Out(My_stack *stk)
{
    int x1 = 0;

    Stack_pop(stk, &x1);
    printf("You asked me: %d", x1);
    Print_stack(stk);
}

void Add(My_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2 + x1);
    Print_stack(stk);
}

void Mul(My_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2 * x1);
    Print_stack(stk);
}

void Pop_r(My_stack *stk, Cpu *prc, int num_reg)
{
    assert(stk != NULL);
    assert(prc != NULL);

    int Elem = 0;

    Stack_pop(stk, &Elem);

    prc->regs[num_reg] = Elem;

    Print_content(stk, prc);
}

void Processor_ctor(My_stack *stk, Cpu *prc)
{
    Stack_ctor(stk);

    prc->elem_count = 0;
    prc->func_arr = NULL;
    prc->regs[reg_amount] = {};
}

void Print_content(My_stack *stk, Cpu *prc)
{
    assert(stk != NULL);
    assert(prc != NULL);

    Print_stack(stk);

    for(int i = 1; i < reg_amount; i++)
        printf("reg %d = %d\n", i, prc->regs[i]);
}

void Process_file(Cpu *prc)
{
    FILE *fp1 = fopen("assembler.bin", "rb");

    prc->func_arr = (int *)calloc(prc->elem_count, sizeof(int));

    fread(prc->func_arr, sizeof(int), prc->elem_count, fp1);

    fclose(fp1);
}


















