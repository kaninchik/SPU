#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cctype>
#include <cassert>
#include <cmath>
#include <sys/stat.h>

#include "stack.h"
#include "consts.h"
#include "security.h"
#include "dump.h"
#include "assembler.h"
#include "processor.h"
#include "disassembler.h"

static const char *ASM_FILE = "assembler.bin";
static const char *USER_FILE = "quadratic.txt";
const int INSTRUCTIONS_NUM = 30;
const int LABELS_NUM = 30;

struct Label
{
    int label_address = 0;
    char label_name[LABELS_NUM] = {};
};

static int Get_file_size(FILE *fp);
int Get_label_name(char *arg_str, Label *lbs);
int Get_reg_name(char *arg_str);

void Skip_space(char **instructions);
void Skip_empty_lines(char **instructions, int *n_strings);
void Get_arg(char **instructions, int *opcode, int *instr_num, int num);
void Write_instructions(int *opcode, int instr_num);
void Skip_comments(char **instructions);
void Fill_opcode(int *opcode, void *argument, int flag, int instr_num);
void Get_labels(char *instructions, Label *lbs, int n_strings);

bool Is_user_input_correct(char *instructions);
bool Is_register(char *instructions);
bool Is_label(char* func_name);
bool Is_number(char *instructions);
bool Find_closing_bracket(char *instructions);

char *Fill_arr_with_inst(int *n_strings);

