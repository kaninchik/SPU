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

static const char *ASM_FILE = "assembler.bin";
static const char *USER_FILE = "calc1.txt";
const int INSTRUCTIONS = 10;
const int LABELS = 10;

struct Labels
{
    int label = 0;
    char label_name[LABELS] = {};
};

int Label_name(char *arg_str, Labels lbs[LABELS]);
int What_reg(char *arg_str);
void Skip_space(char **instructions);
void Get_arg(char **instructions, Spu *prc, char *arg_str, int num);
bool Is_register(char *instructions);
bool Is_label(char* func_name);
void Create_file(Spu *prc);
char *Read_instructions(Spu *prc, int *n_strings);

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
            Skip_space(&instructions);
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
            #define DEF_CMD(name, num, args, code)                                              \
                if(strcmp(func_name, #name) == 0)                                               \
                {                                                                               \
                    instructions = strlen(func_name) + instructions;                            \
                    Skip_space(&instructions);                                                  \
                    if(args > 0)                                                                \
                        Get_arg(&instructions, prc, arg_str, num);                              \
                    else                                                                        \
                    {                                                                           \
                        if(*instructions != '\n')                                               \
                        {                                                                       \
                            printf("Bad syntax. This function doesn't have an arguments");      \
                            exit(1);                                                            \
                        }                                                                       \
                        else                                                                    \
                        {                                                                       \
                            instructions++;                                                     \
                            prc->instr[prc->instr_number] = num;                                \
                            prc->instr_number++;                                                \
                        }                                                                       \
                    }                                                                           \
                }

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
                }

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
        printf("element %d = %d\n", i, prc->instr[i]);

    Create_file(prc);

    return 0;
}

bool Is_register(char *instructions)
{
    assert(instructions != NULL);
    return isalpha(*instructions);
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
    {   *instructions = *instructions + 1;
        Skip_space(instructions);
        if(Is_register(*instructions))
        {
            sscanf(*instructions, "%[^ ]]", arg_str);
            puts(arg_str);
            prc->instr[prc->instr_number] = num | reg | osu;
            prc->instr[prc->instr_number + 1] = What_reg(arg_str);
            prc->instr_number = prc->instr_number + 2;
            *instructions = *instructions + strlen(arg_str);
            Skip_space(instructions);
            Check_input_if_ram(instructions);

        }
        else
        {
            sscanf(*instructions, "%d", &arg);
            prc->instr[prc->instr_number] = num | imm | osu;
            prc->instr[prc->instr_number + 1] = arg;
            prc->instr_number = prc->instr_number + 2;
            itoa(arg, arg_str, 10);
            *instructions = *instructions + strlen(arg_str);
            Skip_space(instructions);
            Check_input_if_ram(*instructions);
        }
    }
    else if(Is_register(*instructions))
    {
        Skip_space(instructions);
        sscanf(*instructions, "%s", arg_str);
        prc->instr[prc->instr_number] = num | reg;
        prc->instr[prc->instr_number + 1] = What_reg(arg_str);
        prc->instr_number = prc->instr_number + 2;
        *instructions = *instructions + strlen(arg_str);
        Skip_space(instructions);
        Check_input(instructions);
    }
    else
    {
        Skip_space(instructions);
        sscanf(*instructions, "%d", &arg);
        prc->instr[prc->instr_number] = num | imm;
        prc->instr[prc->instr_number + 1] = arg;
        prc->instr_number = prc->instr_number + 2;
        itoa(arg, arg_str, 10);
        *instructions = *instructions + strlen(arg_str);
        Skip_space(instructions);
        Check_input(instructions);
    }
}

void Check_input(char **instructions)
{
    assert(instructions != NULL);

    if(**instructions != '\n')
    {
        printf("Bad syntax");
        exit(1);
    }
    else
        *instructions = *instructions + 1
}

void Check_input_if_ram(char **instructions)
{
    if(**instructions != ']')
    {
        printf("Bad syntax. You haven't to write symbols in OSU after register's name\n");
        exit(1);
    }
    else
    {
        *instructions = *instructions + 1;
        Skip_space(instructions);
    }

    if(**instructions != '\n')
    {
        printf("Bad syntax. You haven't to write symbols after ']'\n");
        exit(1);
    }
    else
        *instructions = *instructions + 1;
}

void Create_file(Spu *prc)
{
    assert(prc != NULL);
    FILE *fp1 = fopen(ASM_FILE, "wb");

    if(fp1 == NULL)
    {
        prc->status = prc->status | ERROR_WITH_OPENNING_FILE;
        Process_errors(prc);
    }

    fwrite(prc->instr, sizeof(int), prc->instr_number, fp1);
    fclose(fp1);
}

char *Read_instructions(Spu *prc, int *n_strings)
{
    assert(prc != NULL);
    assert(n_strings != NULL);

    int strings = 0;
    FILE *fp = fopen(USER_FILE, "r");

   if(fp == NULL)
    {
        prc->status = prc->status | ERROR_WITH_OPENNING_FILE;
        Process_errors(prc);
    }

    struct stat st = {};
    int fd = fileno(fp);

    fstat(fd, &st);

    char *instructions = (char *)calloc(st.st_size, sizeof(char));

    int n_sym = fread(instructions, sizeof(char), st.st_size, fp);

    /*if(n_sym != st.st_size)
    {
        prc->status = prc->status | ERROR_WITH_READING_FROM_FILE;
        Process_errors(prc);
    }*/

    for(int i = 0; i < st.st_size; i++)
    {
        if(instructions[i] == '\n')
            strings++;
    }
    *n_strings = strings;

    prc->instr = (int *)calloc(2*(strings + 1), sizeof(int));

    if(prc->instr == NULL)
    {
        prc->status = prc->status | ARRAY_IS_NULLPTR;
        Process_errors(prc);
    }

    fclose(fp);

    return instructions;
}

void Skip_space(char **instructions)
{
    assert(instructions != NULL);

    while(**instructions == ' ')
        *instructions = *instructions + 1;
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

    puts(arg_str);

    for(int i = 0; i < reg_amount; i++)
    {
        if(strcmp(arg_str, registers_name[i]) == 0)
            return i + 1;
    }

    assert(!"Unknown register! File: assembler.cpp");
}






