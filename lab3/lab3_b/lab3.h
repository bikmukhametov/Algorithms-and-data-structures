#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item{                             //элемент таблицы
    int len_k; // - длина ключа
    char* key;
    int release;    // - порядковый номер с таким ключом
    int offset;
    int len;
    struct Item* next;
} Item;

typedef struct Table{                                   //таблица
    Item *first;
    char* fname;
    FILE *fd;
} Table;

int scan(int *x);
int my_readline(char **stroka);
int save(Table* table);
int add(Table* table);
void show(Table* table);
void delet_key_and_release(Table* table, char** k, int r);
void delet_key(Table* table, char** k);
void delet_table(Table* table);
int load (Table *table, char *fname);
int input_file(Table* table);
void find_key(Table* table, char** k);
void find_key_and_release(Table* table, char** k, int r);
int dialog(Table* table);
int insert(Table *table, char* k, char *str);
