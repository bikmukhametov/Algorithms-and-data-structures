#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3.h"

int scan(int *x){                                       //проверка на целочисленное значение
    int m;
    while((m = scanf("%d", x)) != 1){
        if(m == 0){
            scanf("%*[^\n]");
            printf("Ошибка! Введите корректное значение: ");
        }else if(m == EOF){
            return 1;
        }
    }
    scanf("%*c");   ///////////
    return 0;
}

int my_readline(char **stroka){
    char *substring = NULL;
    int len = 0;
    int k = 1;
    while(k != 0){
        substring = (char *) calloc(81, sizeof(char));
        substring[80] = '\0';
        k = scanf("%80[^\n]", substring);
        if(k == -1){
            free(substring);
            *stroka = NULL;
            return 1;
        }else if(k == 1){
            int substring_len = strlen(substring);
            *stroka = (char *) realloc (*stroka, len + substring_len + 1);
            memcpy(*stroka + len, substring, substring_len);
            len += substring_len;
        }else{
            scanf("%*c");
        }
        free(substring);
    }
    if (len > 0){
        (*stroka)[len] = '\0';
    }else{
        *stroka = (char *) realloc (*stroka, sizeof(char) * 1);
        (*stroka)[len] = '\0';
    }
    return 0;
}

void import(Table* table){                              //внесение таблицы в файл
    if(table != NULL){
        FILE* file;
        file = fopen("file.txt", "w");
        KeySpace* keyspace = table->ks;
        while(keyspace != NULL){
            fprintf(file, "%s ", keyspace->key);
            fprintf(file, "%d ", keyspace->release);
            fprintf(file, "%s\n", keyspace->info->info);
            keyspace = keyspace->next;
        }
        fclose(file);
    }
}

int add(Table* table){                                  //добавление элемента в таблицу
    if(table != NULL){
        KeySpace* keyspace = (KeySpace*) calloc (1, sizeof(KeySpace));
        Item* item = (Item*) calloc (1, sizeof(Item));

        printf("key: ");
        int i = my_readline(&(keyspace->key));
        if(i == 1){
            free(keyspace->key);
            free(keyspace);
            free(item);
            return 1;
        }

        int rel = 0;                                    //внесение 'release'
        KeySpace* keyspace_release = table->ks;
        while(keyspace_release != NULL){
            if((keyspace_release->release > rel) && (strcmp(keyspace_release->key, keyspace->key) == 0)){
                rel = keyspace_release->release;
            }
            keyspace_release = keyspace_release->next;
        }
        rel += 1;
        keyspace->release = rel;

        printf("info: ");
        i = my_readline(&(item->info));

        if(i == 1){
            free(keyspace->key);
            free(keyspace);
            free(item->info);
            free(item);
            return 1;
        }

        keyspace->info = item;

        if(table->ks == NULL){
            table->ks = keyspace;
        }else{
            KeySpace* keyspace2 = table->ks;
            while(keyspace2->next != NULL){
                keyspace2 = keyspace2->next;
            }
            keyspace2->next = keyspace;
        }
    }
    return 0;
}

void show(Table* table){                                //показ таблицы
    if(table != NULL){
        KeySpace* keyspace = table->ks;
        while(keyspace != NULL){
            printf("%s ", keyspace->key);
            printf("%d ", keyspace->release);
            printf("%s\n", keyspace->info->info);
            keyspace = keyspace->next;
        }
    }
}

void delet_key_and_release(Table* table, char** k, int r){           //удаление по заданному 'Key' и 'Release'
    if(table != NULL){
        KeySpace* keyspace = table->ks;     //текущий
        KeySpace* keyspace2 = NULL;         //предыдущий
        while(keyspace != NULL){
            if((strcmp(keyspace->key, *k) == 0) && (keyspace->release == r)){
                if(table->ks == keyspace){
                    table->ks = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = table->ks;
                }else{
                    keyspace2->next = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = table->ks;
                }
            }else{
                keyspace2 = keyspace;
                keyspace = keyspace->next;
            }
        }
    }
}

void delet_key(Table* table, char** k){           //удаление по заданному 'Key'
    if(table != NULL){
        KeySpace* keyspace = table->ks;     //текущий
        KeySpace* keyspace2 = NULL;         //предыдущий
        while(keyspace != NULL){
            if(strcmp(keyspace->key, *k) == 0){
                if(table->ks == keyspace){
                    table->ks = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = table->ks;
                }else{
                    keyspace2->next = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = table->ks;
                }
            }else{
                keyspace2 = keyspace;
                keyspace = keyspace->next;
            }
        }
    }
}

void delet_table(Table* table){                      //чистка памяти (таблицы)
    if(table != NULL){
        KeySpace* keyspace2 = NULL;
        if(table != NULL){
            KeySpace* keyspace = table->ks;
            while(keyspace != NULL){
                free(keyspace->info->info);
                free(keyspace->info);
                free(keyspace->key);
                keyspace2 = keyspace;
                keyspace = keyspace->next;
                free(keyspace2);
            }
        }
        table->ks = NULL;
    }
}

