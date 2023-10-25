DEF_CMD(push, 1, 1,
    {
        if(CMD[INDEX] & osu)
        {
            if(CMD[INDEX] & imm)
                PUSH(OSU[ARG]);

            else if(CMD[INDEX] & reg)
            {
                x1 = REGS[CMD[ARG]];
                PUSH(OSU[x1]);
            }
            else
                assert(!"Unknown command");
        }

        if(CMD[INDEX] & reg)
        {
            PUSH(REGS[CMD_ARG]);
            Print_content(prc);
        }

        if(CMD[INDEX] & imm)
        {
            PUSH(CMD[ARG]);
            Print_stack(&prc->stk);
        }
        INDEX++;})

DEF_CMD(pop, 11, 1,
    {
        if(CMD[INDEX] & osu)
        {
            if(CMD[INDEX] & imm)
            {
                POP(x1);
                OSU[CMD[ARG]] = x1;
            }
            else if(CMD[INDEX] & reg)
            {
                POP(x1);
                OSU[REGS[CMD_ARG]] = x1;
            }
            else
                assert(!"Unknown command");
        }

        if(CMD[INDEX] & reg)
        {
            POP(x1);
            REGS[CMD[ARG]] = x1;
            Print_content(prc);
        }
        INDEX++;})

DEF_CMD(in, 2, 0,
    {
        printf("Enter a number:\n");
        scanf("%d", &x1);
        PUSH(x1);
        Print_stack(&prc->stk);
    })

DEF_CMD(div, 3, 0,
    {
        POP(x1);
        POP(x2);
        PUSH(x2/x1);
        Print_stack(&prc->stk);
    })

DEF_CMD(sub, 4, 0,
    {
        POP(x1);
        POP(x2);
        PUSH(x2 - x1);
        Print_stack(&prc->stk);
    })

DEF_CMD(add, 5, 0,
    {
        POP(x1);
        POP(x2);
        PUSH(x2 + x1);
        Print_stack(&prc->stk);
    })

DEF_CMD(mul, 6, 0,
    {
        POP(x1);
        POP(x2);
        PUSH(x2 * x1);
        Print_stack(&prc->stk);
    })

DEF_CMD(out, 7, 0,
    {
        POP(x1);
        printf("You asked me: %d", x1);
        Print_stack(&prc->stk);
    })

DEF_CMD(ret, 21, 0,
    {
        POP(x1);
        INDEX = x1;
    })
DEF_CMD(hlt, 12, 0, )



DEF_JMP(jump, 13, || 1 ||)
DEF_JMP(ja, 14, >)
DEF_JMP(jae, 15, >=)
DEF_JMP(jb, 16, <)
DEF_JMP(jbe, 17, <=)
DEF_JMP(je, 18, ==)
DEF_JMP(jne, 19, !=)
DEF_JMP(call, 20, || 1 ||)



