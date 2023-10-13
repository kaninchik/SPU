#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cstdint>
#include<cctype>
#include<cassert>

#include"stack.h"
#include"security.h"
#include"dump.h"
#include"assembler.h"
#include"processor.h"
#include"disassembler.h"


int Assembler(Cpu *prc)
{
    FILE *fp = fopen("calc.txt", "r");

    int count_commands = 0;
    int arg = 0;
    char func_name[5] = {};
    char reg_name[5] = {};

    fscanf(fp, "%d", &count_commands);

    Instructions commands[count_commands];

    for(int i = 0; i < count_commands; i++)
    {
        fscanf(fp, "%s ", func_name);

        if(strcmp(func_name, "push") == 0)
        {
            commands[i].n_func = push;

            if(Is_registrs(fp))
            {
                fscanf(fp, "%s", reg_name);
                commands[i].arg = reg_name[1] - 'a' + 1;
                commands[i].is_reg = true;
            }
            else
            {
                fscanf(fp, "%d", &arg);
                commands[i].arg = arg;
                commands[i].is_reg = false;
            }
        }

        if(strcmp(func_name, "pop") == 0)
        {
            commands[i].n_func = pop;

            if(Is_registrs(fp))
            {
                fscanf(fp, " %s", reg_name);
                commands[i].arg = reg_name[1] - 'a' + 1;
                commands[i].is_reg = true;

            }
        }

        if(strcmp(func_name, "in") == 0)
            commands[i].n_func = in;

        if(strcmp(func_name, "div") == 0)
            commands[i].n_func = div;

        if(strcmp(func_name, "sub") == 0)
            commands[i].n_func = sub;

        if(strcmp(func_name, "add") == 0)
            commands[i].n_func = add;

        if(strcmp(func_name, "mul") == 0)
            commands[i].n_func = mul;

        if(strcmp(func_name, "out") == 0)
            commands[i].n_func = out;

        if(strcmp(func_name, "hlt") == 0)
            commands[i].n_func = hlt;
    }

    Creat_file(commands, count_commands, prc);

    fclose(fp);

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

void Creat_file(Instructions *commands, int count_commands, Cpu *prc)
{
    assert(commands != NULL);

    FILE *fp1 = fopen("assembler.bin", "wb");

    int all_func[2*count_commands];
    int size = 0;

    for(int i = 0; i < count_commands; i++)
    {
        if(commands[i].n_func == push)
        {
            if(commands[i].is_reg)
                all_func[size++] = push | (1 << 5);
            else
                all_func[size++] = push | (1 << 4);

            all_func[size++] = commands[i].arg;
        }
        else if(commands[i].n_func == pop)
        {
            if(commands[i].is_reg)
                all_func[size++] = pop | (1 << 5);

            all_func[size++] = commands[i].arg;
        }
        else
            all_func[size++] = commands[i].n_func;
    }

    fwrite(all_func, sizeof(int), size, fp1);

    prc->elem_count = size;

    fclose(fp1);
}