void input_file(Table* table, FILE** file){          //считывание из файла
    if(*file != NULL){
        int e = 0;
        if(feof(*file) == 0){                //первый элемент
            KeySpace* keyspace = (KeySpace*) calloc (1, sizeof(KeySpace));
            Item* item = (Item*) calloc (1, sizeof(Item));
            keyspace->key = (char*) calloc (81, sizeof(char));
            item->info = (char*) calloc (81, sizeof(char));
            int k = 0;
            e = fscanf(*file, "%s%d%s", keyspace->key, &k, item->info);
            keyspace->release = k;
            if(e == EOF){                //чистка и не формируем новый элемент
                table->ks = NULL;
                free(keyspace->key);
                free(item->info);
                free(item);
                free(keyspace);
            }else{                       //формирование нового элемента
                table->ks = keyspace;
                keyspace->next = NULL;
                keyspace->info = item;
            }
            while(feof(*file) == 0){     //последующие элементы
                KeySpace* keyspace2 = (KeySpace*) calloc (1, sizeof(KeySpace));
                Item* item2 = (Item*) calloc (1, sizeof(Item));
                keyspace2->key = (char*) calloc (81, sizeof(char));
                item2->info = (char*) calloc (81, sizeof(char));
                int k2 = 0;
                e = fscanf(*file, "%s%d%s", keyspace2->key, &k2, item2->info);
                keyspace2->release = k2;
                if(e == EOF){       //чистка и не формируем новый элемент
                    keyspace->next = NULL;
                    free(keyspace2->key);
                    free(item2->info);
                    free(item2);
                    free(keyspace2);
                }else{              //формирование нового элемента
                    keyspace2->info = item2;
                    keyspace2->next = NULL;
                    keyspace->next = keyspace2;
                    keyspace = keyspace2;
                }
            }
        }
    }
}

void find_key(Table* table, char** k){                  //показ элементов таблицы по заданному 'Kлючу'
    if(table != NULL){
        KeySpace* keyspace = table->ks;
        while(keyspace != NULL){
            if(strcmp(keyspace->key, *k) == 0){
                printf("%s ", keyspace->key);
                printf("%d ", keyspace->release);
                printf("%s\n", keyspace->info->info);
            }
            keyspace = keyspace->next;
        }
    }
}

void find_key_and_release(Table* table, char** k, int r){       //показ элементов таблицы по заданному 'Kлючу' и 'Bерсии'
    if(table != NULL){
        KeySpace* keyspace = table->ks;
        while(keyspace != NULL){
            if((strcmp(keyspace->key, *k) == 0) && (keyspace->release == r)){
                printf("%s ", keyspace->key);
                printf("%d ", keyspace->release);
                printf("%s\n", keyspace->info->info);
            }
            keyspace = keyspace->next;
        }
    }
}

int dialog(Table* table){
    int i = 0;
    FILE* file;
    file = fopen("file.txt", "a+");
    input_file(table, &file);
    fclose(file);
    while(i == 0){
        const char *msgs[ ] = {"1. Add                                  -- 1. Добавить",
                               "2. Delete items by 'Key'                -- 2. Удаление по 'Ключу'",
                               "3. Delete items by 'Key' and 'Version'  -- 3. Удалить элементы по 'Ключу' и 'Версии'",
                               "4. Find items by 'Key'                  -- 4. Найти элементы по 'Ключу'",
                               "5. Find items by 'Key' and 'Version'    -- 5. Найти элементы по 'Ключу' и 'Версии'",
                               "6. Show                                 -- 6. Показать",
                               "7. Import                               -- 7. Запись в файл",
                               "8. Quit                                 -- 8. Выход"};
        printf("\n");
        for(int ms = 0; ms < 8; ms++){
            printf("%s\n", msgs[ms]);
        }
        int m = 0;
        printf("\n");
        printf("command: ");
        i = scan(&m);
        if(i == 1){
            delet_table(table);
            return 1;
        }
        printf("\n");
        switch(m){
            case 1:
                i = add(table);
                if(i == 1){
                    delet_table(table);
                    return 1;
                }
                break;
            case 2:
                printf("Enter the key: ");
		char* k2 = NULL;
                i = my_readline(&k2);
                if(i == 1){
                    free(k2);
                    delet_table(table);
                    return 1;
                }
                delet_key(table, &k2);
                free(k2);
                break;
            case 3:
                printf("Enter the key: ");
                char* k3 = NULL;
		i = my_readline(&k3);
                if(i == 1){
                    free(k3);
                    delet_table(table);
                    return 1;
                }
                printf("Enter the release: ");
                int r3;
                i = scan(&r3);
                if(i == 1){
                    delet_table(table);
                    free(k3);
                    return 1;
                }
                delet_key_and_release(table, &k3, r3);
                free(k3);
                break;
            case 4:
                printf("Enter the key: ");
                char* k4 = NULL;
		i = my_readline(&k4);
                printf("\n");
                if(i == 1){
                    free(k4);
                    delet_table(table);
                    return 1;
                }
                find_key(table, &k4);
                free(k4);
                break;
            case 5:
                printf("Enter the key: ");
                char* k5 = NULL;
		i = my_readline(&k5);
                if(i == 1){
                    free(k5);
                    delet_table(table);
                    return 1;
                }
                printf("Enter the release: ");
                int r5;
                i = scan(&r5);
                if(i == 1){
                    delet_table(table);
                    free(k5);
                    return 1;
                }
                printf("\n");
                find_key_and_release(table, &k5, r5);
                free(k5);
                break;
            case 6:
                show(table);
                break;
            case 7:
                import(table);
                break;
            case 8:
                delet_table(table);
                i = 1;
                break;
            default:
                printf("Ошибка! Введите число [1 , 8]: ");
                break;
        }
    }
    return 0;
}
