#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"stack.h"
#include"security.h"
#include"dump.h"


int Stack_ctor(My_stack *stk)
{
    if(stk == NULL)
    {
        STACK_DUMP_IF_ERROR(stk);

        return STACK_IS_NULL;
    }

    stk->size_stack = 0;
    stk->capacity = INITIAL_CAPACITY;

    #ifdef STK_PROTECT

    stk->left_stk_canary = STK_CANARY;
    stk->right_stk_canary = STK_CANARY;

    stk->data = (Elem_t* )calloc(stk->capacity*sizeof(Elem_t) + 2*sizeof(Canary_t), 1);

    #else

    stk->data = (Elem_t* )calloc(stk->capacity, sizeof(Elem_t));

    #endif

    if(stk->data == NULL)
    {
        stk->status |= STK_DATA_IS_NULL;
        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    #ifdef STK_PROTECT

    stk->data = (Elem_t *)((char *)stk->data + sizeof(Canary_t));

    ((Canary_t *)stk->data)[-1] = STK_CANARY;
    *(Canary_t *)((Elem_t *)stk->data + stk->capacity) = STK_CANARY;

    #endif

    Fill_poison(stk);

    #ifdef STK_PROTECT

    Stack_hash(stk);

    #endif

    return stk->status;

}

void Stack_dtor(My_stack *stk)
{
    stk->data = NULL;
    stk->size_stack = -1;
    stk->capacity = 0;
    stk->file = NULL;
    stk->name = NULL;
    stk->func = NULL;
    stk->line = 0;

    #ifdef STK_PROTECT

    free((char *)stk->data - sizeof(Canary_t));

    #else

    free(stk->data);

    #endif
}

int Stack_push(My_stack *stk, Elem_t value)
{
    if(Stack_check(stk) != NO_ERRORS)
    {
        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    Stack_realloc(stk);

    stk->data[stk->size_stack] = value;

    stk->size_stack++;

    Fill_poison(stk);

    #ifdef STK_PROTECT

    Stack_hash(stk);

    #endif

    return stk->status;

}

int Stack_pop(My_stack *stk, Elem_t *x)
{
    if(x == NULL)
    {
        stk->status = VALUE_POINTER_IS_NULL;

        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }


    if(Stack_check(stk) != NO_ERRORS)
    {
        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    *x = stk->data[stk->size_stack - 1];
    stk->size_stack--;

    Stack_realloc(stk);

    Fill_poison(stk);

    #ifdef STK_PROTECT

    Stack_hash(stk);

    #endif

    return stk->status;
}

int Fill_poison(My_stack *stk)
{
    for(int i = stk->size_stack; i < stk->capacity; i++)
        stk->data[i] = POISON;

    return stk->status;
}

int Stack_realloc(My_stack *stk)
{

    int prev_capacity = stk->capacity;

    if(stk->size_stack + 1 >= stk->capacity)
        stk->capacity = INCREASE_CAPACITY*stk->capacity;

    if(stk->size_stack < stk->capacity/2
       && stk->capacity > 4)
    {
    stk->capacity = stk->capacity/DECRISE_CAPACITY;
    }

    if(stk->capacity == prev_capacity)
        return stk->status;

    #ifdef STK_PROTECT

    stk->data = (Elem_t *)((char *)stk->data - sizeof(Canary_t));
    stk->data = (Elem_t *)realloc(stk->data, stk->capacity*sizeof(Elem_t) + 2*sizeof(Canary_t));

    #else

    stk->data = (Elem_t *)realloc(stk->data, stk->capacity*sizeof(Elem_t));

    #endif

    if(stk->data == NULL)
    {
        stk->status |= STK_DATA_IS_NULL;

        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    #ifdef STK_PROTECT

    stk->data = (Elem_t *)((char *)stk->data + sizeof(Canary_t));

    *(Canary_t *)((Elem_t *)(stk->data) + stk->capacity) = STK_CANARY;

    #endif

    return stk->status;
}








