#include <stdio.h>
#include <stdlib.h>

typedef struct Stack{
    int n;
    int *array;
}Stack;

void m_push(Stack* stack, int i);
void m_pop(Stack* stack, int j);
void delet(Stack* stack);
void print_result(Stack* stack);
void input(Stack* stack);
char *in_str();
void func(Stack* stack, char** stroka);
