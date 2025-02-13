#include <stdio.h>
#include <stdlib.h>
#include "lab1.2.h"

int main(){
    Matrix Matr = {0, NULL};
    printf("--> количество строк: ");
    int x = 0;
    scan(&x);
    printf("\n");
    Matr.len_line = x;
    input(&Matr);
    printf("\n\n");
    printf("*Исходная матрица* :\n");
    output(&Matr);
    Matrix Matr_2 = {0, NULL};
    creation(&Matr, &Matr_2);
    printf("\n");
    printf("*Новая матрица* :\n");
    output(&Matr_2);
    delet(&Matr);
    delet(&Matr_2);
    return 0;
}
