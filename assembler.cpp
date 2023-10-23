#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cstdint>
#include<cctype>
#include<cassert>
#include<sys/stat.h>

#include"stack.h"
#include"consts.h"
#include"security.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"


int Assembler(Spu *prc)
{
    int arg = 0;
    int num_lab = 0;
    char func_name[INSTRUCTIONS] = {};
    char arg_str[INSTRUCTIONS] = {};
    Labels lbs[LABELS] = {};
    int n_strings = 0;
    char *instructions = Read_instructions(prc, &n_strings);
    char *tmp = instructions;

    for(int j = 0; j < 2; j++)
    {
        for(int i = 0; i < n_strings; i++)
        {
            sscanf(instructions, "%s", func_name);

            if(Is_label(func_name))
            {
                instructions = strlen(func_name) + 1 + instructions;
                if(j == 0)
                {
                    strcpy(lbs[num_lab].label_name, func_name);
                    lbs[num_lab].label = prc->instr_number;
                    num_lab++;
                }
            }
            #define DEF_CMD(name, num, args, code)                                          \
                if(strcmp(func_name, #name) == 0)                                           \
                {                                                                           \
                    instructions = strlen(func_name) + 1 + instructions;                    \
                                                                                            \
                    if(args > 0)                                                            \
                        Get_arg(&instructions, prc, arg_str, num);                          \
                    else                                                                    \
                    {                                                                       \
                        prc->instr[prc->instr_number] = num;                                \
                        prc->instr_number++;                                                \
                }                                                                           \

            #define DEF_JMP(name, num, sign)                                             \
                if(strcmp(func_name, #name) == 0)                                        \
                {                                                                        \
                    instructions = strlen(func_name) + 1 + instructions;                 \
                    sscanf(instructions, "%s", arg_str);                                 \
                    prc->instr[prc->instr_number] = num | imm;                           \
                    prc->instr_number++;                                                 \
                    prc->instr[prc->instr_number] = Label_name(arg_str, lbs);            \
                    prc->instr_number++;                                                 \
                    instructions = instructions + strlen(arg_str) + 1;                   \
                }                                                                        \

                #include"commands.h"
                #undef DEF_CMD
                #undef DEF_JMP
        }
        if(j == 0)
        {
            instructions = tmp;
            prc->instr_number = 0;
        }
    }

    for(int i = 0; i < prc->instr_number; i++)
        printf("elem %d = %d\n", i, prc->instr[i]);

    printf("prc->instr_number = %d", prc->instr_number);

    Create_file(prc);

    return 0;
}

bool Is_register(char *instructions)
{
    assert(instructions != NULL);
    return (*instructions == 'r');
}

bool Is_label(char *func_name)
{
    assert(func_name != NULL);
    return (*func_name == ':');
}

bool Is_osu(char *instructions)
{
    assert(instructions != NULL);
    return (*instructions == '[');
}

void Get_arg(char **instructions, Spu *prc, char *arg_str, int num)
{
    assert(instructions != NULL);
    assert(prc != NULL);
    assert(arg_str != NULL);

    int arg = 0;

    if(Is_osu(*instructions))
    {
        *instructions = *instructions + 2;
        if(Is_register(*instructions))
        {
            sscanf(*instructions, "%s", arg_str);
            prc->instr[prc->instr_number] = num | reg | osu;
            prc->instr_number++;
            prc->instr[prc->instr_number] = What_reg(arg_str);
            prc->instr_number++;
            *instructions = *instructions + strlen(arg_str) + 3;
        }
        else
        {
            sscanf(*instructions, "%d", &arg);
            prc->instr[prc->instr_number] = num | imm | osu;
            prc->instr_number++;
            prc->instr[prc->instr_number] = arg;
            prc->instr_number++;
            itoa(arg, arg_str, 10);
            *instructions = *instructions + strlen(arg_str) + 3;
        }
    }
    else if(Is_register(*instructions))
    {
        sscanf(*instructions, "%s", arg_str);
        prc->instr[prc->instr_number] = num | reg;
        prc->instr_number++;
        prc->instr[prc->instr_number] = What_reg(arg_str);
        prc->instr_number++;
        *instructions = *instructions + strlen(arg_str) + 1;
    }
    else
    {
        sscanf(*instructions, "%d", &arg);
        prc->instr[prc->instr_number] = num | imm;
        prc->instr_number++;
        prc->instr[prc->instr_number] = arg;
        prc->instr_number++;
        itoa(arg, arg_str, 10);
        *instructions = *instructions + strlen(arg_str) + 1;
    }
}

void Create_file(Spu *prc)
{
    assert(prc != NULL);

    FILE *fp1 = fopen("assembler.bin", "wb");

    fwrite(prc->instr, sizeof(int), prc->instr_number, fp1);

    fclose(fp1);
}

char *Read_instructions(Spu *prc, int *n_strings)
{
    assert(prc != NULL);
    assert(n_strings != NULL);

    FILE *fp = fopen("calc.txt", "r");
    int strings = 0;

    struct stat st = {};
    int fd = fileno(fp);

    fstat(fd, &st);

    char *instructions = (char *)calloc(st.st_size, sizeof(char));

    fread(instructions, sizeof(char), st.st_size, fp);

   for(int i = 0; i < st.st_size; i++)
    {
        if(instructions[i] == '\n')
            strings++;
    }
    *n_strings = strings;

    prc->instr = (int *)calloc(2*(strings + 1), sizeof(int));

    fclose(fp);

    return instructions;
}

int Label_name(char *arg_str, Labels lbs[LABELS])
{
    assert(arg_str != NULL);

    for(int i = 0; i < LABELS; i++)
    {
        if(strcmp(arg_str, lbs[i].label_name) == 0)
            return lbs[i].label;
    }

    return -1;
}

int What_reg(char *arg_str)
{
    assert(arg_str !=  NULL);

    static const char *regs_name[] = {"rax",
                                  "rbx",
                                  "rcx",
                                  "rdx",
                                  "rsi",
                                  "rdi",
                                  "rbp",
                                  "rsp",
                                  "r08",
                                  "r09",
                                  "r10",
                                  "r11",
                                  "r12",
                                  "r13",
                                  "r14",
                                  "r15"};

    for(int i = 0; i < reg_amount; i++)
    {
        if(strcmp(arg_str, regs_name[i]) == 0)
            return i + 1;
    }

    assert(!"Unknown register! File: assembler.cpp");
}






