#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include"processor.h"

const int INSTRUCTIONS = 10;
const int LABELS = 10;

struct Labels
{
    int label = 0;
    char label_name[LABELS] = {};
};

int Assembler(Spu *prc);
int Label_name(char *arg_str, Labels lbs[LABELS]);
int What_reg(char *arg_str);
bool Is_registrs(char *instructions);
bool Is_label(char* func_name);
void Create_file(Spu *prc);
char *Read_instructions(Spu *prc, int *n_strings);


#endif // ASSEMBLER_H_INCLUDED