int Assembler()
{
    int arg = 0;

    char func_name[INSTRUCTIONS_NUM] = {};
    char arg_str[INSTRUCTIONS_NUM] = {};
    Label lbs[LABELS_NUM] = {};

    int n_strings = 0;
    char *instructions = Fill_arr_with_inst(&n_strings);
    int *opcode = (int *)calloc(2*(n_strings) + 1, sizeof(int));

    int instr_num = 0;

    Get_labels(instructions, lbs, n_strings);

    for(int i = 0; i < n_strings; i++)
    {
        Skip_empty_lines(&instructions, &n_strings);
        Skip_space(&instructions);
        sscanf(instructions, "%s", func_name);

        if(Is_label(func_name))
        {
            instructions = strlen(func_name) + instructions;
            Skip_space(&instructions);
        }

        #define DEF_CMD(name, num, args, code)                                              \
            if(strcmp(func_name, #name) == 0)                                               \
            {                                                                               \
                instructions = strlen(func_name) + instructions;                            \
                Skip_space(&instructions);                                                  \
                if(args > 0)                                                                \
                    Get_arg(&instructions, opcode, &instr_num, num);                        \
                else                                                                        \
                {                                                                           \
                    if(!Is_user_input_correct(instructions))                                \
                    {                                                                       \
                        printf("Bad syntax. This function doesn't have an arguments");      \
                        abort();                                                            \
                    }                                                                       \
                    instructions++;                                                         \
                    opcode[instr_num] = num;                                                \
                    instr_num++;                                                            \
                }                                                                           \
            }

        #define DEF_JMP(name, num, sign)                                                \
            if(strcmp(func_name, #name) == 0)                                           \
            {                                                                           \
                instructions = strlen(func_name) + instructions;                        \
                Skip_space(&instructions);                                              \
                sscanf(instructions, "%s", arg_str);                                    \
                opcode[instr_num] = num | imm;                                          \
                opcode[instr_num + 1] = Get_label_name(arg_str, lbs);                   \
                instr_num = instr_num + 2;                                              \
                instructions = instructions + strlen(arg_str);                          \
                Skip_space(&instructions);                                              \
                                                                                        \
                if(!Is_user_input_correct(instructions))                                \
                {                                                                       \
                    printf("Bad syntax. This function doesn't have an arguments");      \
                    abort();                                                            \
                }                                                                       \
            }

            #include"commands.h"
            #undef DEF_CMD
            #undef DEF_JMP
    }

    Write_instructions(opcode, instr_num);
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

bool Is_number(char *instructions)
{
    assert(instructions != NULL);

    if(*instructions == '-')
        instructions = instructions + 1;

    return isdigit(*instructions);
}

void Get_arg(char **instructions, int *opcode, int *instr_num, int num)
{
    assert(instructions != NULL);
    assert(opcode != NULL);
    assert(instr_num != NULL);

    int arg = 0;
    char arg_str[INSTRUCTIONS_NUM] = {};

    if(Is_osu(*instructions))
    {
        if(!Find_closing_bracket(*instructions))
            abort();

        *instructions = *instructions + 1;
        Skip_space(instructions);

        if(Is_register(*instructions))
        {
            sscanf(*instructions, "%[^ ]]", arg_str);
            Fill_opcode(opcode, arg_str, num | reg | osu, *instr_num);
        }
        else if(Is_number(*instructions))
        {
            sscanf(*instructions, "%d", &arg);
            Fill_opcode(opcode, &arg, num | imm | osu, *instr_num);
            itoa(arg, arg_str, 10);
        }
        else
        {
            printf("You haven't to write something after ]\n");
            abort();
        }
    }
    else if(Is_register(*instructions))
    {
        Skip_space(instructions);
        sscanf(*instructions, "%s", arg_str);
        Fill_opcode(opcode, arg_str, num | reg, *instr_num);
    }
    else if(Is_number(*instructions))
    {
        Skip_space(instructions);
        sscanf(*instructions, "%d", &arg);
        Fill_opcode(opcode, &arg, num | imm, *instr_num);
        itoa(arg, arg_str, 10);
    }
    else
    {
        printf("Bad syntax\n");
        abort();
    }
    *instructions = *instructions + strlen(arg_str);
    Skip_space(instructions);
    *instr_num = *instr_num + 2;

    if(!Is_user_input_correct(*instructions))
        abort();
}

bool Is_user_input_correct(char *instructions)
{
    assert(instructions != NULL);

    Skip_comments(&instructions);

    return (*instructions == '\n');
}

void Fill_opcode(int *opcode, void *argument, int flag, int instr_num)
{
    assert(opcode != nullptr);

    opcode[instr_num] = flag;

    if(flag & imm)
    {
        opcode[instr_num + 1] = *((int *)argument);
    }
    else if(flag & reg)
    {
        char *str_arg = (char *)argument;
        opcode[instr_num + 1] = Get_reg_name(str_arg);
    }
}

void Write_instructions(int *opcode, int instr_num)
{
    assert(opcode != NULL);

    FILE *fp1 = fopen(ASM_FILE, "wb");

    /*if(fp1 == NULL)
    {
        prc->status = prc->status | ERROR_WITH_OPENNING_FILE;
        Process_errors(prc);
    }*/

    fwrite(opcode, sizeof(int), instr_num, fp1);
    fclose(fp1);
}

char *Fill_arr_with_inst(int *n_strings)
{
    assert(n_strings != NULL);

    int strings = 0;
    FILE *fp = fopen(USER_FILE, "rb");

   /*if(fp == NULL)
    {
        prc->status = prc->status | ERROR_WITH_OPENNING_FILE;
        Process_errors(prc);
    }*/

    int file_size = Get_file_size(fp);
    char *instructions = (char *)calloc(file_size, sizeof(char));
    int n_sym = fread(instructions, sizeof(char), file_size, fp);

    /*if(n_sym != file_size)
    {
        prc->status = prc->status | ERROR_WITH_READING_FROM_FILE;
        Process_errors(prc);
    }*/

    for(int i = 0; i < file_size; i++)
    {
        if(instructions[i] == '\n')
            strings++;
    }
    *n_strings = strings;
    fclose(fp);

    return instructions;
}

void Skip_space(char **instructions)
{
    assert(instructions != NULL);

    while(**instructions == ' ' || **instructions == '\r' )
        *instructions = *instructions + 1;
}

void Skip_comments(char **instructions)
{
    assert(instructions != nullptr);

    if(**instructions == ';')
    {
        while(**instructions != '\n')
            *instructions = *instructions + 1;
    }
}

void Skip_empty_lines(char **instructions, int *n_strings)
{
    assert(instructions != NULL);

    while(**instructions == '\r' || **instructions == '\n')
    {
        *instructions = *instructions + 1;
        if(**instructions == '\n')
            *n_strings = *n_strings - 1;
    }
}

int Get_label_name(char *arg_str, Label *lbs)
{
    assert(arg_str != NULL);
    assert(lbs != NULL);

    for(int i = 0; i < LABELS_NUM; i++)
    {
        if(strcmp(arg_str, lbs[i].label_name) == 0)
            return lbs[i].label_address;
    }
    return -1;
}

int Get_reg_name(char *arg_str)
{
    assert(arg_str != nullptr);

    for(int i = 0; i < registers_num; i++)
    {
        if(strcmp(arg_str, register_names[i]) == 0)
            return i + 1;
    }

    assert(!"Unknown register");
}

bool Find_closing_bracket(char *instructions)
{
    assert(instructions != nullptr);

    while(*instructions != '\r')
    {
        if(*instructions == ']')
            return true;
        else
            instructions++;
    }
    return false;
}

static int Get_file_size(FILE *fp)
{
    assert(fp != NULL);

    struct stat st = {};
    int fd = fileno(fp);
    fstat(fd, &st);

    return st.st_size;
}

void Get_labels(char *instructions, Label *lbs, int n_strings)
{
    assert(instructions != nullptr);
    assert(lbs != nullptr);

    int num_lab = 0;
    int instr_num = 0;
    int arg = 0;

    char func_name[INSTRUCTIONS_NUM] = {};
    char arg_str[INSTRUCTIONS_NUM] = {};

    for(int i = 0; i < n_strings; i++)
    {
        Skip_empty_lines(&instructions, &n_strings);
        Skip_space(&instructions);
        sscanf(instructions, "%s", func_name);

        if(Is_label(func_name))
        {
            instructions = strlen(func_name) + instructions;
            Skip_space(&instructions);

            strcpy(lbs[num_lab].label_name, func_name);
            lbs[num_lab].label_address = instr_num;
            num_lab++;
        }
        else
        {
            #define DEF_CMD(name, num, args, code)                                              \
                if(strcmp(func_name, #name) == 0)                                               \
                {                                                                               \
                    instructions = strlen(func_name) + instructions;                            \
                    Skip_space(&instructions);                                                  \
                    if(args > 0)                                                                \
                    {                                                                           \
                        while(*instructions != '\n')                                            \
                            instructions++;                                                     \
                                                                                                \
                        instructions++;                                                         \
                        instr_num = instr_num + 2;                                              \
                    }                                                                           \
                    else                                                                        \
                    {                                                                           \
                        instructions++;                                                         \
                        instr_num++;                                                            \
                    }                                                                           \
                }                                                                               \

            #define DEF_JMP(name, num, sign)                                                \
                if(strcmp(func_name, #name) == 0)                                           \
                {                                                                           \
                    while(*instructions != '\n')                                            \
                        instructions++;                                                     \
                                                                                            \
                    instructions++;                                                         \
                    instr_num = instr_num + 2;                                              \
                }
        }
        #include"commands.h"
        #undef DEF_CMD
        #undef DEF_JMP
    }
}












