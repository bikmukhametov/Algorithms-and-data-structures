typedef struct Line{
    int n;
    int *array;
}Line;

typedef struct Matrix{
    int len_line;
    Line *line;
}Matrix;

int scan(int *x);
void input(Matrix* Matr);
void output(Matrix* Matr);
void func(int *x);
void creation(Matrix* Matr, Matrix* Matr_2);
void delet(Matrix *Matr);
