#include<cstdio>
#include<cstring>
#include<cstdint>
#include <ctype.h>
#include<cassert>

#include"stack.h"
#include"security.h"
#include"kernel_func.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"


int Assembler()
{
    FILE *fp = fopen("calc.txt", "r");
    FILE *fp1 = fopen("assembler.txt", "w");

    char command[20] = {};
    char reg[4] = {};
    int Elem = 0;

    while(fscanf(fp, "%s ", command) != EOF)
    {
        if(strcmp(command, "push") == 0)
        {
            if(Is_registrs(fp))
            {
                fscanf(fp, " %s", reg);
                fprintf(fp1, "%d %d\n", push | (1 << 5), reg[1] - 'a' + 1);
            }
            else
            {
                fscanf(fp, " %d", &Elem);
                fprintf(fp1, "%d %d\n", push | (1 << 4), Elem);
            }
        }

        if(strcmp(command, "in") == 0)
            fprintf(fp1, "%d\n", 2);

        if(strcmp(command, "pop") == 0)
        {
            if(Is_registrs(fp))
            {
                fscanf(fp, " %s", reg);
                fprintf(fp1, "%d %d\n", pop | (1 << 5), reg[1] - 'a' + 1);
            }
        }

        if(strcmp(command, "div") == 0)
            fprintf(fp1, "%d\n", 3);

        if(strcmp(command, "sub") == 0)
            fprintf(fp1, "%d\n", 4);

        if(strcmp(command, "add") == 0)
            fprintf(fp1, "%d\n", 5);

        if(strcmp(command, "mul") == 0)
            fprintf(fp1, "%d\n", 6);

        if(strcmp(command, "out") == 0)
            fprintf(fp1, "%d\n", 7);

        if(strcmp(command, "hlt") == 0)
            fprintf(fp1, "%d\n", -1);
    }

    fclose(fp);
    fclose(fp1);

    return 0;
}

bool Is_registrs(FILE *fp)
{
    assert(fp != NULL);

    int sym = fgetc(fp);

    fseek(fp, -1, SEEK_CUR);

    if(isalpha(sym))
        return 1;
    else
        return 0;
}



