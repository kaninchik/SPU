#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED


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

const int reg_amount = sizeof(regs_name)/sizeof(regs_name[0]);;

#endif
