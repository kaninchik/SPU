#ifndef DISASSEMBLER_H_INCLUDED
#define DISASSEMBLER_H_INCLUDED

int Disassembler(Cpu *prc);
void Write_reg(FILE *fp2, int Arg);
void Define_func(FILE *fp1, FILE *fp2, int n_func);

#endif // DISASSEMBLER_H_INCLUDED
