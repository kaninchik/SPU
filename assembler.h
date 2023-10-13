#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include<cstdio>
#include"stack.h"
#include"processor.h"

struct Instructions
{
    int n_func;
    int arg;
    bool is_reg;
};

int Assembler(Cpu *prc);
bool Is_registrs(FILE *fp);
void Creat_file(Instructions *commands, int count_commands, Cpu *prc);

#endif // ASSEMBLER_H_INCLUDED
