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

int Processor(my_stack *stk, Registrs *rgs)
{
    int n_func = 0;

    FILE *fp1 = fopen("assembler.txt", "r");

    while(fscanf(fp1, "%d", &n_func) != EOF)
    {
        switch(n_func)
        {
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

            case hlt:
            {
                break;
            }

            default:
            {
                Choose_func(stk, fp1, rgs, /*byte,*/ n_func);
                break;
            }
        }

        if(n_func == hlt)
            break;
    }

    fclose(fp1);

    return 0;
}

void Push(my_stack *stk, FILE *fp1)
{
    assert(stk != NULL);
    assert(fp1 != NULL);

    int Elem = 0;

    fscanf(fp1, " %d", &Elem);

    Stack_push(stk, Elem);
    Print_stack(stk);
}

void Push_r(my_stack *stk, FILE *fp1, Registrs *rgs)
{
    assert(stk != NULL);
    assert(fp1 != NULL);

    int num_reg = 0;

    fscanf(fp1, " %d", &num_reg);

    switch(num_reg)
    {
        case 1:
            Stack_push(stk, rgs->rax);
            break;

        case 2:
            Stack_push(stk, rgs->rbx);
            break;

        case 3:
            Stack_push(stk, rgs->rcx);
            break;

        case 4:
            Stack_push(stk, rgs->rdx);
            break;

        default:
            assert(!"Unknown registr");
            break;
    }

    Print_content(stk, rgs);
}

void In(my_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;

    printf("Enter a number:\n");
    scanf("%d", &x1);
    Stack_push(stk, x1);
    Print_stack(stk);
}

void Div(my_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2/x1);
    Print_stack(stk);
}

void Sub(my_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2 - x1);
    Print_stack(stk);
}

void Out(my_stack *stk)
{
    int x1 = 0;

    Stack_pop(stk, &x1);
    printf("You asked me: %d", x1);
    Print_stack(stk);
}

void Add(my_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2 + x1);
    Print_stack(stk);
}

void Mul(my_stack *stk)
{
    assert(stk != NULL);

    int x1 = 0;
    int x2 = 0;

    Stack_pop(stk, &x1);
    Stack_pop(stk, &x2);
    Stack_push(stk, x2 * x1);
    Print_stack(stk);
}

void Pop_r(my_stack *stk, FILE *fp1, Registrs *rgs)
{
    assert(stk != NULL);
    assert(rgs != NULL);

    int Elem = 0;
    int num_reg = 0;

    Stack_pop(stk, &Elem);
    fscanf(fp1, " %d", &num_reg);

    switch(num_reg)
    {
        case 1:
            rgs->rax = Elem;
            break;

        case 2:
            rgs->rbx = Elem;
            break;

        case 3:
            rgs->rcx = Elem;
            break;

        case 4:
            rgs->rdx = Elem;
            break;

        default:
            assert(!"Unknown registr");
            break;
    }

    Print_content(stk, rgs);
}


void Processor_ctor(my_stack *stk, Registrs *rgs)
{
    Stack_ctor(stk);

    rgs->rax = 0;
    rgs->rbx = 0;
    rgs->rcx = 0;
    rgs->rdx = 0;
}

void Choose_func(my_stack *stk, FILE *fp1, Registrs *rgs, /*flags byte,*/ int n_func)
{
    assert(stk != NULL);
    assert(fp1 != NULL);
    assert(rgs != NULL);

    switch(n_func & 15)
    {
        case push:
        {
            if((n_func & 240) == 32)
                Push_r(stk, fp1, rgs);
            else
                Push(stk, fp1);

            break;
        }

        case pop:
        {
            if((n_func & 240) == 32)
                Pop_r(stk, fp1, rgs);

            break;
        }

        default:
        {
            assert(!"Unknown function");
            break;
        }
    }
}

void Print_content(my_stack *stk, Registrs *rgs)
{
    assert(stk != NULL);
    assert(rgs != NULL);

    Print_stack(stk);

    printf("rax = %d\n", rgs->rax);
    printf("rbx = %d\n", rgs->rbx);
    printf("rcx = %d\n", rgs->rcx);
    printf("rdx = %d\n", rgs->rdx);
}












