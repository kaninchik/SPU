#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <sys/stat.h>

#include "stack.h"
#include "security.h"
#include "dump.h"
#include "assembler.h"
#include "processor.h"
#include "disassembler.h"

static const char *ASM_FILE = "assembler.bin";
static const char *DISASM_FILE = "disassembler.txt";

static int *Get_opcode(int file_size, FILE *fp1);
static int Get_file_size(FILE *fp1);

void Convert_instr(int *instr, FILE *fp, const char *name);


int Disassembler()
{
    FILE *fp1 = fopen(ASM_FILE, "rb");
    FILE *fp2 = fopen(DISASM_FILE, "w");

    int file_size = Get_file_size(fp1);
    int *instr = Get_opcode(file_size, fp1);

    for(int i = 0; i < file_size/4; i++)
    {
        switch(instr[i] & cmd)
        {
            #define DEF_CMD(name, num, args, code)                                  \
                case(num):                                                          \
                {                                                                   \
                    if(args > 0)                                                    \
                    {                                                               \
                        Convert_instr(instr + i, fp2, #name);                       \
                        i++;                                                        \
                    }                                                               \
                    else                                                            \
                        fprintf(fp2, "%s\n", #name);                                \
                    break;                                                          \
                }

            #define DEF_JMP(name, num, sign)                                        \
                case(num):                                                          \
                {                                                                   \
                    fprintf(fp2, "%s :%d\n", #name, instr[i + 1]);                  \
                    i++;                                                            \
                    break;                                                          \
                }

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

static int *Get_opcode(int file_size, FILE *fp1)
{
    int instr_number = Get_file_size(fp1)/sizeof(int);
    int *instr = (int *)calloc(instr_number, sizeof(int));
    int n_sum = fread(instr, sizeof(int), instr_number, fp1);

    fclose(fp1);
    return instr;
}

static int Get_file_size(FILE *fp1)
{
    assert(fp1 != NULL);

    struct stat st = {};
    int fd = fileno(fp1);
    fstat(fd, &st);

    return st.st_size;
}

void Convert_instr(int *instr, FILE *fp, const char *name)
{
    assert(instr != nullptr);
    assert(fp != nullptr);
    assert(name != nullptr);

    if(*instr & osu)
    {
        if(*instr & imm)
            fprintf(fp, "%s [%d]\n", name, *(instr + 1));
        else
            if(*instr & reg)
                 fprintf(fp, "%s [%s]\n", name, register_names[*(instr + 1) - 1]);
    }
    else if(*instr & reg)
        fprintf(fp, "%s %s\n", name, register_names[*(instr + 1) - 1]);

    else if(*instr & imm)
        fprintf(fp, "%s %d\n", name, *(instr + 1));
}















