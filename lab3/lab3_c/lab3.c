#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3.h"

int scan(int *x){                                       //проверка на целочисленное значение
    int m = 0;
    int x_2 = 0;
    m = scanf("%d", &x_2);
    while(m != 1){
        if(m == 0){
            scanf("%*[^\n]");
            printf("Ошибка! Введите корректное значение: ");
            m = scanf("%d", &x_2);
        }else if(m == EOF){
            return 1;
        }
    }
    scanf("%*c");   ///////////
    *x = x_2;
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
        for (int i = 0; i < table->msize; i++){
            KeySpace* keyspace = table->ks[i];
            while(keyspace != NULL){
                fprintf(file, "%s ", keyspace->key);
                fprintf(file, "%d ", keyspace->release);
                fprintf(file, "%s\n", keyspace->info->info);
                keyspace = keyspace->next;
            }
        }
        fclose(file);
    }
}

int func_index(char* stroka, int size){
    if(stroka != NULL){
        int p = 11;
        int res = 0;
        int res_current = 0;
        int s = 0;
        for(int i = 0; i < strlen(stroka); i++){
            s = stroka[i] - '0';
            for(int j = strlen(stroka) - i - 1; j > 0; j--){
                p = p * p;
            }
            res_current = s * p;
            res += res_current;
            p = 17;
        }
        int index = res % size;
        if(index < 0){
            index = -index;
        }
        return index;
    }else{
        return -1;
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

        printf("info: ");
        i = my_readline(&(item->info));
        if(i == 1){
            free(keyspace->key);
            free(keyspace);
            free(item->info);
            free(item);
            return 1;
        }

        if((keyspace->key[0] == '\0') || (item->info[0] == '\0')){
            printf("Ошибка! Пустое значение\n");
            free(keyspace->key);
            free(keyspace);
            free(item->info);
            free(item);
            return 0;
        }
        keyspace->info = item;

        int index = func_index(keyspace->key, table->msize);

        int rel = 1; //внесение 'release'
        KeySpace* keyspace_rel = table->ks[index];
        while(keyspace_rel != NULL){
            if(strcmp(keyspace_rel->key, keyspace->key) == 0){
                rel = keyspace_rel->release + 1;
		break;
            }
            keyspace_rel = keyspace_rel->next;
        }
        keyspace->release = rel;

        keyspace->info = item;
        keyspace->next = table->ks[index];
        table->ks[index] = keyspace;
    }
    return 0;
}

void show(Table* table){                                //показ таблицы
    if(table != NULL){
        for (int i = 0; i < table->msize; i++) {
            KeySpace* keyspace = table->ks[i];
            printf("[%d] --> ", i);
            while(keyspace != NULL){
                if(keyspace != table->ks[i]) printf("        ");
                printf("%s ", keyspace->key);
                printf("%d ", keyspace->release);
                printf("%s\n", keyspace->info->info);
                keyspace = keyspace->next;
            }
            printf("\n");
        }
    }
}

