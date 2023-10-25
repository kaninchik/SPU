#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

static const char *registers_name[] = {
"rax", "rbx", "rcx", "rdx",
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

const int reg_amount = sizeof(registers_name)/sizeof(registers_name[0]);

enum Errors_prc
{
    NO_ERRORS_PRC                 = 0,
    ERROR_WITH_OPENNING_FILE      = 1 << 0,
    ERROR_WITH_READING_FROM_FILE  = 1 << 1,
    ERROR_WITH_WRITING_IN_FILE    = 1 << 2,
    ARRAY_IS_NULLPTR              = 1 << 3
};

const uint8_t imm = 1 << 5;
const uint8_t reg = imm << 1;
const uint8_t osu = reg << 1;
const uint8_t cmd = 0b11111;

static_assert((imm & reg) == 0, "imm and reg overlap");
static_assert((imm & cmd) == 0, "imm and cmd overlap");
static_assert((imm & osu) == 0, "imm and osu overlap");
static_assert((reg & osu) == 0, "reg and osu overlap");
static_assert((reg & cmd) == 0, "reg and cmd overlap");
static_assert((osu & cmd) == 0, "osu and cmd overlap");

#endif
