#ifndef SECURITY_H_INCLUDED
#define SECURITY_H_INCLUDED

#include"stack.h"

int Stack_check(My_stack *stk);
int Stack_hash(My_stack *stk);
bool Check_hash(My_stack *stk);

#endif // SECURITY_H_INCLUDED
