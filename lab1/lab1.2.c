#include <stdio.h>
#include <stdlib.h>

typedef struct Line{
    int n;
    int *array;
}Line;

typedef struct Matrix{
    int len_line;
    Line *line;
}Matrix;

int scan(int *x){
    int m;
    while((m = scanf("%d", x)) != 1){
        if(m == 0){
            scanf("%*[^\n]");
            printf("Ошибка! Некорректное значение: ");
        }else if(m == EOF){
            return 1;
        }
    }
    return 0;
}
void delet(Matrix *Matr){
    if(Matr->line != NULL){
        for(int i = 0; i < Matr->len_line; i++){
            free(Matr->line[i].array);
        }
        free(Matr->line);
    }
    Matr->len_line = 0;
    Matr->line = NULL;
}

void input(Matrix* Matr){
    int x = 0;
    int m = 0;
    int array_len = 0;
    int *p;
    Matr->line = (Line *) calloc(Matr->len_line, sizeof(Line));
    if((m == 0) && (array_len == 0)){
        for(int i = 0; i < Matr->len_line; i++){
            printf("--> количество столбцов в [%d] стороке: ", i);
            m = scan(&array_len);
            Matr->line[i].n = array_len;
            Matr->line[i].array = (int *) calloc (array_len, sizeof(int));
            printf("--> ");
            for(int j = 0; j < array_len; j++){
                if(m == 0){
                    m = scan(&x);
                    p = Matr->line[i].array;
                    p[j] = x;
                }
            }
            array_len = 0;
        }
    }
}

void output(Matrix* Matr){
    int p;
    for (int i = 0; i < Matr->len_line; i++){
        for (int j = 0; j < Matr->line[i].n; j++){
            p = Matr->line[i].array[j];
            printf("%d    ", p);
        }
        printf("\n\n");

    }
}

void func(int *x){
    int res = 0;
    int n;
    while(*x != 0){
        n = *x % 10;
        res = res * 10 + n;
        *x /= 10;
    }
    *x = res;
}

void creation(Matrix* Matr, Matrix* Matr_2){
    int x = 0;
    if(Matr->line != 0){
        Matr_2->line = (Line *) calloc(Matr->len_line, sizeof(Line));
        Matr_2->len_line = Matr->len_line;
        for(int i = 0; i < Matr->len_line; i++){
            Matr_2->line[i].array = (int *) calloc (Matr->line[i].n, sizeof(int));
            Matr_2->line[i].n = Matr->line[i].n;
            for(int j = 0; j < Matr->line[i].n; j++){
                x = Matr->line[i].array[j];
                func(&x);
                Matr_2->line[i].array[j] = x;
            }
        }
    }
}
