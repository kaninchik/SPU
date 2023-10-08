#ifndef DISASSEMBLER_H_INCLUDED
#define DISASSEMBLER_H_INCLUDED

enum Functions
{
    push = 1,
    in = 2,
    div = 3,
    sub = 4,
    add = 5,
    mul = 6,
    out = 7,
    pop = 11,
    hlt = -1
};


int Disassembler();
void Write_reg(FILE *fp2, int Arg);
void Define_func(FILE *fp1, FILE *fp2, int n_func);

#endif // DISASSEMBLER_H_INCLUDED