void delet_key_and_release(Table* table, char** k, int r){           //удаление по заданному 'Key' и 'Release'
    if(table != NULL){
        int index = func_index(*k, table->msize);
        KeySpace* keyspace = table->ks[index]; //текущий
        KeySpace* keyspace2 = NULL;           //предыдущий
        while(keyspace != NULL){
            if((keyspace->release == r) && (strcmp(keyspace->key, *k) == 0)){
                if(table->ks[index] == keyspace){
                    table->ks[index] = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = table->ks[index];
                }else{
                    keyspace2->next = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = NULL;
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
        int index = func_index(*k, table->msize);
        KeySpace* keyspace = table->ks[index]; //текущий
        KeySpace* keyspace2 = NULL;           //предыдущий
        while(keyspace != NULL){
            if(strcmp(keyspace->key, *k) == 0){
                if(table->ks[index] == keyspace){
                    table->ks[index] = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = table->ks[index];
                }else{
                    keyspace2->next = keyspace->next;
                    free(keyspace->info->info);
                    free(keyspace->info);
                    free(keyspace->key);
                    free(keyspace);
                    keyspace = keyspace2->next;
                }
            }else{
                keyspace2 = keyspace;
                keyspace = keyspace->next;
            }
        }
    }
}

void delet_table(Table* table){                      //чистка памяти (таблицы)
    if(table != NULL) {
        for (int i = 0; i < table->msize; i++) {
            KeySpace *keyspace = table->ks[i];
            while (table->ks[i] != NULL) {
                table->ks[i] = keyspace->next;
                free(keyspace->info->info);
                free(keyspace->info);
                free(keyspace->key);
                free(keyspace);
                keyspace = table->ks[i];
            }
        }
        free(table->ks);
        table->ks = NULL;
    }
}

void input_file(Table* table, FILE** file){                       //считывание из файла
    while((*file != NULL) && feof(*file) == 0){
        int e = 0;
        KeySpace* keyspace = (KeySpace*) calloc (1, sizeof(KeySpace));
        int index = func_index(keyspace->key, table->msize);
        Item* item = (Item*) calloc (1, sizeof(Item));
        keyspace->key = (char*) calloc (81, sizeof(char));
        item->info = (char*) calloc (81, sizeof(char));
        int k = 0;
        e = fscanf(*file, "%s%d%s", keyspace->key, &k, item->info);
        keyspace->release = k;
	keyspace->key = (char*) realloc (keyspace->key, (strlen(keyspace->key) + 1) * sizeof(char));
	keyspace->key[strlen(keyspace->key)] = '\0';
	item->info = (char*) realloc (item->info, (strlen(item->info) + 1) * sizeof(char));
	item->info[strlen(item->info)] = '\0';

        if(e == EOF){      //чистка и не формируем новый элемент
            free(keyspace->key);
            free(item->info);
            free(item);
            free(keyspace);
        }else{                    //формирование нового элемента
            int index = func_index(keyspace->key, table->msize);
            keyspace->info = item;
            if(table->ks[index] == NULL){
                table->ks[index] = keyspace;
            }else{
                KeySpace* keyspace2 = table->ks[index];
                while(keyspace2->next != NULL){
                    keyspace2 = keyspace2->next;
                }
                keyspace2->next = keyspace;
            }
        }
    }
}

void find_key(Table* table, char** k){              //показ элементов таблицы по заданному 'Kлючу'
    if(table != NULL){
        int index = func_index(*k, table->msize);
        KeySpace* keyspace = table->ks[index];
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
        int index = func_index(*k, table->msize);
        KeySpace* keyspace = table->ks[index];
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
    int size = 0;
    while(size == 0) {
        printf("enter the size of the table: ");
        i = scan(&size);
        if(i == 1){
            fclose(file);
            delet_table(table);
            return 1;
        }
        if(size == 0){
            printf("error\n");
        }
    }
    table->msize = size;
    table->ks = (KeySpace**) calloc (table->msize, sizeof(KeySpace*));
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
                char* stroka2 = NULL;
                i = my_readline(&stroka2);
                if(i == 1){
                    free(stroka2);
                    delet_table(table);
                    return 1;
                }
                delet_key(table, &stroka2);
                free(stroka2);
                break;
            case 3:
                printf("Enter the key: ");
                char* stroka3 = NULL;
                i = my_readline(&stroka3);
                if(i == 1){
                    free(stroka3);
                    delet_table(table);
                    return 1;
                }
                printf("Enter the release: ");
                int r3;
                i = scan(&r3);
                if(i == 1){
                    delet_table(table);
                    free(stroka3);
                    return 1;
                }
                delet_key_and_release(table, &stroka3, r3);
                free(stroka3);
                break;
            case 4:
                printf("Enter the key: ");
                char* stroka4 = NULL;
                i = my_readline(&stroka4);
                printf("\n");
                if(i == 1){
                    free(stroka4);
                    delet_table(table);
                    return 1;
                }
                find_key(table, &stroka4);
                free(stroka4);
                break;
            case 5:
                printf("Enter the key: ");
                char* stroka5 = NULL;
                i = my_readline(&stroka5);
                if(i == 1){
                    free(stroka5);
                    delet_table(table);
                    return 1;
                }
                printf("Enter the release: ");
                int r5;
                i = scan(&r5);
                if(i == 1){
                    delet_table(table);
                    free(stroka5);
                    return 1;
                }
                printf("\n");
                find_key_and_release(table, &stroka5, r5);
                free(stroka5);
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
