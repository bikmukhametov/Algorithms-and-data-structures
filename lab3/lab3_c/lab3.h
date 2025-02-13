#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item{                    //информация
    char* info;
} Item;

typedef struct KeySpace{           //элемент таблицы
    char* key;
    int release; // № с таким же ключом
    Item* info;
    struct KeySpace* next;
} KeySpace;

typedef struct Table{                      //таблица
    KeySpace** ks;
    int msize;
} Table;

int scan(int *x);
int my_readline(char **stroka);
void import(Table* table);
int func_index(char* stroka, int size);
int add(Table* table);
void show(Table* table);
void delet_key_and_release(Table* table, char** k, int r);
void delet_key(Table* table, char** k);
void delet_table(Table* table);
void input_file(Table* table, FILE** file);
void find_key(Table* table, char** k);
void find_key_and_release(Table* table, char** k, int r);
int dialog(Table* table);
